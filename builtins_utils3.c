/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:07:08 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:07:15 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_env(t_shell *shell)
{
	t_env	*env;

	env = shell->env;
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	shell->exit_status = 0;
}

char	*get_value_part(const char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (ft_strdup(""));
	return (ft_strdup(equal + 1));
}

void	exec_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->args[1])
	{
		shell->exit_status = 0;
		return ;
	}
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n",
				cmd->args[i]);
			shell->exit_status = 1;
			return ;
		}
		env_unset(&shell->env, cmd->args[i]);
		i++;
	}
	shell->exit_status = 0;
}
