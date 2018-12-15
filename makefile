Project1: main.o polygon.o line.o point.o utility.o
	g++ -g -std=c++11 -Wall -o Project main.o line.o point.o utility.o --lglut -lGL

main.o: main.cpp utility.h line.h polygon.h
	g++ -I ./glm -g -std=c++11 -Wall -c -o main.o main.cpp -lglut -lGL

polygon.o: polygon.cpp polygon.h utility.h line.h point.h
	g++ -I ./glm -g -std=c++11 -Wall -c -o polygon.o polygon.cpp -lglut -lGL

line.o: line.cpp line.h point.h utility.h polygon.h
	g++ -g -std=c++11 -Wall -c -o line.o line.cpp -lglut -lglut -lGL

point.o: point.cpp point.h utility.h 
	g++ -g -std=c++11 -Wall -c -o point.o point.cpp -lglut -lGL

clean:
	rm -f */*.o Project1
