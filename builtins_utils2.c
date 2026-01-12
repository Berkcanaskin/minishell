/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:06:48 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/27 23:06:51 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_overflow(unsigned long long num, int sign, int *error)
{
	if ((sign == 1 && num > LLONG_MAX)
		|| (sign == -1 && num > (unsigned long long)LLONG_MAX + 1))
	{
		*error = 1;
		return (1);
	}
	return (0);
}

static int	to_numeric_value(const char *str, int *error)
{
	unsigned long long	num;
	int					sign;
	int					i;

	num = 0;
	sign = 1;
	i = 0;
	*error = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && isdigit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		if (check_overflow(num, sign, error))
			break ;
		i++;
	}
	return ((int)(sign * num));
}

static int	handle_exit_args(t_shell *shell, t_cmd *cmd)
{
	int	error;
	int	exit_code;

	error = 0;
	if (!is_numeric(cmd->args[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
		exit(2);
	}
	if (cmd->args[2])
	{
		printf("minishell: exit: too many arguments\n");
		shell->exit_status = 1;
		return (1);
	}
	exit_code = to_numeric_value(cmd->args[1], &error);
	if (error)
	{
		printf("minishell: exit: %s: numeric argument required\n",
			cmd->args[1]);
		exit(2);
	}
	exit(exit_code & 255);
}

void	exec_exit(t_shell *shell, t_cmd *cmd)
{
	printf("exit\n");
	if (cmd->args[1])
	{
		if (handle_exit_args(shell, cmd))
			return ;
	}
	exit(shell->exit_status);
}
