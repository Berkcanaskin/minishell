/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:10:26 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:10:28 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_if_empty(t_cmd *cmd, int *i, char *expanded)
{
	int	j;

	if (expanded[0] == '\0')
	{
		free(expanded);
		j = *i;
		while (cmd->args[j])
		{
			cmd->args[j] = cmd->args[j + 1];
			j++;
		}
	}
	else
	{
		cmd->args[*i] = expanded;
		(*i)++;
	}
}

static void	expand_one_arg(t_shell *sh, t_cmd *cmd, int *i)
{
	char	*arg;
	char	*tmp;

	arg = cmd->args[*i];
	if (is_fully_single_quoted(arg))
	{
		tmp = remove_quotes(arg);
		free(arg);
		cmd->args[*i] = tmp;
		(*i)++;
	}
	else
	{
		tmp = expand_string(arg, sh);
		free(arg);
		remove_if_empty(cmd, i, tmp);
	}
}

void	expand_variables(t_shell *shell)
{
	t_cmd	*cmd;
	int		i;

	cmd = shell->cmds;
	while (cmd)
	{
		i = 0;
		while (cmd->args && cmd->args[i])
			expand_one_arg(shell, cmd, &i);
		cmd = cmd->next;
	}
}
