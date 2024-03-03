#include "name_table.h"

#include "name_table.h"

using namespace std::string_literals;


void NameTable::open_scope(){
    NameTableEntry new_entry;
    new_entry.entry_type == NameTableEntryType::ScopeOpen;
    entries.push_back(new_entry);
}

void NameTable::close_scope(){
    NameTableEntry new_entry;
    new_entry.entry_type == NameTableEntryType::ScopeClose;
    entries.push_back(new_entry);
}


bool NameTable::has_name(const std::string& name){
    for (const auto& entry:entries){
        if (entry.name == name){
            return true;
        }
    }
    return false;
}


bool NameTable::has_name_in_this_scope(const std::string& name){
    auto iter = entries.end();
    --iter;
    while (iter != entries.begin() && iter->entry_type != NameTableEntryType::ScopeOpen){
        if (iter->entry_type == NameTableEntryType::ScopeClose){
            while (iter != entries.begin() 
                && iter->entry_type != NameTableEntryType::ScopeOpen){
                --iter;
            }
            if (iter == entries.begin()){
                return false;
            }
            --iter;
        } else {
            if (iter->name == name){
                return true;
            }
            --iter;
        }
    }
    return false;
}

void NameTable::add_name(std::string name, NameTableEntryType entry_type, PrimitiveType value_type, 
    NameTableValue value, bool is_mutable){
    NameTableEntry new_entry;
    new_entry.name = name;
    new_entry.entry_type = entry_type;
    new_entry.value_type = value_type;
    new_entry.value = value;
    new_entry.is_mutable = is_mutable;
}