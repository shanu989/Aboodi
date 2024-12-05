CC = gcc
CFLAGS = -Wall -Wextra -O3
OBJS = main.o PSO.o OF.o
TARGET = pso

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
