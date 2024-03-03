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
expression_parser.o: expression_parser.cpp expression_parser.h
	g++ expression_parser.cpp -c
name_table.o: name_table.cpp name_table.h
	g++ name_table.cpp -c

all: main.o scanner.o scan_wrapper.o converter.o parser.o expression_parser.o name_table.o
	g++ main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		name_table.o \
		parser.o \
		expression_parser.o \
		-o app
clean:
	rm  app \
		main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		parser.o \
		name_table.o \
		expression_parser.o \
