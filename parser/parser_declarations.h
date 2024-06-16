#pragma once

#include "parser.h"

// ISO/IEC 9899:1999 (E) A.2.2 Declarations
void parse_declaration(Scanner& scan, AST*& ast, int depth);
void parse_declaration_specifiers(Scanner& scan, AST*& ast, int depth);
void parse_init_declarator_list(Scanner& scan, AST*& ast, int depth);
void parse_init_declarator(Scanner& scan, AST*& ast, int depth);
void parse_storage_class_specifier(Scanner& scan, AST*& ast, int depth);
void parse_type_specifier(Scanner& scan, AST*& ast, int depth);
void parse_struct_or_union_specifier(Scanner& scan, AST*& ast, int depth);
void parse_struct_or_union(Scanner& scan, AST*& ast, int depth);
void parse_struct_declaration_list(Scanner& scan, AST*& ast, int depth);
void parse_struct_declaration(Scanner& scan, AST*& ast, int depth);
void parse_specifier_qualifier_list(Scanner& scan, AST*& ast, int depth);
void parse_struct_declarator_list(Scanner& scan, AST*& ast, int depth);
void parse_struct_declarator(Scanner& scan, AST*& ast, int depth);
void parse_enum_specifier(Scanner& scan, AST*& ast, int depth);
void parse_enumerator_list(Scanner& scan, AST*& ast, int depth);
void parse_enumerator(Scanner& scan, AST*& ast, int depth);
void parse_type_qualifier(Scanner& scan, AST*& ast, int depth);
void parse_function_specifier(Scanner& scan, AST*& ast, int depth);
void parse_declarator(Scanner& scan, AST*& ast, int depth);
void parse_direct_declartor(Scanner& scan, AST*& ast, int depth);
void parse_pointer(Scanner& scan, AST*& ast, int depth);
void parse_type_qualifier_list(Scanner& scan, AST*& ast, int depth);
void parse_parameter_type_list(Scanner& scan, AST*& ast, int depth);
void parse_parameter_list(Scanner& scan, AST*& ast, int depth);
void parse_parameter_declaration(Scanner& scan, AST*& ast, int depth);
void parse_identifier_list(Scanner& scan, AST*& ast, int depth);
void parse_type_name(Scanner& scan, AST*& ast, int depth);
void parse_abstract_declarator(Scanner& scan, AST*& ast, int depth);
void parse_direct_abstract_declarator(Scanner& scan, AST*& ast, int depth);
void parse_typedef_name(Scanner& scan, AST*& ast, int depth);
void parse_initializer(Scanner& scan, AST*& ast, int depth);
void parse_initializer_list(Scanner& scan, AST*& ast, int depth);
void parse_designation(Scanner& scan, AST*& ast, int depth);
void parse_designator_list(Scanner& scan, AST*& ast, int depth);
void parse_designator(Scanner& scan, AST*& ast, int depth);