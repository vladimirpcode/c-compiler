#include "name_table.h"

#include <iostream>

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
    NameTableEntryType nameTableEntryType = NameTableEntryType::ScopeClose;
    return has_name_in_this_scope(name, nameTableEntryType);
}


bool NameTable::has_name_in_this_scope(const std::string& name, NameTableEntryType& nameTableEntryType)
{
    if (entries.size() == 0){
        return false;
    }
    auto iter = entries.end();
    --iter;
    std::cout << iter->name << "\n";
    while (iter != entries.begin() && iter->entry_type != NameTableEntryType::ScopeOpen){
        if (iter->entry_type == NameTableEntryType::ScopeClose){
            while (iter != entries.begin() && iter->entry_type != NameTableEntryType::ScopeOpen){
                --iter;
            }
            if (iter == entries.begin()){
                return false;
            }
        } else {
            if (iter->name == name){
                nameTableEntryType = iter->entry_type;
                return true;
            }
        }
        --iter;
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