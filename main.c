/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:50:44 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:50:47 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env(envp);
	shell->input = NULL;
	shell->cmds = NULL;
	shell->exit_status = 0;
}

static void	minishell_loop(t_shell *shell)
{
	while (1)
	{
		shell->input = readline("HBSOT$ ");
		if (!shell->input)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*shell->input)
			add_history(shell->input);
		if (parse_input(shell) == 1)
		{
			expand_variables(shell);
			if (shell->cmds && shell->cmds->next)
				execute_pipeline(shell);
			else
				execute_commands(shell);
		}
		free_input(shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	setup_signal_handlers();
	init_shell(&shell, envp);
	minishell_loop(&shell);
	cleanup_shell(&shell);
	return (0);
}
