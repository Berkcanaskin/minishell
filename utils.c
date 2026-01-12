/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:59 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:52:01 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_input(t_shell *shell)
{
	t_cmd	*tmp;

	if (!shell)
		return ;
	free(shell->input);
	shell->input = NULL;
	while (shell->cmds)
	{
		tmp = shell->cmds->next;
		free_cmd_fields(shell->cmds);
		free(shell->cmds);
		shell->cmds = tmp;
	}
	shell->cmds = NULL;
}

void	cleanup_shell(t_shell *shell)
{
	free_input(shell);
	free_env(shell->env);
}

void	cleanup_heredoc_files(t_cmd *cmds)
{
	while (cmds)
	{
		if (cmds->heredoc)
		{
			if (cmds->heredoc_file)
			{
				unlink(cmds->heredoc_file);
				free(cmds->heredoc_file);
				cmds->heredoc_file = NULL;
			}
			if (cmds->heredoc_limiter)
			{
				free(cmds->heredoc_limiter);
				cmds->heredoc_limiter = NULL;
			}
		}
		cmds = cmds->next;
	}
}
