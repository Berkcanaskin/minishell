/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:08:22 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:08:24 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_set(t_env **env, char *key, char *val)
{
	t_env	*cur;
	t_env	*new;

	if (!key)
		return ;
	cur = *env;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			free(cur->value);
			if (val)
				cur->value = ft_strdup(val);
			else
				cur->value = ft_strdup("");
			return ;
		}
		cur = cur->next;
	}
	if (val)
		new = env_new(key, val);
	else
		new = env_new(key, "");
	new->next = *env;
	*env = new;
}

void	env_unset(t_env **env, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (!ft_strcmp(cur->key, key))
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
