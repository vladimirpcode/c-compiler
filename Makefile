scan_wrapper.o: scan_wrapper.cpp scan_wrapper.h
	g++ scan_wrapper.cpp -c
main.o: main.cpp
	g++ main.cpp -c
scanner.o: scanner.cpp scanner.h
	g++ scanner.cpp -c

all: main.o scanner.o scan_wrapper.o
	g++ main.o \
		scanner.o \
		scan_wrapper.o \
		-o app
clean:
	rm  main.o \
		scanner.o \
		scan_wrapper.o \