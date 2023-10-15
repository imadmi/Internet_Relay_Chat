

NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address
# FLAGS = -fsanitize=address -std=c++98
CC = c++  
OBJ_DIR = obj
SRC_DIR = src
HEADER_DIR = headers

all: $(NAME)

SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp)

OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

HEADER = $(wildcard $(HEADER_DIR)/*.hpp)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(FLAGS) $(OBJ) -o $@
	@clear
	@echo ./$(NAME) "\033[0;31m6667\033[0m pass"
	@./$(NAME) 6667 pass

bonus:
	@c++ fileTransfer/filetransfer.cpp -o fileTransfer/bot
	@clear
	@echo ./fileTransfer/bot

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

.PHONY: all clean fclean re $(NAME)


