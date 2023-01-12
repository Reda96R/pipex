NAME = pipex.out
FILES = ft_errors.c ft_free.c ft_processes.c ft_strlen.c pipex.c
OBJS = $(FILES:.c=.o)
CFLAGS = -Wall -Wextra -Werror
NONE='\033[0m'
GREEN='\033[32m'
GRAY='\033[2;37m'
CURSIVE='\033[3m'

all: $(NAME)

$(NAME): $(OBJS)
	@echo $(CURSIVE)$(GRAY) ":::Compiling $(NAME):::" $(NONE)
	@gcc $(CFLAGS) $(OBJS) -o $(NAME)
	@echo $(GREEN)":::Compiled:::"$(NONE)

$(OBJS): $(FILES)
	@echo $(CURSIVE)$(GRAY) ":::Making object files:::" $(NONE)
	@gcc $(CFLAGS) -c $(FILES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:all clean fclean re 