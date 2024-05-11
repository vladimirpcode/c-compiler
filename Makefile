scan_wrapper.o: scan_wrapper.cpp scan_wrapper.h
	g++ scan_wrapper.cpp -c
main.o: main.cpp
	g++ main.cpp -c
scanner.o: scanner.cpp scanner.h
	g++ scanner.cpp -c
converter.o: converter.cpp converter.h
	g++ converter.cpp -c
parser/parser.o: parser/parser.cpp parser/parser.h
	g++ parser/parser.cpp -c -o parser/parser.o
parser/parser_expressions.o: parser/parser_expressions.cpp parser/parser_expressions.h
	g++ parser/parser_expressions.cpp -c -o parser/parser_expressions.o
parser/parser_declarations.o: parser/parser_declarations.cpp parser/parser_declarations.h
	g++ parser/parser_declarations.cpp -c -o parser/parser_declarations.o
parser/parser_statements.o: parser/parser_statements.cpp parser/parser_statements.h
	g++ parser/parser_statements.cpp -c -o parser/parser_statements.o
parser/parser_external_definitions.o: parser/parser_external_definitions.cpp parser/parser_external_definitions.h
	g++ parser/parser_external_definitions.cpp -c -o parser/parser_external_definitions.o
parser/try_parse.o: parser/try_parse.cpp parser/try_parse.h
	g++ parser/try_parse.cpp -c -o parser/try_parse.o
parser/parser_others.o: parser/parser_others.cpp parser/parser_others.h
	g++ parser/parser_others.cpp -c -o parser/parser_others.o
name_table.o: name_table.cpp name_table.h
	g++ name_table.cpp -c
ast.o: ast.h ast.cpp
	g++ ast.cpp -c

all: main.o scanner.o scan_wrapper.o converter.o parser/parser.o parser/parser_expressions.o \
			parser/parser_declarations.o parser/parser_statements.o parser/parser_external_definitions.o \
			name_table.o parser/try_parse.o parser/parser_others.o ast.o
	g++ main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		name_table.o \
		parser/parser.o \
		parser/parser_expressions.o \
		parser/parser_declarations.o \
		parser/parser_statements.o \
		parser/parser_external_definitions.o \
		parser/try_parse.o \
		parser/parser_others.o \
		ast.o \
		-o app
clean:
	rm  app \
		main.o \
		scanner.o \
		scan_wrapper.o \
		converter.o \
		parser/parser.o \
		name_table.o \
		parser/parser_expressions.o \
		parser/parser_declarations.o \
		parser/parser_statements.o \
		parser/parser_external_definitions.o \
		parser/try_parse.o \
		parser/parser_others.o \
		ast.o \

