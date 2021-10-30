NAME := snakeClient

D_SRCS := Srcs
D_INCS := Incs
D_OBJS := Objs

SRC		:= main.cpp Client.cpp Snake.cpp Game.cpp Utils.cpp

SRCS	:= $(addprefix $(D_SRCS)/, $(SRC))
OBJS	:= $(addprefix $(D_OBJS)/, $(SRC:cpp=o))

CC		:= g++
CFLAGS  := -g -std=c++11 # -fsanitize=address
RM		:= rm -rf

INC		:= -I$(D_INCS)/

LIBS	:= -lncurses

all: $(NAME)

$(NAME): $(D_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(D_OBJS):
	mkdir -p $(D_OBJS)

$(D_OBJS)/%.o: $(D_SRCS)/%.cpp
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	$(RM) $(D_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re