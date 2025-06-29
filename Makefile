NAME = miniRT

INC = inc
SRC = src
OBJS = objs
SCRIPTS = scripts

LIBFT = libft
LIBFTA = $(LIBFT)/libft.a
LIBFTI = $(LIBFT)/include

MLX_X11 = mlx/x11
MLX_X11A = $(MLX_X11)/libmlx.a
MLX_X11I = $(MLX_X11)
MLX_X11F = -lX11 -lXext

MLX_OPENGL = mlx/opengl
MLX_OPENGLA = $(MLX_OPENGL)/libmlx.a
MLX_OPENGLI = $(MLX_OPENGL)
MLX_OPENGLF = -framework OpenGL -framework AppKit -DGL_SILENCE_DEPRECATION

FILES =		destructor.c				\
		initializer.c				\
		main.c					\
		mlx/listeners/click.c			\
		mlx/listeners/expose.c			\
		mlx/listeners/press_key.c		\
		mlx/listeners/release_key.c		\
		mlx/mlx.c				\
		mlx/mlx_pixel_to_rgb.c			\
		objects/ambiant.c			\
		objects/camera/camera.c			\
		objects/camera/keys.c			\
		objects/cone/cone.c			\
		objects/cone/keys.c			\
		objects/cone/render.c			\
		objects/cone/intersect.c		\
		objects/cone/utils.c			\
		objects/cylinder/cylinder.c		\
		objects/cylinder/render.c		\
		objects/cylinder/intersect.c		\
		objects/cylinder/keys.c			\
		objects/cylinder/side.c			\
		objects/cylinder/utils.c		\
		objects/light/light.c			\
		objects/light/keys.c			\
		objects/light/utils.c			\
		objects/plane/keys.c			\
		objects/plane/plane.c			\
		objects/plane/render.c			\
		objects/plane/intersect.c		\
		objects/plane/utils.c			\
		objects/rotations.c			\
		objects/sphere/sphere.c			\
		objects/sphere/render.c			\
		objects/sphere/intersect.c		\
		objects/sphere/keys.c			\
		objects/sphere/utils.c			\
		objects/cap.c				\
		objects/complex.c			\
		objects/factory.c			\
		objects/types_factory.c			\
		objects/delete.c			\
		parsing/elements/default.c		\
		parsing/elements/extra.c		\
		parsing/parser.c			\
		parsing/utils.c				\
		render/blender.c			\
		render/lights.c				\
		render/render.c				\
		render/specular_reflection.c		\
		render/state.c				\
		render/tracer.c				\

ifeq ($(shell uname), Darwin)
FILES += mlx/opengl.c
else
FILES += mlx/x11.c
endif

OFILES = $(FILES:%.c=$(OBJS)/%.o)

FLAGS = -Wall -Wextra -Werror -O3
EXTRA_FLAGS = -g
COMPILATOR = cc

all: $(NAME)

$(NAME): $(LIBFTA) $(OFILES)
ifeq ($(shell uname), Darwin)
	make -C $(MLX_OPENGL) CFLAGS="-O3 -DSTRINGPUTX11 -w" > /dev/null
	$(COMPILATOR) $(FLAGS) $(OFILES) $(LIBFTA) $(MLX_OPENGLA) -o $(NAME) -I $(INC) -I $(LIBFTI) -I $(MLX_OPENGLI) $(EXTRA_FLAGS) $(MLX_OPENGLF)
else
	make -C $(MLX_X11) > /dev/null 2>&1
	$(COMPILATOR) $(FLAGS) $(OFILES) $(LIBFTA) $(MLX_X11A) -o $(NAME) -I $(INC) -I $(LIBFTI) -I $(MLX_X11I) $(EXTRA_FLAGS) $(MLX_X11F) -lm
endif

$(LIBFTA):
	make -C $(LIBFT) > /dev/null

clean:
	rm -rf $(OFILES)
	rm -rf $(OBJS)
	make -C $(LIBFT) clean > /dev/null
ifeq ($(shell uname), Darwin)
	make -C $(MLX_OPENGL) clean > /dev/null
else
	make -C $(MLX_X11) clean > /dev/null
endif

fclean: clean
	rm -rf $(NAME)
	make -C $(LIBFT) fclean > /dev/null

$(OBJS)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
ifeq ($(shell uname), Darwin)
	$(COMPILATOR) $(FLAGS) $< -c -o $@ -I $(INC) -I $(LIBFTI) -I $(MLX_OPENGLI) $(EXTRA_FLAGS)
else
	$(COMPILATOR) $(FLAGS) $< -c -o $@ -I $(INC) -I $(LIBFTI) -I $(MLX_X11I) $(EXTRA_FLAGS)
endif

submodules:
	git submodule update --remote --init --recursive

clean-branches:
	@echo "Fetching and pruning remote branches...";
	@git fetch --prune
	@for branch in $$(git branch --format '%(refname:short)' | grep -v '\*'); do \
		if ! git show-ref --quiet --verify refs/remotes/origin/$$branch; then \
			git branch -D $$branch; \
		fi \
	done
	@echo "✅ Cleanup complete";

norminette:
	@echo "Checking norminette..."
	@OUTPUT=$$(norminette $(SRC) $(INC) $(LIBFT)); \
	ERR_LINES=$$(echo "$$OUTPUT" | grep -c "Error:"); \
	if [ $$ERR_LINES -eq 0 ]; then \
		echo "\033[0;32m✅\033[0m Norminette: OK"; \
	else \
		echo "$$OUTPUT" | grep "Error"; \
		echo "\033[0;31m❌\033[0m Norminette: $$ERR_LINES error(s)"; \
	fi

check_parsing:
	bash $(SCRIPTS)/check_parsing.sh

re: fclean $(NAME)

.PHONY: all clean fclean re
