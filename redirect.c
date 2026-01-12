/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:46 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:51:48 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_input(t_cmd *cmd)
{
	int		fd;
	char	*infile;

	if (!cmd->infile)
		return (0);
	infile = remove_quotes(cmd->infile);
	fd = open(infile, O_RDONLY);
	if (fd < 0)
	{
		printf("%s: %s\n", infile, strerror(errno));
		free(infile);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 infile");
		close(fd);
		free(infile);
		return (1);
	}
	close(fd);
	free(infile);
	return (0);
}

static int	open_outfile(char *outfile, int append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(outfile, flags, 0644);
	if (fd < 0)
	{
		printf("%s: %s\n", outfile, strerror(errno));
		free(outfile);
		exit(1);
	}
	return (fd);
}

static int	redirect_output(t_cmd *cmd)
{
	int		i;
	int		fd;
	char	*outfile;

	i = 0;
	while (i < cmd->out_count)
	{
		outfile = remove_quotes(cmd->outfiles[i]);
		fd = open_outfile(outfile, cmd->append_flags[i]);
		if (i == cmd->out_count - 1)
		{
			if (dup2(fd, STDOUT_FILENO) < 0)
			{
				perror("dup2 outfile");
				close(fd);
				free(outfile);
				exit(1);
			}
		}
		close(fd);
		free(outfile);
		i++;
	}
	return (0);
}

int	handle_redirections(t_cmd *cmd)
{
	if (cmd->infile && redirect_input(cmd))
		return (1);
	if (cmd->outfiles && redirect_output(cmd))
		return (1);
	return (0);
}
