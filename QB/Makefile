CC = cc -std=c11
CFLAGS = -Wall -Werror

OBJFILES = main.o  utils.o handlers.o server.o 
TARGET = main

$(TARGET): $(OBJFILES) 
			$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 

main.o: main.c main.h
			$(CC) $(CFLAGS) -c main.c


utils.o: utils.c utils.h
			$(CC) $(CFLAGS) -c utils.c

handlers.o: handlers.c handlers.h
			$(CC) $(CFLAGS) -c handlers.c

server.o: server.c server.h
			$(CC) $(CFLAGS) -c server.c

clean:
	rm -f $(OBJFILES) $(TARGET)

run: $(TARGET)
	./$(TARGET)
