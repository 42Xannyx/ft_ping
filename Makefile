NAME := ft_ping
CC := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic 
LDFLAGS := -lm

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "$(NAME) compiled successfully"

bonus: CFLAGS += -DBONUS=1
bonus: all

clean:
	rm -rf $(OBJ_DIR)
	@echo "Object files cleaned"

fclean: clean
	rm -f $(NAME)
	@echo "$(NAME) removed"

re: fclean all

debug: CFLAGS += -g3 -fsanitize=address
debug: LDFLAGS += -fsanitize=address
debug: all


.PHONY: all clean fclean re install debug bonus
