/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:23 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:51:25 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_cmd_struct(t_cmd **cmd)
{
	*cmd = malloc(sizeof(t_cmd));
	if (!*cmd)
		return (1);
	(*cmd)->infile = NULL;
	(*cmd)->outfile = NULL;
	(*cmd)->append = 0;
	(*cmd)->heredoc = 0;
	(*cmd)->heredoc_limiter = NULL;
	(*cmd)->heredoc_file = NULL;
	(*cmd)->next = NULL;
	(*cmd)->args = ft_calloc(256, sizeof(char *));
	(*cmd)->outfiles = ft_calloc(16, sizeof(char *));
	(*cmd)->append_flags = ft_calloc(16, sizeof(int));
	(*cmd)->out_count = 0;
	return (!(*cmd)->args || !(*cmd)->outfiles || !(*cmd)->append_flags);
}

static void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

static int	handle_heredoc(t_cmd *cmd, char **tokens, int *i, t_shell *shell)
{
	cmd->heredoc = 1;
	cmd->heredoc_limiter = ft_strdup(tokens[++(*i)]);
	if (read_heredoc(cmd->heredoc_limiter, &cmd->heredoc_file, shell))
	{
		printf("^C\n");
		if (cmd->heredoc_file)
			free(cmd->heredoc_file);
		if (cmd->heredoc_limiter)
			free(cmd->heredoc_limiter);
		free_cmd_fields(cmd);
		free(cmd);
		free_tokens(tokens);
		return (1);
	}
	return (0);
}

static void	process_token(t_cmd *cmd, char **tokens, int *i, int *j)
{
	if (ft_strcmp(tokens[*i], "<") == 0 && tokens[*i + 1])
		cmd->infile = ft_strdup(tokens[++(*i)]);
	else if (ft_strcmp(tokens[*i], ">") == 0 && tokens[*i + 1])
	{
		cmd->outfiles[cmd->out_count] = ft_strdup(tokens[++(*i)]);
		cmd->append_flags[cmd->out_count++] = 0;
	}
	else if (ft_strcmp(tokens[*i], ">>") == 0 && tokens[*i + 1])
	{
		cmd->outfiles[cmd->out_count] = ft_strdup(tokens[++(*i)]);
		cmd->append_flags[cmd->out_count++] = 1;
	}
	else if (ft_strcmp(tokens[*i], "<<") == 0 && tokens[*i + 1])
	{
		if (handle_heredoc(cmd, tokens, i, cmd->shell))
			*j = -1;
	}
	else
		cmd->args[(*j)++] = ft_strdup(tokens[*i]);
}

t_cmd	*create_cmd_from_segment(char *segment)
{
	char	**tokens;
	t_cmd	*cmd;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tokens = tokenize_with_quotes(segment);
	if (!tokens || init_cmd_struct(&cmd))
		return (NULL);
	while (tokens[i])
	{
		process_token(cmd, tokens, &i, &j);
		if (j == -1)
			return (NULL);
		i++;
	}
	cmd->args[j] = NULL;
	free_tokens(tokens);
	return (cmd);
}
