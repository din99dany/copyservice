CC = gcc
INCLUDE = -I ./include
FLAGS = -lrt -pthread

all : demon tests
	echo done

demon : demon.o queue.o
	$(CC) -o demon demon.o queue.o $(FLAGS)

demon.o : queue.o  
	$(CC) -c ./src/demon.c $(INCLUDE)

queue.o :
	$(CC) -c ./src/queue.c $(INCLUDE)

tests : spellcaster.o
	$(CC) ./tests/1jobtest.c -o 1jobtest spellcaster.o $(INCLUDE)
	$(CC) ./tests/2jobtest.c -o 2jobtest spellcaster.o $(INCLUDE)
	$(CC) ./tests/stresstest.c -o stresstest spellcaster.o $(INCLUDE)

spellcaster.o :
	$(CC) -c ./src/spellcaster.c $(INCLUDE)

clean :
	rm -f *.o
	rm -f testserver
	rm -f demon
	rm -f 1jobtest
	rm -f 2jobtest
	rm -f stresstest
	
