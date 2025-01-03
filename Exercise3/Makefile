CC = g++
CFLAGS = -Wall -O3 --std=c++11

main3.2.exe : main3.2.o random.o
	$(CC) random.o main3.2.o -o main3.2.exe
main3.2.o : main3.2.cpp
	$(CC) -c main3.2.cpp -o main3.2.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o main3.2.exe seed.out
