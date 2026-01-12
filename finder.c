/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:10:40 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:47:10 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

static char	**get_path_array(t_env *env)
{
	char	*val;
	char	**arr;

	val = expand_var("PATH", env);
	if (!val)
		return (NULL);
	arr = ft_split(val, ':');
	free(val);
	return (arr);
}

static void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*find_executable(char *cmd, t_env *env)
{
	char	**paths;
	char	*full;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	paths = get_path_array(env);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (access(full, X_OK) == 0)
			return (free_paths(paths), full);
		free(full);
		i++;
	}
	return (free_paths(paths), NULL);
}
