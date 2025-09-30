CXX= c++
FLAGSXX= -Wall -Wextra -Werror -std=c++98 -g
SRC = src/user.cpp src/commands.cpp src/utils.cpp src/network.cpp src/main.cpp src/command_utils.cpp src/channel.cpp
OBJ = $(SRCSRC:.cpp=.o)
INCLUDE = -Iinclude
NAME = irc

all:$(NAME)

$(NAME):
	$(CXX) $(FLAGSXX) $(INCLUDE) $(SRC) -o $(NAME)

%.o:%.cpp 
	$(CXX) $(FLAGSXX) $(INCLUDE) -c $< -o $@
clean:
	rm -fr $(OBJ)
fclean:clean
	rm -fr $(NAME)
re: fclean all
