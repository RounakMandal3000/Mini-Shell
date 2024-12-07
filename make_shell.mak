CC = gcc
CFLAGS = -Wall -Iinclude   
LDFLAGS =

SRC = src/main.c src/shell_input.c src/delep.c src/default_comm.c src/exec_comm.c src/file_lock.c src/file_open.c -lreadline
OBJ = $(SRC:.c=.o)

TARGET = myshell

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
