/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:10:08 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:10:10 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char(char **res, char c)
{
	char	tmp[2];
	char	*new;

	tmp[0] = c;
	tmp[1] = '\0';
	new = ft_strjoin(*res, tmp);
	free(*res);
	*res = new;
}

void	get_var_value_and_append(char **res, char *var, t_shell *sh)
{
	char	*val;
	char	*tmp;

	val = expand_var(var, sh->env);
	if (!val)
		val = ft_strdup("");
	tmp = ft_strjoin(*res, val);
	free(*res);
	*res = tmp;
	free(val);
}

void	append_str(char **dest, const char *src)
{
	char	*tmp;

	if (!src)
		return ;
	if (!*dest)
		tmp = ft_strdup(src);
	else
		tmp = ft_strjoin(*dest, src);
	free(*dest);
	*dest = tmp;
}

int	handle_dollar_expansion(char **res, char *str, int i, t_shell *sh)
{
	i++;
	if (!str[i] || (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '?'))
		append_char(res, '$');
	else
		i = append_var(res, str, i, sh);
	return (i);
}

char	*expand_string(char *str, t_shell *sh)
{
	if (!str)
		return (NULL);
	return (expand_loop(str, sh, 0, 0));
}
