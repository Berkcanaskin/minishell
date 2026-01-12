/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:09:25 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:09:30 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

void	execute_builtin(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		exec_echo(cmd);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		exec_cd(shell, cmd);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		exec_pwd();
	else if (!ft_strcmp(cmd->args[0], "export"))
		exec_export(shell, cmd);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		exec_unset(shell, cmd);
	else if (!ft_strcmp(cmd->args[0], "env"))
		exec_env(shell);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		exec_exit(shell, cmd);
}

static void	free_env_array(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

static void	external_exit(int code, char *msg, char *path, DIR *dir)
{
	if (msg && path)
		printf("minishell: %s: %s\n", path, msg);
	if (dir)
		closedir(dir);
	if (path)
		free(path);
	exit(code);
}

void	execute_external(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**env;
	DIR		*dir;

	path = find_executable(cmd->args[0], shell->env);
	if (!path)
		external_exit(127, "command not found", cmd->args[0], NULL);
	if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
	{
		perror(path);
		free(path);
		exit(126);
	}
	dir = opendir(path);
	if (dir)
		external_exit(126, "is a directory", path, dir);
	env = env_to_array(shell->env);
	execve(path, cmd->args, env);
	perror(path);
	free_env_array(env);
	free(path);
	exit(127);
}
