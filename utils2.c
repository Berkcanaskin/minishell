/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:56:35 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 21:57:32 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_cmd_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

static void	free_cmd_outfiles(char **outfiles, int count)
{
	int	i;

	if (!outfiles)
		return ;
	i = 0;
	while (i < count)
		free(outfiles[i++]);
	free(outfiles);
}

void	free_cmd_fields(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_cmd_args(cmd->args);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->heredoc_file)
		free(cmd->heredoc_file);
	if (cmd->heredoc_limiter)
		free(cmd->heredoc_limiter);
	free_cmd_outfiles(cmd->outfiles, cmd->out_count);
	free(cmd->append_flags);
}
