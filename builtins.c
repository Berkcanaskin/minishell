/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:08:07 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:08:09 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_echo(t_cmd *cmd)
{
	int		i;
	int		newline;
	char	*home;

	i = 1;
	newline = 1;
	if (cmd->args[i] && !ft_strcmp(cmd->args[i], "-n"))
	{
		newline = 0;
		i++;
	}
	else if (cmd->args[i] && !ft_strcmp(cmd->args[i], "~"))
	{
		home = getenv("HOME");
		printf("%s", home);
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	exec_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, sizeof(buf)))
		printf("%s\n", buf);
	else
		perror("pwd");
}

static char	*get_target_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->args[1] || !ft_strcmp(cmd->args[1], "~"))
	{
		path = get_env(shell->env, "HOME");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (!ft_strcmp(cmd->args[1], "-"))
	{
		path = get_env(shell->env, "OLDPWD");
		if (!path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		printf("%s\n", path);
	}
	else
		path = cmd->args[1];
	return (path);
}

static void	update_pwd_vars(t_shell *shell, char *old_pwd)
{
	char	new_pwd[1024];

	if (!getcwd(new_pwd, sizeof(new_pwd)))
	{
		perror("minishell: cd getcwd");
		shell->exit_status = 1;
		return ;
	}
	env_set(&shell->env, ft_strdup("OLDPWD"), ft_strdup(old_pwd));
	env_set(&shell->env, ft_strdup("PWD"), ft_strdup(new_pwd));
}

void	exec_cd(t_shell *shell, t_cmd *cmd)
{
	char	old_pwd[1024];
	char	*path;

	if (cmd->args[1] && cmd->args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2),
			shell->exit_status = 1, (void)0);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (perror("minishell: cd getcwd"), shell->exit_status = 1,
			(void)0);
	path = get_target_path(shell, cmd);
	if (!path)
		return (shell->exit_status = 1, (void)0);
	if (chdir(path) != 0)
		return (perror("minishell: cd"), shell->exit_status = 1, (void)0);
	update_pwd_vars(shell, old_pwd);
	shell->exit_status = 0;
}
