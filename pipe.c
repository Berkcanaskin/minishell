/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:36 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:51:38 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_process(t_shell *sh, t_cmd *cmd, int prev, int *pipefd)
{
	setup_child_signals();
	if (prev != -1)
	{
		dup2(prev, STDIN_FILENO);
		close(prev);
	}
	if (cmd->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (cmd->heredoc && cmd->heredoc_file)
		if (redirect_heredoc_input(cmd->heredoc_file))
			exit(1);
	if (handle_redirections(cmd))
		exit(1);
	if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
		exit(0);
	if (is_builtin(cmd->args[0]))
		execute_builtin(sh, cmd);
	else
		execute_external(sh, cmd);
	exit(sh->exit_status);
}

static void	close_and_update(int *prev, int *pipefd, t_cmd *cmd)
{
	if (*prev != -1)
		close(*prev);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev = pipefd[0];
	}
}

static void	wait_last_status(pid_t last, t_shell *sh)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (WIFEXITED(status) && pid == last)
			sh->exit_status = WEXITSTATUS(status);
		pid = wait(&status);
	}
}

void	execute_pipeline(t_shell *sh)
{
	t_cmd	*cmd;
	int		prev_fd;
	int		pipefd[2];
	pid_t	pid;
	pid_t	last_pid;

	cmd = sh->cmds;
	prev_fd = -1;
	last_pid = -1;
	while (cmd)
	{
		if (cmd->next && pipe(pipefd) < 0)
		{
			perror("pipe");
			return ;
		}
		pid = fork();
		if (pid == 0)
			handle_child_process(sh, cmd, prev_fd, pipefd);
		close_and_update(&prev_fd, pipefd, cmd);
		last_pid = pid;
		cmd = cmd->next;
	}
	wait_last_status(last_pid, sh);
	cleanup_heredoc_files(sh->cmds);
}
