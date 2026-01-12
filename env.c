/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:08:55 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:08:56 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_env_var(t_env **env, t_env **last, char *env_str)
{
	char	*equal;
	t_env	*node;

	equal = ft_strchr(env_str, '=');
	if (!equal)
		return ;
	*equal = '\0';
	node = env_new(env_str, equal + 1);
	*equal = '=';
	if (!*env)
		*env = node;
	else
		(*last)->next = node;
	*last = node;
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*last;
	int		i;
	char	cwd[1024];
	char	*existing_pwd;

	env = NULL;
	last = NULL;
	i = 0;
	while (envp[i])
	{
		append_env_var(&env, &last, envp[i]);
		i++;
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		existing_pwd = get_env_value(env, "PWD");
		if (!existing_pwd || !*existing_pwd || access(existing_pwd, F_OK) != 0)
		{
			env_unset(&env, "PWD");
			env_set(&env, ft_strdup("PWD"), ft_strdup(cwd));
		}
	}
	return (env);
}

char	**env_to_array(t_env *env)
{
	char	**arr;
	int		count;
	int		i;

	count = env_count(env);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = join_key_value(env);
		if (!arr[i++])
			return (NULL);
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
