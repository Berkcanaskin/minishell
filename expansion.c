/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:10:34 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:10:36 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_status(int status)
{
	return (ft_itoa(status));
}

char	*expand_var(const char *name, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(env->key, name))
		{
			if (env->value == NULL)
				return (ft_strdup(""));
			return (ft_strdup(env->value));
		}
		env = env->next;
	}
	return (ft_strdup(""));
}

int	is_fully_single_quoted(const char *s)
{
	if (!s || ft_strlen(s) < 2)
		return (0);
	return (s[0] == '\'' && s[ft_strlen(s) - 1] == '\'');
}
