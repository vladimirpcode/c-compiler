#include "parser_declarations.h"

#include "try_parse.h"
#include "parser_expressions.h"
#include "parser_others.h"
#include <iostream>
#include "parser.h"
#include "../name_table.h"

void parse_declaration(Scanner& scan){
    DEBUG_PRINT("parse_declaration\n");
    parse_declaration_specifiers(scan);
    bool parsed_init_declarator_list = try_parse(parse_init_declarator_list, scan);
    if (parsed_init_declarator_list){
    }
    check(scan, Lex::SemiColon);
}

void parse_declaration_specifiers(Scanner& scan){
    DEBUG_PRINT("parse_declaration_specifiers\n");
    if (try_parse(parse_storage_class_specifier, scan)){
        
    } else if (try_parse(parse_type_specifier, scan)){

    } else if (try_parse(parse_type_qualifier, scan)){
        
    } else if (try_parse(parse_function_specifier, scan)){

    } else {
        throw ExpectedLexException("declaration specifiers", scan.get_line_for_compiler_msg());
    }
    while (scan.current_state.lex != Lex::EOT){
        if (try_parse(parse_storage_class_specifier, scan)){
        
        } else if (try_parse(parse_type_specifier, scan)){

        } else if (try_parse(parse_type_qualifier, scan)){
            
        } else if (try_parse(parse_function_specifier, scan)){

        } else {
            break;
        }
    }
}
void parse_init_declarator_list(Scanner& scan){
    DEBUG_PRINT("parse_init_declarator_list\n");
    parse_init_declarator(scan);
    if (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_init_declarator(scan);
    }
}

void parse_init_declarator(Scanner& scan){
    DEBUG_PRINT("parse_init_declarator\n");
    parse_declarator(scan);
    if (scan.current_state.lex == Lex::Assigment){
        scan.get_next();
        parse_initializer(scan);
    }
}

void parse_storage_class_specifier(Scanner& scan){
    DEBUG_PRINT("parse_storage_class_specifier\n");
    if (scan.current_state.lex == Lex::Typedef
        || scan.current_state.lex == Lex::Extern
        || scan.current_state.lex == Lex::Static
        || scan.current_state.lex == Lex::Auto
        || scan.current_state.lex == Lex::Register)
    {
        scan.get_next();
    } else {
        throw ExpectedLexException("storage class specifier", scan.get_line_for_compiler_msg());
    }
}

void parse_type_specifier(Scanner& scan){
    DEBUG_PRINT("parse_type_specifier\n");
    if (scan.current_state.lex == Lex::Void
        || scan.current_state.lex == Lex::Char
        || scan.current_state.lex == Lex::Short
        || scan.current_state.lex == Lex::Int
        || scan.current_state.lex == Lex::Long
        || scan.current_state.lex == Lex::Float
        || scan.current_state.lex == Lex::Double
        || scan.current_state.lex == Lex::Signed
        || scan.current_state.lex == Lex::Unsigned
        || scan.current_state.lex == Lex::Bool_
        || scan.current_state.lex == Lex::Complex_
        || scan.current_state.lex == Lex::Imaginary_)
    {
        scan.get_next();
    } else if (try_parse(parse_struct_or_union_specifier, scan)){
        
    } else if (try_parse(parse_enum_specifier, scan)){
        
    } else {
        parse_typedef_name(scan);
    }
}

void parse_struct_or_union_specifier(Scanner& scan){
    DEBUG_PRINT("parse_struct_or_union_specifier\n");
    parse_struct_or_union(scan);
    bool identifier_parsed = try_parse(parse_identifier, scan);
    if (!identifier_parsed && scan.current_state.lex != Lex::LCurlyBrace){
        throw ExpectedLexException("{ struct-declaration-list }", scan.get_line_for_compiler_msg());
    } else {
        scan.save_state();
        //ToDo nametable
        if (scan.current_state.lex == Lex::LCurlyBrace){
            check(scan, Lex::LCurlyBrace);
            parse_struct_declaration_list(scan);
            check(scan, Lex::RCurlyBrace);
        }
    }
}

void parse_struct_or_union(Scanner& scan){
    DEBUG_PRINT("parse_struct_or_union\n");
    if (scan.current_state.lex == Lex::Struct
        || scan.current_state.lex == Lex::Union)
    {
        scan.get_next();
    } else {
        throw ExpectedLexException("struct | union", scan.get_line_for_compiler_msg());
    }
}

void parse_struct_declaration_list(Scanner& scan){
    DEBUG_PRINT("parse_struct_declaration_list\n");
    parse_struct_declaration(scan);
    if (scan.current_state.lex == Lex::RCurlyBrace){
        return;
    }
    while (try_parse(parse_struct_declaration, scan)){
        if (scan.current_state.lex == Lex::RCurlyBrace){
            return;
        }
    }
}

void parse_struct_declaration(Scanner& scan){
    DEBUG_PRINT("parse_struct_declaration\n");
    parse_specifier_qualifier_list(scan);
    parse_struct_declarator_list(scan);
    check(scan, Lex::SemiColon);
}

void parse_specifier_qualifier_list(Scanner& scan){
    DEBUG_PRINT("parse_specifier_qualifier_list\n");
    if (try_parse(parse_type_specifier, scan)){
        try_parse(parse_specifier_qualifier_list, scan);
    } else if (try_parse(parse_type_qualifier, scan)){
        try_parse(parse_specifier_qualifier_list, scan);
    } else {
        throw ExpectedLexException("specifier qualifier list", scan.get_line_for_compiler_msg());
    }
}

void parse_struct_declarator_list(Scanner& scan){
    DEBUG_PRINT("parse_struct_declarator_list\n");
    parse_struct_declarator(scan);
    if (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_struct_declarator(scan);
    }
}

void parse_struct_declarator(Scanner& scan){
    DEBUG_PRINT("parse_struct_declarator\n");
    bool parsed_declarator = try_parse(parse_declarator, scan);
    if (!parsed_declarator && scan.current_state.lex != Lex::Colon){
        throw ExpectedLexException("struct declarator", scan.get_line_for_compiler_msg());
    }
    if (scan.current_state.lex == Lex::Colon){
        scan.get_next();
        parse_constant_expression(scan);
    }
}

void parse_enum_specifier(Scanner& scan){
    DEBUG_PRINT("parse_enum_specifier\n");
    check(scan, Lex::Enum);
    bool identifier_parsed = try_parse(parse_identifier, scan);
    if (!identifier_parsed && scan.current_state.lex != Lex::LCurlyBrace){
        throw ExpectedLexException("enum specifier", scan.get_line_for_compiler_msg());
    }
    if (scan.current_state.lex == Lex::LCurlyBrace){
        scan.get_next();
        parse_enumerator_list(scan);
        if (scan.current_state.lex == Lex::Comma){
            scan.get_next();
        }
        check(scan, Lex::RCurlyBrace);
    }
}

void parse_enumerator_list(Scanner& scan){
    DEBUG_PRINT("parse_enumerator_list\n");
    parse_enumerator(scan);
    while (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_enumerator(scan);
    }
}

void parse_enumerator(Scanner& scan){
    DEBUG_PRINT("parse_enumerator\n");
    parse_enumeration_constant(scan);
    if (scan.current_state.lex == Lex::Assigment){
        scan.get_next();
        parse_constant_expression(scan);
    }
}

void parse_type_qualifier(Scanner& scan){
    DEBUG_PRINT("parse_type_qualifier\n");
    if (scan.current_state.lex == Lex::Const
        || scan.current_state.lex == Lex::Restrict
        || scan.current_state.lex == Lex::Volatile)
    {
        scan.get_next();
    } else {
        throw ExpectedLexException("type qualifier", scan.get_line_for_compiler_msg());
    }
}

void parse_function_specifier(Scanner& scan){
    DEBUG_PRINT("parse_function_specifier\n");
    if (scan.current_state.lex == Lex::Inline)
    {
        scan.get_next();
    } else {
        throw ExpectedLexException("function specifier", scan.get_line_for_compiler_msg());
    }
}

void parse_declarator(Scanner& scan){
    DEBUG_PRINT("parse_declarator\n");
    try_parse(parse_pointer, scan);
    parse_direct_declartor(scan);
}

void parse_direct_declartor(Scanner& scan){
    DEBUG_PRINT("parse_direct_declartor\n");
    if (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        parse_declarator(scan);
        check(scan, Lex::RBrace);
    } else {
        parse_identifier(scan);
    }
    while (scan.current_state.lex == Lex::LBrace || scan.current_state.lex == Lex::LSquareBrace){
        if (scan.current_state.lex == Lex::LBrace){
            scan.get_next();
            if (try_parse(parse_parameter_list, scan)){

            } else {
                try_parse(parse_identifier_list, scan);
            }
            check(scan, Lex::RBrace);
        } else {
            scan.get_next();
            if (scan.current_state.lex == Lex::Static){
                scan.get_next();
                try_parse(parse_type_qualifier_list, scan);
                parse_assignment_expression(scan);
            } else {
                bool type_qualifier_list_parsed = try_parse(parse_type_qualifier_list, scan);
                if (type_qualifier_list_parsed && scan.current_state.lex == Lex::Static){
                    scan.get_next();
                    parse_assignment_expression(scan);
                } else if (scan.current_state.lex == Lex::Asterisk){
                    scan.get_next();
                } else {
                    try_parse(parse_assignment_expression, scan);
                }
            }
            check(scan, Lex::RSquareBrace);
        }
    }
}

void parse_pointer(Scanner& scan){
    DEBUG_PRINT("parse_pointer\n");
    check(scan, Lex::Asterisk);
    try_parse(parse_type_qualifier_list, scan);
    while (scan.current_state.lex == Lex::Asterisk){
        scan.get_next();
        try_parse(parse_type_qualifier_list, scan);
    }
}

void parse_type_qualifier_list(Scanner& scan){
    DEBUG_PRINT("parse_type_qualifier_list\n");
    parse_type_qualifier(scan);
    while (try_parse(parse_type_qualifier, scan)){

    }
}

void parse_parameter_type_list(Scanner& scan){
    DEBUG_PRINT("parse_parameter_type_list\n");
    parse_parameter_list(scan);
    if (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        check(scan, Lex::TripleDot);
    }
}

void parse_parameter_list(Scanner& scan){
    DEBUG_PRINT("parse_parameter_list\n");
    parse_parameter_declaration(scan);
    while (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_parameter_declaration(scan);
    }
}

void parse_parameter_declaration(Scanner& scan){
    DEBUG_PRINT("parse_parameter_declaration\n");
    parse_declaration_specifiers(scan);
    if (try_parse(parse_declarator, scan)){
        return;
    }
    try_parse(parse_abstract_declarator, scan);
}

void parse_identifier_list(Scanner& scan){
    DEBUG_PRINT("parse_identifier_list\n");
    parse_identifier(scan);
    while (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_identifier(scan);
    }
}

void parse_type_name(Scanner& scan){
    DEBUG_PRINT("parse_type_name\n");
    parse_specifier_qualifier_list(scan);
    try_parse(parse_abstract_declarator, scan);
}

void parse_abstract_declarator(Scanner& scan){
    DEBUG_PRINT("parse_abstract_declarator\n");
    bool parsed_pointer = try_parse(parse_pointer, scan);
    if (!parsed_pointer){
        parse_direct_abstract_declarator(scan);
    } else {
        try_parse(parse_direct_abstract_declarator, scan);
    }

}

void parse_direct_abstract_declarator(Scanner& scan){
    DEBUG_PRINT("parse_direct_abstract_declarator\n");
    check(scan, Lex::LBrace);
    parse_abstract_declarator(scan);
    check(scan, Lex::RBrace);
    bool aborted = false;
    while (scan.current_state.lex == Lex::LBrace){
        scan.get_next();
        if (!try_parse(parse_abstract_declarator, scan)){
            aborted = true;
            break;
        }
        check(scan, Lex::RBrace);
    }
    if (aborted){
        try_parse(parse_parameter_type_list, scan);
        check(scan, Lex::RBrace);
    }
    while (scan.current_state.lex == Lex::LBrace || scan.current_state.lex == Lex::LSquareBrace){
        if (scan.current_state.lex == Lex::LBrace){
            scan.get_next();
            try_parse(parse_parameter_type_list, scan);
            check(scan, Lex::RBrace);
        } else {
            scan.get_next();
            if (scan.current_state.lex == Lex::Asterisk){
                scan.get_next();
            } else {
                try_parse(parse_assignment_expression, scan);
            }
            check(scan, Lex::RSquareBrace);
        }
    }
}

void parse_typedef_name(Scanner& scan){
    DEBUG_PRINT("parse_typedef_name\n");
    check(scan, Lex::UserDefinedType);
}

void parse_initializer(Scanner& scan){
    DEBUG_PRINT("parse_initializer\n");
    if (scan.current_state.lex == Lex::LCurlyBrace){
        scan.get_next();
        parse_initializer_list(scan);
        if (scan.current_state.lex == Lex::Comma){
            scan.get_next();
        }
        check(scan, Lex::RCurlyBrace);
    } else {
        parse_assignment_expression(scan);
    }
}

void parse_initializer_list(Scanner& scan){
    DEBUG_PRINT("parse_initializer_list\n");
    try_parse(parse_designation, scan);
    parse_initializer(scan);
    while (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        try_parse(parse_designation, scan);
        parse_initializer(scan);
    }
}

void parse_designation(Scanner& scan){
    DEBUG_PRINT("parse_designation\n");
    parse_designator_list(scan);
    check(scan, Lex::Assigment);
}

void parse_designator_list(Scanner& scan){
    DEBUG_PRINT("parse_designator_list\n");
    parse_designator(scan);
    while (try_parse(parse_designator, scan)){

    }
}

void parse_designator(Scanner& scan){
    DEBUG_PRINT("parse_designator\n");
    if (scan.current_state.lex == Lex::LSquareBrace){
        scan.get_next();
        parse_constant_expression(scan);
        check(scan, Lex::RSquareBrace);
    } else {
        check(scan, Lex::Dot);
        parse_identifier(scan);
    }
}
