CC = cc -std=c11
CFLAGS = -Wall -Werror

OBJFILES = main.o utils.o handlers.o server.o 
TARGET = main
HEADERS = server.h

$(TARGET): $(OBJFILES) 
			$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) 

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJFILES) $(TARGET)

run: $(TARGET)
	./$(TARGET)
