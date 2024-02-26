scan_wrapper.o: scan_wrapper.cpp scan_wrapper.h
	g++ scan_wrapper.cpp -c
main.o: main.cpp
	g++ main.cpp -c
scanner.o: scanner.cpp scanner.h
	g++ scanner.cpp -c
converter.o: converter.cpp converter.h
	g++ converter.cpp -c
parser.o: parser.cpp parser.h
	g++ parser.cpp -c

all: main.o scanner.o scan_wrapper.o converter.o parser.o
	g++ main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		parser.o \
		-o app
clean:
	rm  app \
		main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		parser.o \