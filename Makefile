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
parser_expressions.o: parser_expressions.cpp parser_expressions.h
	g++ parser_expressions.cpp -c
parser_declarations.o: parser_declarations.cpp parser_declarations.h
	g++ parser_declarations.cpp -c
parser_statements.o: parser_statements.cpp parser_statements.h
	g++ parser_statements.cpp -c
parser_external_definitions.o: parser_external_definitions.cpp parser_external_definitions.h
	g++ parser_external_definitions.cpp -c
name_table.o: name_table.cpp name_table.h
	g++ name_table.cpp -c

all: main.o scanner.o scan_wrapper.o converter.o parser.o parser_expressions.o \
			parser_declarations.o parser_statements.o parser_external_definitions.o \
			name_table.o
	g++ main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		name_table.o \
		parser.o \
		parser_expressions.o \
		parser_declarations.o \
		parser_statements.o \
		parser_external_definitions.o \
		-o app
clean:
	rm  app \
		main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		parser.o \
		name_table.o \
		parser_expressions.o \
		parser_declarations.o \
		parser_statements.o \
		parser_external_definitions.o \
