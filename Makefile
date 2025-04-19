NAME = miniRT

INC = inc
SRC = src
OBJS = objs

LIBFT = libft
LIBFTA = $(LIBFT)/libft.a
LIBFTI = $(LIBFT)

MLX_X11 = mlx/x11
MLX_X11A = $(MLX_X11)/libmlx.a
MLX_X11I = $(MLX_X11)
MLX_OPENGL = mlx/opengl
MLX_OPENGLA = $(MLX_OPENGL)/libmlx.a
MLX_OPENGLI = $(MLX_OPENGL)

FILES =	main.c									\

OFILES = $(FILES:%.c=$(OBJS)/%.o)

FLAGS = -Wall -Wextra -Werror
EXTRA_FLAGS = -g
COMPILATOR = cc

TYPE ?= linux

all: $(NAME)

$(NAME): $(LIBFTA) $(OFILES)
ifeq ($(TYPE),mac)
	make -C $(MLX_OPENGL) > /dev/null
	$(COMPILATOR) $(FLAGS) $(OFILES) $(LIBFTA) $(MLX_OPENGLA) -o $(NAME) -I $(INC) -I $(LIBFTI) -I $(MLX_OPENGLI) $(EXTRA_FLAGS)
else
	make -C $(MLX_X11) > /dev/null
	$(COMPILATOR) $(FLAGS) $(OFILES) $(LIBFTA) $(MLX_X11A) -o $(NAME) -I $(INC) -I $(LIBFTI) -I $(MLX_X11I) $(EXTRA_FLAGS)
endif

$(LIBFTA):
	make -C $(LIBFT) bonus > /dev/null

clean:
	rm -rf $(OFILES)
	rm -rf $(OBJS)
	make -C $(LIBFT) clean > /dev/null
	make -C $(MLX_X11) clean > /dev/null
	make -C $(MLX_OPENGL) clean > /dev/null

fclean: clean
	rm -rf $(NAME)
	make -C $(LIBFT) fclean > /dev/null

$(OBJS)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
ifeq ($(TYPE),mac)
	$(COMPILATOR) $(FLAGS) $< -c -o $@ -I $(INC) -I $(LIBFTI) -I $(MLX_OPENGLI) $(EXTRA_FLAGS)
else
	$(COMPILATOR) $(FLAGS) $< -c -o $@ -I $(INC) -I $(LIBFTI) -I $(MLX_X11I) $(EXTRA_FLAGS)
endif

re: fclean $(NAME)

.PHONY: all clean fclean re
