#include "ast.h"


AST::AST()
    : left(nullptr),
    right(nullptr),
    value(0),
    type(AstNodeType::None),
    operation(Operation::None)
{

}