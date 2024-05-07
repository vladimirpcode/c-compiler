#include "parser_declarations.h"

#include "try_parse.h"

void parse_declaration(Scanner& scan){
    parse_declaration_specifiers(scan);
    try_parse(parse_init_declarator_list, scan);
    check(scan, Lex::SemiColon);
}

void parse_declaration_specifiers(Scanner& scan){
    if (try_parse(parse_storage_class_specifier, scan)){
        try_parse(parse_declaration_specifiers, scan);
        return;
    }
    if (try_parse(parse_type_specifier, scan)){
        try_parse(parse_declaration_specifiers, scan);
        return;
    }
    if (try_parse(parse_type_qualifier, scan)){
        try_parse(parse_declaration_specifiers, scan);
        return;
    }
    if (try_parse(parse_function_specifier, scan)){
        try_parse(parse_declaration_specifiers, scan);
        return;
    }
    throw ExpectedLexException("declaration specifiers", scan.get_line_for_compiler_msg());
}
void parse_init_declarator_list(Scanner& scan){
    parse_init_declarator(scan);
    if (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_init_declarator(scan);
    }
}

void parse_init_declarator(Scanner& scan){
    parse_declarator(scan);
    if (scan.current_state.lex == Lex::Assigment){
        scan.get_next();
        parse_initializer(scan);
    }
}

void parse_storage_class_specifier(Scanner& scan){
    if (scan.current_state.lex == Lex::Typedef
        || scan.current_state.lex == Lex::Extern
        || scan.current_state.lex == Lex::Static
        || scan.current_state.lex == Lex::Auto
        || scan.current_state.lex == Lex::Register)
    {
        scan.get_next();
    } else {
        throw ExpectedLexException("class specifier", scan.get_line_for_compiler_msg());
    }
}

void parse_type_specifier(Scanner& scan){
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
        return;
    }
    if (try_parse(parse_struct_or_union_specifier, scan)){
        return;
    }
    if (try_parse(parse_enum_specifier, scan)){
        return;
    }
    parse_typedef_name(scan);
}

void parse_struct_or_union_specifier(Scanner& scan){
    parse_struct_or_union(scan);
    bool identifier_parsed = try_parse(parse_identifier, scan);
    if (!identifier_parsed && scan.current_state.lex != Lex::LCurlyBrace){
        throw ExpectedLexException("{ struct-declaration-list }", scan.get_line_for_compiler_msg());
    } else {
        if (scan.current_state.lex == Lex::LCurlyBrace){
            check(scan, Lex::LCurlyBrace);
            parse_struct_declaration_list(scan);
            check(scan, Lex::RCurlyBrace);
        }
    }
}

void parse_struct_or_union(Scanner& scan){
    if (scan.current_state.lex == Lex::Struct
        || scan.current_state.lex == Lex::Union)
    {
        scan.get_next();
    } else {
        throw ExpectedLexException("struct | union", scan.get_line_for_compiler_msg());
    }
}

void parse_struct_declaration_list(Scanner& scan){
    parse_struct_declaration(scan);
    while (try_parse(parse_struct_declaration, scan)){

    }
}

void parse_struct_declaration(Scanner& scan){
    parse_specifier_qualifier_list(scan);
    parse_struct_declaration_list(scan);
    check(scan, Lex::SemiColon);
}

void parse_specifier_qualifier_list(Scanner& scan){
    if (try_parse(parse_type_specifier, scan)){
        try_parse(parse_specifier_qualifier_list, scan);
        return;
    }
    if (try_parse(parse_type_qualifier, scan)){
        try_parse(parse_specifier_qualifier_list, scan);
        return;
    }
    throw ExpectedLexException("specifier qualifier list", scan.get_line_for_compiler_msg());
}

void parse_struct_declarator_list(Scanner& scan){
    parse_struct_declarator(scan);
    if (scan.current_state.lex == Lex::Comma){
        scan.get_next();
        parse_struct_declarator(scan);
    }
}

void parse_struct_declarator(Scanner& scan){
    bool parsed_declarator = try_parse(parse_declarator, scan);
    //ToDo
}

void parse_enum_specifier(Scanner& scan){

}

void parse_enumerator_list(Scanner& scan){

}

void parse_enumerator(Scanner& scan){

}

void parse_type_qualifier(Scanner& scan){

}

void parse_function_specifier(Scanner& scan){

}

void parse_declarator(Scanner& scan){

}

void parse_direct_declartor(Scanner& scan){

}

void parse_pointer(Scanner& scan){

}

void parse_type_qualifier_list(Scanner& scan){

}

void parse_parameter_type_list(Scanner& scan){

}

void parse_parameter_list(Scanner& scan){

}

void parse_parameter_declaration(Scanner& scan){

}

void parse_identifier_list(Scanner& scan){

}

void parse_type_name(Scanner& scan){

}

void parse_abstract_declarator(Scanner& scan){

}

void parse_direct_abstract_declarator(Scanner& scan){

}

void parse_typedef_name(Scanner& scan){

}

void parse_initializer(Scanner& scan){

}

void parse_initializer_list(Scanner& scan){

}

void parse_designation(Scanner& scan){

}

void parse_designator_list(Scanner& scan){

}

void parse_designator(Scanner& scan){

}
