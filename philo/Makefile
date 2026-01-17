NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror #-fsanitize=address
OBJS = main.o utilities/setup_utils_1.o utilities/setup_utils_2.o utilities/miscellaneous_utils.o \
	utilities/cleaning_utils.o routines.o

.PHONY: all clean fclean re

all: $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
