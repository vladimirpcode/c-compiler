#include "name_table.h"

#include <iostream>

using namespace std::string_literals;


void NameTable::open_scope(){
    NameTableEntry new_entry;
    new_entry.entry_type == NameTableEntryType::ScopeOpen;
    current_state.entries.push_back(new_entry);
}

void NameTable::close_scope(){
    NameTableEntry new_entry;
    new_entry.entry_type == NameTableEntryType::ScopeClose;
    current_state.entries.push_back(new_entry);
}

bool NameTable::has_name_in_this_scopes(const std::string& name){
    NameTableEntryType entryType = NameTableEntryType::ScopeClose;
    return has_name_in_this_scopes(name, entryType);
}


bool NameTable::has_name_in_this_scopes(const std::string& name, NameTableEntryType& nameTableEntryType)
{
    if (current_state.entries.size() == 0){
        return false;
    }
    auto iter = current_state.entries.end();
    iter--;
    while (iter != current_state.entries.begin() && iter->name != name){
        if (iter->entry_type == NameTableEntryType::ScopeClose){
            int scopes_count = 1;
            iter--;
            while (iter != current_state.entries.begin() && scopes_count != 0){
                if (iter->entry_type == NameTableEntryType::ScopeClose){
                    scopes_count++;
                } else if (iter->entry_type == NameTableEntryType::ScopeOpen){
                    scopes_count--;
                }
                iter--;
            }
        } else {
            if (iter->name == name){
                return true;
            }
            iter--;
        }
    }
    // первая запись должна быть OpenScope
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

void NameTable::save_state(){
    states.push(current_state);
}

void NameTable::load_state(){
    if (states.size() == 0){
        return;
    }
    current_state = states.top();
    states.pop();
}

void NameTable::delete_state(){
    if (states.size() == 0){
        return;
    }
    states.pop();
}