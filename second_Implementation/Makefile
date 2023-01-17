NAME = pipex
FILES = main.c ft_errors.c ft_free.c ft_processes.c ft_strlen.c ft_cmdfinder.c ft_split.c ft_strjoin.c ft_strncmp.c
OBJS = $(FILES:.c=.o)
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g


all: $(NAME)

$(NAME): $(OBJS)
	@echo $(CURSIVE)$(GRAY) ":::Compiling $(NAME):::" $(NONE)
	@gcc $(CFLAGS) $(OBJS) -o $(NAME)
	@echo $(GREEN)":::Compiled:::"$(NONE)

$(OBJS): $(FILES)
	@echo $(CURSIVE)$(GRAY) ":::Making object files:::" $(NONE)
	@gcc $(CFLAGS) -c $(FILES)

clean:
	@echo $(CURSIVE)$(GRAY) ":::Deleting object files:::" $(NONE)
	@rm -f $(OBJS)
	@echo $(RED)":::Deleted:::"$(NONE)


fclean: clean
	@echo $(CURSIVE)$(GRAY) ":::Deleting $(NAME):::" $(NONE)
	@rm -f $(NAME)
	@echo $(RED)":::All deleted:::"$(NONE)

re: fclean all

.PHONY:all clean fclean re 

NONE='\033[0m'
GREEN='\033[32m'
GRAY='\033[2;37m'
RED = '\033[1;31m'
YELLOW = '\033[1;33m'
CURSIVE='\033[3m'