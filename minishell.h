/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:51:00 by vbicer            #+#    #+#             */
/*   Updated: 2025/06/28 00:32:07 by vbicer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shell	t_shell;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_cmd
{
	char				**args;
	char				*infile;
	char				*outfile;
	int					append;
	int					heredoc;
	char				*heredoc_limiter;
	char				*heredoc_file;
	char				**outfiles;
	int					*append_flags;
	int					out_count;
	t_shell				*shell;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_shell
{
	int					last_exit_code;
	char				*input;
	t_env				*env;
	t_cmd				*cmds;
	int					exit_status;
}						t_shell;

void					cleanup_shell(t_shell *shell);
void					free_input(t_shell *shell);
void					setup_signal_handlers(void);
void					setup_child_signals(void);
int						parse_input(t_shell *shell);
char					*remove_quotes(const char *str);
t_cmd					*create_cmd_from_segment(char *segment);
int						is_syntax_error(const char *input);
char					**tokenize_with_quotes(const char *str);
char					**split_pipe_segments(char *input);
void					execute_commands(t_shell *shell);
int						is_builtin(char *cmd);
void					execute_builtin(t_shell *shell, t_cmd *cmd);
void					execute_external(t_shell *shell, t_cmd *cmd);
void					execute_pipeline(t_shell *shell);
void					expand_variables(t_shell *shell);
char					*expand_loop(char *str, t_shell *sh, int i, char q);
int						append_var(char **res, const char *str, int i,
							t_shell *sh);
void					append_str(char **dest, const char *src);
int						handle_dollar_expansion(char **res, char *str, int i,
							t_shell *sh);
void					get_var_value_and_append(char **res, char *var,
							t_shell *sh);
void					append_char(char **res, char c);
void					exec_echo(t_cmd *cmd);
void					exec_cd(t_shell *shell, t_cmd *cmd);
void					exec_pwd(void);
void					exec_export(t_shell *shell, t_cmd *cmd);
void					exec_unset(t_shell *shell, t_cmd *cmd);
void					exec_env(t_shell *shell);
void					exec_exit(t_shell *shell, t_cmd *cmd);
int						is_valid_identifier(const char *str);
char					*get_value(char *str);
char					*get_value_part(const char *arg);
int						key_len(const char *arg);
void					print_sorted_env(t_env *env);
int						ft_strcmp(const char *s1, const char *s2);
t_env					*init_env(char **envp);
void					env_set(t_env **env, char *key, char *val);
void					env_unset(t_env **env, char *key);
char					**env_to_array(t_env *env);
int						env_count(t_env *env);
char					*join_key_value(t_env *env);
t_env					*env_new(char *key, char *value);
void					free_env(t_env *env);
char					*get_env_value(t_env *env, const char *key);
char					*get_env(t_env *env, const char *key);
int						handle_redirections(t_cmd *cmd);
int						read_heredoc(char *limiter, char **filename,
							t_shell *shell);
int						redirect_heredoc_input(char *filename);
void					cleanup_heredoc_files(t_cmd *cmds);
void					child_heredoc(char *limiter, char *filename,
							t_shell *shell);
char					*generate_tmp_filename(void);
char					*expand_string(char *str, t_shell *sh);
char					*expand_var(const char *name, t_env *env);
int						is_fully_single_quoted(const char *s);
char					*find_executable(char *cmd, t_env *env);
void					free_cmd_fields(t_cmd *cmd);

#endif
