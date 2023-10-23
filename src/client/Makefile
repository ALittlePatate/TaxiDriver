SRC =	main.cpp \
	memory.cpp \
	imgui/*.cpp
OBJ = $(SRC:.cpp=.o)
NAME = Revird 
INC = -Iimgui/
CFLAGS = -Wall -Wextra -Wpedantic -lSDL3 -lOpenGL

all:	$(NAME)

$(NAME):	$(OBJ)
	g++ $(SRC) $(CFLAGS) $(INC) -o $(NAME)
	mv $(NAME) ../../.

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : all $(NAME) clean fclean re
