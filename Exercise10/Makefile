CC = /usr/bin/mpic++
CFLAGS = -Wall -O3 --std=c++17

mainind.exe : mainind.o random.o tour.o population.o
	$(CC) mainind.o random.o tour.o population.o -o mainind.exe

mainind.o : mainind.cpp population.h
	$(CC) -c mainind.cpp -o mainind.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

tour.o : tour.cpp tour.h
	$(CC) -c tour.cpp -o tour.o $(CFLAGS)

population.o : population.cpp population.h
	$(CC) -c population.cpp -o population.o $(CFLAGS)

clean :
	rm *.o mainind.exe seed.out