CC=g++
INCLUDE= -I ./src
all :
	$(CC) demon.cpp -o demon $(INCLUDE)
	$(CC) test.cpp -o test $(INCLUDE)
	  
	