#pragma once

#include "../scanner.h"

using parsing_function = void (*)(Scanner&);

bool try_parse(parsing_function func, Scanner scan);