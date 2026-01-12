/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:07:50 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:07:52 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	export_var(t_shell *shell, char *arg)
{
	char	*key;
	char	*val;

	key = ft_substr(arg, 0, key_len(arg));
	val = get_value_part(arg);
	env_set(&shell->env, key, val);
	free(val);
	free(key);
}

static void	export_handle_invalid(t_shell *shell, char *arg)
{
	char	*key;
	char	*val;

	key = ft_substr(arg, 0, key_len(arg));
	val = get_value_part(arg);
	printf("minishell: export: `%s`: not a valid identifier\n", arg);
	shell->exit_status = 1;
	free(key);
	free(val);
}

static void	export_handle_arg(t_shell *shell, char *arg)
{
	if (!is_valid_identifier(arg))
		export_handle_invalid(shell, arg);
	else
		export_var(shell, arg);
}

static void	export_export_args(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		export_handle_arg(shell, cmd->args[i]);
		i++;
	}
}

void	exec_export(t_shell *shell, t_cmd *cmd)
{
	shell->exit_status = 0;
	if (!cmd->args[1])
		print_sorted_env(shell->env);
	else
		export_export_args(shell, cmd);
}
