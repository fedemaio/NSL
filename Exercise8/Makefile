CC = g++
CFLAGS = -Wall -O3 --std=c++17

main.exe : main.o random.o groundstate_SA.o
	$(CC) random.o main.o groundstate_SA.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
groundstate_SA.o : groundstate_SA.cpp groundstate_SA.h
	$(CC) -c groundstate_SA.cpp -o groundstate_SA.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o main.exe seed.out