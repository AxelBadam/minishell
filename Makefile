NAME = minishell
SRC = minishell.c
LIBFT = libft
OBJECT = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -I/Users/ekoljone/.brew/opt/readline/include
#CFLAGS = -Wall -Wextra -Werror -I. -I/opt/homebrew/opt/readline/include
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
#$(CC) $(OBJECT) -L/opt/homebrew/opt/readline/lib -lreadline $(LIBFT)/libft.a -o $(NAME)
	$(CC) $(OBJECT) -L/Users/ekoljone/.brew/opt/readline/lib -lreadline $(LIBFT)/libft.a -o $(NAME)
	
clean:
	rm -rf $(OBJDIR)
	@make -s clean -C $(LIBFT)
	$(RM) $(OBJECT)

fclean: clean
	@make -s fclean -C $(LIBFT)
	$(RM) $(NAME)

re: fclean all