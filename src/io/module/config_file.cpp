#include "../internal.h"

#define COMMENT '#'
#define KV_SEP ':'
#define EOL '\n'
#define CR '\r'
#define SPACE ' '
#define MINUS '-'
#define DOT '.'
#define TRUE_STR "true"
#define FALSE_STR "false"
#define TMP_SIZE 8
#define READ_BUFFER_SIZE 256
#define MAX_KEY_SIZE 16
#define MAX_VALUE_SIZE 64

struct ValueBuffer
{
    char chars[MAX_VALUE_SIZE];
    int index;
    int pattern;
};

int get_char_type_bit(char c)
{
    if (is_upper(c)) return CT_UPPER;
    if (is_lower(c)) return CT_LOWER;
    if (is_number(c)) return CT_NUMBER;
    if (c == SPACE) return CT_SPACE;
    if (c == MINUS) return CT_MINUS;
    if (c == DOT) return CT_DOT;
    return CT_EMPTY;
}

// TODO: REF - akward logic, after each match it can return
//  -> how would you write this in a better way?
// TODO: handle 0 and 1 as valid bool params also
void parse_values(NamedValue* val)
{
    if (val->value_length == 0)
    {
        // treating default as strings as well
        val->empty = true;
        val->type = T_STRING;
        return;
    }

    if (val->pattern == NUM_PATTERN)
    {
        val->type = T_INT;
        val->value.i = stoi(val->value_str);
        return;
    }

    if (val->pattern == NUM_NEG_PATTERN && val->value_str[0] == MINUS)
    {
        val->value.i = stoi(val->value_str);
        val->type = T_INT;
        return;
    }

    if (val->pattern == FLOAT_PATTERN)
    {
        val->type = T_FLOAT;
        val->value.f = stof(val->value_str);
        return;
    }

    if (val->pattern == FLOAT_NEG_PATTERN && val->value_str[0] == MINUS)
    {
        val->value.f = stof(val->value_str);
        val->type = T_FLOAT;
        return;
    }

    // letters only, but doesn't need upper & lower case
    // so this is a OR test not a AND test
    if ((val->pattern & ALPHA_PATTERN) > 0 &&
        (val->value_length == 4 || val->value_length == 5))
    {
        if (case_insensitive_equals(val->value_str, TRUE_STR))
        {
            val->type = T_BOOL;
            val->value.b = true;
            return;
        }
        else if (case_insensitive_equals(val->value_str, FALSE_STR))
        {
            val->type = T_BOOL;
            val->value.b = false;
            return;
        }

        // this means it's not a valid bool
    }

    val->type = T_STRING;
    val->value.str = &val->value_str;
}

int parse_text(ifstream& file, ConfigStore store)
{
    char buffer[READ_BUFFER_SIZE];

    // TODO: REF - i could move these also into a struct and then have smaller
    // functions here
    //  -> but i don't think it's necessary right now
    ValueBuffer keyBuffer = {};
    ValueBuffer valueBuffer = {};
    int totalBytesRead = 0;
    int storeIndex = 0;

    bool insideComment = false;
    bool reachedEok = false;

    int spaceBufferCount = 0;

    while (file.read(buffer, READ_BUFFER_SIZE) || file.gcount() > 0)
    {
        int bytesRead = file.gcount();
        totalBytesRead += bytesRead;

        for (int i = 0; i < bytesRead; i++)
        {
            char ch = buffer[i];
            if (insideComment && ch != EOL) continue;

            switch (ch)
            {
                case EOL:

                    // ignore empty lines
                    if (keyBuffer.index > 0)
                    {
                        if (storeIndex < store.capacity)
                        {
                            NamedValue* value = &store.items[storeIndex++];
                            value->id = string(keyBuffer.chars,
                                               keyBuffer.index);
                            value->value_str = string(valueBuffer.chars,
                                                      valueBuffer.index);
                            value->value_length = valueBuffer.index;
                            value->pattern = valueBuffer.pattern;
                            parse_values(value);
                        }
                        else
                        {
                            logf("Config store capacity %i reached, key %s "
                                 "skipped!",
                                 store.capacity,
                                 string(keyBuffer.chars, keyBuffer.index).c_str());
                            ASSERT(false, "Increase config store capacity!");
                        }
                    }

                    insideComment = false;
                    reachedEok = false;
                    keyBuffer.index = 0;
                    valueBuffer.index = 0;
                    keyBuffer.pattern = 0;
                    valueBuffer.pattern = 0;
                    spaceBufferCount = 0;
                    break;
                case COMMENT: insideComment = true; break;
                case SPACE: spaceBufferCount++; break;
                case KV_SEP:
                    spaceBufferCount = 0;
                    reachedEok = true;
                    break;
                case CR: continue;
                default:
                    ValueBuffer* current = reachedEok ? &valueBuffer
                                                      : &keyBuffer;

                    // currently we found a valid char -> spaces before belong
                    // to the string
                    for (int i = 0; i < spaceBufferCount; i++)
                    {
                        // start of the string: we ignore spaces
                        if (current->index == 0) break;
                        current->chars[current->index++] = SPACE;
                        current->pattern |= get_char_type_bit(SPACE);
                    }
                    spaceBufferCount = 0;

                    current->chars[current->index++] = ch;
                    current->pattern |= get_char_type_bit(ch);
                    break;
            }
        }
    }

    return totalBytesRead;
}

void parse_config_file(const string filePath, ConfigStore values)
{
    Clock timer = {};
    timer_start(timer);

    ifstream file(filePath);
    if (!file)
    {
        logf("File '%s' could not be opened!", filePath.c_str());
        return;
    }

    int bytesRead = parse_text(file, values);
    file.close();

    float parsingTime = time_since_start(timer);
    logf("| %.3f ms | Parsed config file: '%s' (%i bytes)",
         parsingTime,
         filePath.c_str(),
         bytesRead);
}
