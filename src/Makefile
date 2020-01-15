CC=g++
INCLUDE= -I ./src
all :
	rm testserver
	$(CC) demon.cpp -o demon $(INCLUDE) -pthread
	$(CC) test.cpp -o test $(INCLUDE)
	$(CC) test2.cpp -o test2 $(INCLUDE) 
	