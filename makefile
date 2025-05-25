# Compiler settings (Uncomment based on OS)
# For FreeBSD:
#CC = clang
#CFLAGS = -g -I/usr/src/usr.sbin/ppp
# For Linux:
CC = gcc
CFLAGS = -g

LDFLAGS = -lpcap
SRC = main.c
BIN = packet_sniffer

# Build process
all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

# Clean compiled binaries (Ensure tab indentation)
clean:
	$(RM) $(BIN)