mycalc: main.o El.o Expression.o
	g++  El.o Expression.o main.o -o mycalc

main.o: main.cpp
	g++ -c main.cpp

El.o: El.cpp El.h
	g++ -c El.cpp 

Expression.o: Expression.cpp Expression.h
	g++ -c Expression.cpp 

clean:
	rm *.o mycalc