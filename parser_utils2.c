/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:16 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:51:18 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	char	quote;
	char	*res;

	i = 0;
	j = 0;
	quote = 0;
	res = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"'))
			quote = str[i++];
		else if (quote && str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

static void	split_segment_helper(char *input, int *k, char *quote)
{
	while (input[*k])
	{
		if (!*quote && (input[*k] == '\'' || input[*k] == '"'))
			*quote = input[*k];
		else if (*quote && input[*k] == *quote)
			*quote = 0;
		else if (!*quote && input[*k] == '|')
			break ;
		(*k)++;
	}
}

char	**split_pipe_segments(char *input)
{
	char	**segments;
	int		i;
	int		k;
	int		start;
	char	quote;

	segments = malloc(sizeof(char *) * 256);
	if (!segments)
		return (NULL);
	i = 0;
	k = 0;
	start = 0;
	quote = 0;
	while (input[k])
	{
		split_segment_helper(input, &k, &quote);
		segments[i++] = ft_substr(input, start, k - start);
		if (input[k] == '|')
			k++;
		while (input[k] == ' ')
			k++;
		start = k;
	}
	segments[i] = NULL;
	return (segments);
}
