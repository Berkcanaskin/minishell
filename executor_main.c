/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:09:01 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:09:02 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_nonfork_builtin(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	return (!ft_strcmp(cmd->args[0], "cd")
		|| !ft_strcmp(cmd->args[0], "exit")
		|| !ft_strcmp(cmd->args[0], "export")
		|| !ft_strcmp(cmd->args[0], "unset"));
}

static void	run_forked_command(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (is_builtin(cmd->args[0]))
			execute_builtin(shell, cmd);
		else
			execute_external(shell, cmd);
		exit(shell->exit_status);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
	}
}

void	execute_commands(t_shell *shell)
{
	t_cmd	*cmd;
	int		stdin_copy;
	int		stdout_copy;

	cmd = shell->cmds;
	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (!cmd || !cmd->args || !cmd->args[0] || !*cmd->args[0])
		return ((void)(shell->exit_status = 0));
	if (cmd->heredoc && cmd->heredoc_file
		&& redirect_heredoc_input(cmd->heredoc_file))
		return ((void)(shell->exit_status = 1));
	if (handle_redirections(cmd))
		return ((void)(shell->exit_status = 1));
	if (is_nonfork_builtin(cmd))
		execute_builtin(shell, cmd);
	else
		run_forked_command(shell, cmd);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	cleanup_heredoc_files(shell->cmds);
}
