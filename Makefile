# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vbicer <vbicer@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/27 23:50:54 by vbicer            #+#    #+#              #
#    Updated: 2025/06/28 00:26:28 by vbicer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBS = -L./libft -lft
READLINE = -lreadline

SRC_FILES = main.c \
	signals.c \
	utils.c \
	utils2.c \
	parser.c \
	executor.c \
	redirect.c \
	heredoc.c \
	heredoc2.c \
	builtins_utils.c \
	expand_variables.c \
	expand_string.c \
	expand_string2.c \
	executor_main.c \
	builtins_utils2.c \
	builtins_utils3.c \
	builtins_utils4.c \
	builtins_utils5.c \
	pipe.c \
	builtins.c \
	env.c \
	expansion.c \
	finder.c \
	env_utils.c \
	env_utils2.c \
	parser_utils.c \
	parser_utils2.c \
	parser_utils3.c

OBJS = $(SRC_FILES:.c=.o)

all: libs $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(READLINE) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I./libft -c $< -o $@

libs:
	$(MAKE) -C ./libft

clean:
	rm -f $(OBJS)
	$(MAKE) clean -C ./libft

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C ./libft

re: fclean all

.PHONY: all clean fclean re libs
