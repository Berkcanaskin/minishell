/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:08 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:51:11 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_op_error(const char *input, int i)
{
	if (input[i] == '>' && input[i + 1] == '>')
		write(2, ">>", 2);
	else if (input[i] == '<' && input[i + 1] == '<')
		write(2, "<<", 2);
	else
		write(2, &input[i], 1);
	write(2, "'\n", 2);
}

static int	is_operator_syntax_error(const char *input, int i)
{
	int	j;

	j = i + 1;
	if ((input[i] == '<' && input[j] == '<')
		|| (input[i] == '>' && input[j] == '>'))
		j++;
	while (input[j] && input[j] == ' ')
		j++;
	if (!input[j] || input[j] == '|' || input[j] == '<' || input[j] == '>')
	{
		print_op_error(input, i);
		return (1);
	}
	return (0);
}

static int	check_unclosed_quote(char quote)
{
	if (quote)
	{
		ft_putstr_fd("minishell: syntax error: unexpected EOF while "
			"looking for matching quote\n", 2);
		return (1);
	}
	return (0);
}

int	is_syntax_error(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else if (!quote && (input[i] == '|' || input[i] == '<'
				|| input[i] == '>'))
		{
			if (is_operator_syntax_error(input, i))
				return (1);
		}
		i++;
	}
	return (check_unclosed_quote(quote));
}
