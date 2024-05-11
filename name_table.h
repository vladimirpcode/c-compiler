#pragma once

#include "scanner.h"
#include <variant>
#include <vector>

enum class NameTableEntryType{
    TypeName,
    Variable,
    EnumConst,
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
    Struct,
    Union
};

using NameTableValue = std::variant<u_int64_t, int64_t, std::string, double>;

struct NameTableEntry{
    std::string name;
    NameTableEntryType entry_type;
    PrimitiveType value_type;
    NameTableValue value; // если value type - Const
    bool is_mutable;
};

struct NameTableState{
    std::vector<NameTableEntry> entries;
};

class NameTable{
public:
    NameTableState current_state;
    std::stack<NameTableState> states; 
    void open_scope();
    void close_scope();
    bool has_name_in_this_scopes(const std::string& name);
    bool has_name_in_this_scopes(const std::string& name, NameTableEntryType& nameTableEntryType);
    void add_name(std::string name, NameTableEntryType entry_type, PrimitiveType value_type, NameTableValue value, bool is_mutable);
    void save_state();
    void load_state();
    void delete_state();
};

static NameTable name_table;