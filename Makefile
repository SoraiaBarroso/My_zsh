CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = my_zsh.c my_zsh.h helper_fun.c commands.c tokenization.c execution.c
TARGET = my_zsh

all : $(TARGET)

$(TARGET) : $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)

re: fclean all