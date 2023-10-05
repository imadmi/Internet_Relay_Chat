

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
INC = irc_header.hpp
NAME = ircserv
CC = c++

all: $(NAME)

$(NAME): $(OBJ) $(INC)
	@ $(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@ clear
	@echo "\033[0;92m./ircserv :\033[0m"

clean:
	@ rm -f $(OBJ)
	@ clear
	@echo "\033[0;93mObject files removed.\033[0m"

fclean: clean
	@rm -f $(NAME)
	@ clear
	@echo "\033[0;93mThe object files and the executable are removed.\033[0m"

re: fclean $(NAME)
	# @./ircserv 6666 pass

%.o: %.cpp $(INC)
	@$(CC) -c $< -o $@

.PHONY: all clean fclean re