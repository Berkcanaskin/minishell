/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:10:15 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:10:17 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_var(char **res, const char *str, int i, t_shell *sh)
{
	char	var[256];
	int		j;
	char	*code;
	char	*tmp;

	j = 0;
	if (str[i] == '?')
	{
		code = ft_itoa(sh->exit_status);
		tmp = ft_strjoin(*res, code);
		free(*res);
		*res = tmp;
		free(code);
		return (i + 1);
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		var[j++] = str[i++];
	var[j] = '\0';
	get_var_value_and_append(res, var, sh);
	return (i);
}

static void	handle_tilde(char **res, char *str, int *i, t_shell *sh)
{
	if (*i == 0 && str[*i] == '~'
		&& (str[*i + 1] == '/' || str[*i + 1] == '\0'))
	{
		append_str(res, get_env_value(sh->env, "HOME"));
		(*i)++;
	}
	else
		append_char(res, str[(*i)++]);
}

char	*expand_loop(char *str, t_shell *sh, int i, char q)
{
	char	*res;

	res = ft_calloc(1, 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (!q && (str[i] == '\'' || str[i] == '"'))
			q = str[i++];
		else if (q && str[i] == q)
		{
			q = 0;
			i++;
		}
		else if (str[i] == '~' && q != '\'')
			handle_tilde(&res, str, &i, sh);
		else if (str[i] == '$' && q != '\'')
			i = handle_dollar_expansion(&res, str, i, sh);
		else
			append_char(&res, str[i++]);
	}
	return (res);
}
