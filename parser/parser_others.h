#pragma once

#include "../scanner.h"

void parse_identifier(Scanner& scan);
void parse_constant(Scanner& scan);
void parse_enumeration_constant(Scanner& scan);