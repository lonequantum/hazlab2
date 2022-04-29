NAME = hazlab2
LIBS = -lm -ldl -lglfw -lGL
OTHER_FLAGS = -D_POSIX_C_SOURCE=200809L -Wall

$(NAME): clean
	c99 -o $(NAME) *.c $(OTHER_FLAGS) $(LIBS)

release:
	c99 -o $(NAME) *.c -O3 -DNDEBUG $(OTHER_FLAGS) $(LIBS)
	strip $(NAME)

clean:
	rm -f $(NAME)
