#pragma once

#include "../define.h"
#include <string>

using std::string;

#define NUM_PATTERN CT_NUMBER
#define NUM_NEG_PATTERN (NUM_PATTERN | CT_MINUS)
#define FLOAT_PATTERN (CT_NUMBER | CT_DOT)
#define FLOAT_NEG_PATTERN (FLOAT_PATTERN | CT_MINUS)
#define ALPHA_PATTERN (CT_LOWER | CT_UPPER)

// TODO: do i really need it?
// -> because at the end it should be just a pattern thing?
enum ValueType
{
    T_BOOL,
    T_INT,
    T_FLOAT,
    T_STRING
};

struct NamedValue
{
    bool empty;
    string id;
    int pattern;
    union
    {
        bool b;
        int i;
        float f;
        string* str;
    } value;
    int value_length;
    string value_str;
    ValueType type;
};

struct ConfigStore
{
    NamedValue* items;
    int capacity;
};

enum CharType
{
    CT_EMPTY = 0b0,
    CT_LOWER = 0b1,
    CT_UPPER = 0b1 << 1,
    CT_NUMBER = 0b1 << 2,
    CT_MINUS = 0b1 << 3,
    CT_DOT = 0b1 << 4,
    CT_SPACE = 0b1 << 5
};
