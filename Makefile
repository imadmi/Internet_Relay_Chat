

NAME = ircserv
# FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
CC = c++  
OBJ_DIR = obj
SRC_DIR = src
HEADER_DIR = headers

all: $(NAME)

SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp)

OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

HEADER = $(wildcard $(HEADER_DIR)/*.h)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(FLAGS) $(OBJ) -o $@
	# @clear
	@./ircserv 5550 pass

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER)
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $@


clean:
	@ rm -rf $(OBJ_DIR)
	@ clear
	@echo "\033[0;93mObject files removed.\033[0m"

fclean: clean
	@rm -f $(NAME)
	@ clear
	@echo "\033[0;93mThe object files and the executable are removed.\033[0m"

re: fclean $(NAME)
	@./ircserv 5550 pass



.PHONY: all clean fclean re


