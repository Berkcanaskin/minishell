/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:07:36 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:07:39 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_list_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

static void	fill_env_array(t_env **arr, t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
}

static void	sort_env_array(t_env **arr, int len)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - 1 - i)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_env(t_env *env)
{
	int		i;
	int		len;
	t_env	**arr;

	i = 0;
	len = env_list_size(env);
	arr = malloc(sizeof(t_env *) * len);
	if (!arr)
		return ;
	fill_env_array(arr, env);
	sort_env_array(arr, len);
	while (i < len)
	{
		if (arr[i]->value)
			printf("declare -x %s=\"%s\"\n",
				arr[i]->key, arr[i]->value);
		else
			printf("declare -x %s\n", arr[i]->key);
		i++;
	}
	free(arr);
}

int	key_len(const char *arg)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	return (i);
}
