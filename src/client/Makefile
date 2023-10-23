SRC =	main.cpp \
	memory.cpp
OBJ = $(SRC:.c=.o)
NAME = Revird 
CFLAGS = -Wall -Wextra -Wpedantic

all:	$(NAME)

$(NAME):	$(OBJ)
	g++ $(SRC) $(CFLAGS) -o $(NAME)
	mv $(NAME) ../../.

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : all $(NAME) clean fclean re
