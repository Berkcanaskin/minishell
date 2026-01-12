/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 05:28:59 by baskin            #+#    #+#             */
/*   Updated: 2025/06/27 23:50:16 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_tmp_filename(void)
{
	static int	count = 0;
	char		*num;
	char		*tmp;
	char		*res;

	num = ft_itoa(count++);
	tmp = ft_strjoin("/tmp/.heredoc_", num);
	free(num);
	res = ft_strdup(tmp);
	free(tmp);
	return (res);
}

static void	write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static void	heredoc_read_lines(int fd, char *limiter, t_shell *shell,
		int quoted)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
			break ;
		if (!quoted)
		{
			expanded = expand_string(line, shell);
			write_line(fd, expanded);
			free(expanded);
		}
		else
			write_line(fd, line);
		free(line);
	}
	free(line);
}

static void	heredoc_loop(int fd, char *limiter, t_shell *shell)
{
	char	*clean_limiter;
	int		quoted;

	quoted = is_fully_single_quoted(limiter);
	if (quoted)
		clean_limiter = remove_quotes(limiter);
	else
		clean_limiter = ft_strdup(limiter);
	if (!clean_limiter)
		return ;
	heredoc_read_lines(fd, clean_limiter, shell, quoted);
	free(clean_limiter);
}

void	child_heredoc(char *limiter, char *filename, t_shell *shell)
{
	int	fd;

	setup_child_signals();
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		exit(1);
	heredoc_loop(fd, limiter, shell);
	close(fd);
	exit(0);
}
