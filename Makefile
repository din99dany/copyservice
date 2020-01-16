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
	$(CC) ./tests/3jobtest.c -o 3jobtest spellcaster.o $(INCLUDE)
	$(CC) ./tests/4jobtest.c -o 4jobtest spellcaster.o $(INCLUDE)
	$(CC) ./tests/5jobtest.c -o 5jobtest spellcaster.o $(INCLUDE)
	$(CC) ./tests/6jobtest.c -o 6jobtest spellcaster.o $(INCLUDE)
	$(CC) ./tests/stresstest.c -o stresstest spellcaster.o $(INCLUDE)

spellcaster.o :
	$(CC) -c ./src/spellcaster.c $(INCLUDE)

clean :
	rm -f *.o
	rm -f testserver
	rm -f demon
	rm -f 1jobtest
	rm -f 2jobtest
	rm -f 3jobtest
	rm -f stresstest
	rm -f test2.txt
	rm -f test4.txt
	rm -f test6.txt
	rm -f test8.txt
	rm -f test10.txt
	rm -f test12.txt
	touch test2.txt
	touch test4.txt
	touch test6.txt
	touch test8.txt
	touch test10.txt
	touch test12.txt
