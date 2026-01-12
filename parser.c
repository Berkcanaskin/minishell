/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:29 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:51:31 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_cmd(t_shell *shell, t_cmd *cmd, t_cmd **last)
{
	if (!shell->cmds)
		shell->cmds = cmd;
	else
		(*last)->next = cmd;
	*last = cmd;
}

static void	parse_segments(t_shell *shell, char **segments)
{
	t_cmd	*cmd;
	t_cmd	*last;
	int		i;

	i = 0;
	last = NULL;
	while (segments[i])
	{
		cmd = create_cmd_from_segment(segments[i]);
		if (cmd)
			append_cmd(shell, cmd, &last);
		free(segments[i]);
		i++;
	}
}

int	parse_input(t_shell *shell)
{
	char	**segments;

	if (is_syntax_error(shell->input))
	{
		shell->exit_status = 1;
		return (0);
	}
	segments = split_pipe_segments(shell->input);
	if (!segments || !segments[0])
		return (0);
	parse_segments(shell, segments);
	free(segments);
	return (1);
}

static int	skip_and_find_token(const char *str, int i, char *quote)
{
	while (str[i])
	{
		if (!*quote && (str[i] == '\'' || str[i] == '"'))
			*quote = str[i];
		else if (*quote && str[i] == *quote)
			*quote = 0;
		else if (!*quote && str[i] == ' ')
			break ;
		i++;
	}
	return (i);
}

char	**tokenize_with_quotes(const char *str)
{
	char	**tokens;
	int		i;
	int		j;
	int		start;
	char	quote;

	tokens = malloc(sizeof(char *) * 256);
	if (!tokens)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		start = i;
		i = skip_and_find_token(str, i, &quote);
		tokens[j++] = ft_substr(str, start, i - start);
	}
	tokens[j] = NULL;
	return (tokens);
}
