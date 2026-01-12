/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:08:32 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:08:34 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_new(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	if (key)
		new->key = ft_strdup(key);
	else
		new->key = ft_strdup("");
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = ft_strdup("");
	new->next = NULL;
	return (new);
}

int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char	*join_key_value(t_env *env)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strjoin(env->key, "=");
	if (!tmp)
		return (NULL);
	joined = ft_strjoin(tmp, env->value);
	free(tmp);
	return (joined);
}
