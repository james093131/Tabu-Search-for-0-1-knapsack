all:
	g++ -Wall -O3 -o TS main.cpp

clean:
	rm -f main *.o