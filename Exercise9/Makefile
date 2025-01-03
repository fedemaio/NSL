CC = g++
CFLAGS = -Wall -O3 --std=c++17

main.exe : main.o random.o tour.o population.o
	$(CC) main.o random.o tour.o population.o -o main.exe

main.o : main.cpp population.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

tour.o : tour.cpp tour.h
	$(CC) -c tour.cpp -o tour.o $(CFLAGS)

population.o : population.cpp population.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)

clean :
	rm *.o main.exe seed.out