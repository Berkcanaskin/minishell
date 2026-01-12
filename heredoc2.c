/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 19:44:59 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 21:22:26 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_heredoc(char *limiter, char **filename, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*tmp;

	tmp = generate_tmp_filename();
	*filename = ft_strdup(tmp);
	free(tmp);
	pid = fork();
	if (pid == 0)
		child_heredoc(limiter, *filename, shell);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
			return (1);
	}
	else
	{
		perror("fork");
		return (1);
	}
	return (0);
}

int	redirect_heredoc_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("heredoc read");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("heredoc dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
