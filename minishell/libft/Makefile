# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekoljone <ekoljone@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/02 14:31:17 by ekoljone          #+#    #+#              #
#    Updated: 2023/04/26 16:57:49 by ekoljone         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC = ft_isalnum.c ft_isprint.c ft_memcmp.c  ft_putchar_fd.c ft_split.c \
					ft_strlcat.c ft_strncmp.c ft_substr.c ft_atoi.c ft_isalpha.c \
					ft_itoa.c ft_memcpy.c  ft_putendl_fd.c ft_strchr.c  ft_strlcpy.c \
					ft_strnstr.c ft_tolower.c ft_bzero.c   ft_isascii.c ft_strtrim.c \
					ft_memmove.c ft_putnbr_fd.c  ft_strdup.c  ft_strlen.c  ft_strrchr.c \
					ft_toupper.c ft_calloc.c  ft_isdigit.c ft_memchr.c  ft_memset.c  \
					ft_putstr_fd.c ft_strjoin.c ft_strmapi.c ft_striteri.c get_next_line.c get_next_line_utils.c \
					ft_printf_utils.c ft_printf.c
OBJECT = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
CFLAGS = -Wall -Wextra -Werror -I.
CC = cc
RM = rm -f
OBJDIR = obj
SRCDIR = src

all: obj $(OBJDIR) $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECT)
	ar rcs $(NAME) $(OBJECT)
clean:
	rm -rf $(OBJDIR)
	$(RM) $(OBJECT)

fclean: clean
	$(RM) $(NAME)

re: fclean all