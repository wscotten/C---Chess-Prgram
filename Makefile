CC     = gcc
CFLAGS = -ansi -Wall -c
LFLAGS = -Wall

DESIGN = Chessbot2k17 Chessbot2k17bin
OBJS = Chessbot2k17.o ScreenDisplay.o Move.o AI.o 
DEPS = ScreenDisplay.h Move.h AI.h 

all: $(DESIGN)

Chessbot2k17: $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) -o Chessbot2k17

Chessbot2k17bin: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o bin/Chessbot2k17


#################### generate object files ###################

Chessbot2k17.o: Chessbot2k17.c $(DEPS) 
	$(CC) $(CFLAGS) Chessbot2k17.c -o Chessbot2k17.o

ScreenDisplay.o: ScreenDisplay.c ScreenDisplay.h
	$(CC) $(CFLAGS) ScreenDisplay.c -o ScreenDisplay.o
 
Move.o: Move.c Move.h
	$(CC) $(CFLAGS) Move.c -o Move.o 
 
AI.o: AI.c AI.h 
	$(CC) $(CFLAGS) AI.c -o AI.o 

clean: 
	rm -f *.o $(DESIGN) log.txt 

