CC = gcc
CFLAGS = -g -Wall
LIBS = -lpcap

TARGET = packet_sniffer
SRC = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)