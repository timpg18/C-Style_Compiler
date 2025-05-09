#ifndef TYPECON_H
#define TYPECON_H

#include <string>
#include <unordered_map>
#include <algorithm>

enum class TypeCategory { VOID, BOOL, SIGNED_INT, UNSIGNED_INT, FLOAT };

struct TypeInfo {
    TypeCategory category;
    int rank; // Higher rank = larger/precision type
};

std::unordered_map<std::string, TypeInfo> type_map = {
    {"VOID",                     {TypeCategory::VOID,         0}},
    {"BOOL",                     {TypeCategory::BOOL,         1}},
    
    // Character types
    {"CHAR",                     {TypeCategory::SIGNED_INT,   2}},
    {"SIGNED CHAR",              {TypeCategory::SIGNED_INT,   2}},
    {"UNSIGNED CHAR",            {TypeCategory::UNSIGNED_INT, 2}},
    
    // Short integer types
    {"SHORT",                    {TypeCategory::SIGNED_INT,   3}},
    {"UNSIGNED SHORT",           {TypeCategory::UNSIGNED_INT, 3}},
    {"SHORT INT",                {TypeCategory::SIGNED_INT,   3}},
    {"SIGNED SHORT INT",         {TypeCategory::SIGNED_INT,   3}},
    {"UNSIGNED SHORT INT",       {TypeCategory::UNSIGNED_INT, 3}},
    
    // Regular integer types
    {"INT",                      {TypeCategory::SIGNED_INT,   4}},
    {"UNSIGNED INT",             {TypeCategory::UNSIGNED_INT, 4}},
    
    // Long integer types
    {"LONG",                     {TypeCategory::SIGNED_INT,   5}},
    {"UNSIGNED LONG",            {TypeCategory::UNSIGNED_INT, 5}},
    {"LONG INT",                 {TypeCategory::SIGNED_INT,   5}},
    {"SIGNED LONG INT",          {TypeCategory::SIGNED_INT,   5}},
    {"UNSIGNED LONG INT",        {TypeCategory::UNSIGNED_INT, 5}},
    
    // Long long integer types
    {"LONG LONG",                {TypeCategory::SIGNED_INT,   6}},
    {"UNSIGNED LONG LONG",       {TypeCategory::UNSIGNED_INT, 6}},
    {"LONG LONG INT",            {TypeCategory::SIGNED_INT,   6}},
    {"SIGNED LONG LONG INT",     {TypeCategory::SIGNED_INT,   6}},
    {"UNSIGNED LONG LONG INT",   {TypeCategory::UNSIGNED_INT, 6}},
    
    // Floating-point types
    {"FLOAT",                    {TypeCategory::FLOAT,        7}},
    {"DOUBLE",                   {TypeCategory::FLOAT,        8}},
    {"LONG DOUBLE",              {TypeCategory::FLOAT,        9}}
};

bool isConvertible( const std::string& target,const std::string& source) {
    // Check for valid types
    if (!type_map.count(source) || !type_map.count(target)) return false;
    
    const auto& s = type_map[source];
    const auto& t = type_map[target];

    // Handle void and boolean cases
    if (s.category == TypeCategory::VOID || t.category == TypeCategory::VOID)
        return false;
    if (t.category == TypeCategory::BOOL)
        return true;
    if (s.category == TypeCategory::BOOL)
        return t.category != TypeCategory::VOID;

    // Numeric type conversion rules
    if (t.category == TypeCategory::FLOAT) {
        // All numeric types can convert to float types
        return true;
    }
    
    if (s.category == TypeCategory::FLOAT) {
        // Float can convert to any numeric type
        return true;
    }

    // Integer-to-integer conversions
    if (s.category == TypeCategory::SIGNED_INT) {
        if (t.category == TypeCategory::SIGNED_INT)
            return t.rank >= s.rank;
        if (t.category == TypeCategory::UNSIGNED_INT)
            return t.rank > s.rank;
    }
    
    if (s.category == TypeCategory::UNSIGNED_INT) {
        if (t.category == TypeCategory::SIGNED_INT)
            return t.rank >= s.rank;
        if (t.category == TypeCategory::UNSIGNED_INT)
            return t.rank >= s.rank;
    }

    return false;
}

#endif