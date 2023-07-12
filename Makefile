NAME = minishell
SRC = main.c exec.c builtin.c builtin_2.c builtin_utils.c builtin_utils2.c utils.c check.c check_2.c env.c expand.c free.c heredoc.c make_list.c \
open_redirects.c remove_quotes.c split_by_operator.c split_command.c utils_2.c utils_3.c pipes.c signal.c split_by_operator_utils.c expand_utils.c split_pipes.c
LIBFT = libft
OBJECT = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -g -I/$(HOME)/.brew/opt/readline/include
RM = rm -f
OBJDIR = obj
SRCDIR = src

all: obj $(OBJDIR) $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECT)
	@make -s -C $(LIBFT)
	$(CC) $(OBJECT) -L/$(HOME)/.brew/opt/readline/lib -lreadline $(LIBFT)/libft.a -o $(NAME)
	
clean:
	rm -rf $(OBJDIR)
	@make -s clean -C $(LIBFT)
	$(RM) $(OBJECT)

fclean: clean
	@make -s fclean -C $(LIBFT)
	$(RM) $(NAME)

re: fclean all