#include "name_table.h"

#include "scanner.h"
#include <variant>
#include <vector>

enum class NameTableEntryType{
    Var,
    Const,
    Function,
    ScopeOpen,
    ScopeClose
};

enum class PrimitiveType{
    Char,
    UnsignedChar,
    Short,
    UnsignedShort,
    Int,
    UnsignedInt,
    Long,
    UnsignedLong,
    LongLong,
    UnsignedLongLong,
    Float,
    Double,
    Char,
};

struct NameTableEntry{
    std::string name;
    NameTableEntryType entry_type;
    PrimitiveType value_type;
    std::variant<u_int64_t, int64_t, std::string, double> value;
};

struct NameTable{
    std::vector<NameTableEntry> entries;
};