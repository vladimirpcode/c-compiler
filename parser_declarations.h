#pragma once

#include "parser.h"

// ISO/IEC 9899:1999 (E) A.2.2 Declarations
void parse_declaration(Scanner& scan);
void parse_declaration_specifiers(Scanner& scan);
void parse_init_declarator_list(Scanner& scan);
void parse_init_declarator(Scanner& scan);
void parse_storage_class_specifier(Scanner& scan);
void parse_type_specifier(Scanner& scan);
void parse_struct_or_union_specifier(Scanner& scan);
void parse_struct_or_union(Scanner& scan);
void parse_struct_declaration_list(Scanner& scan);
void parse_struct_declaration(Scanner& scan);
void parse_specifier_qualifier_list(Scanner& scan);
void parse_struct_declarator_list(Scanner& scan);
void parse_struct_declarator(Scanner& scan);
void parse_enum_specifier(Scanner& scan);
void parse_enumerator_list(Scanner& scan);
void parse_enumerator(Scanner& scan);
void parse_type_qualifier(Scanner& scan);
void parse_function_specifier(Scanner& scan);
void parse_declartor(Scanner& scan);
void parse_direct_declartor(Scanner& scan);
void parse_pointer(Scanner& scan);
void parse_type_qualifier_list(Scanner& scan);
void parse_parameter_type_list(Scanner& scan);
void parse_parameter_list(Scanner& scan);
void parse_parameter_declaration(Scanner& scan);
void parse_identifier_list(Scanner& scan);
void parse_type_name(Scanner& scan);
void parse_abstract_declarator(Scanner& scan);
void parse_direct_abstract_declarator(Scanner& scan);
void parse_typedef_name(Scanner& scan);
void parse_initializer(Scanner& scan);
void parse_initializer_list(Scanner& scan);
void parse_designation(Scanner& scan);
void parse_designator_list(Scanner& scan);
void parse_designator(Scanner& scan);