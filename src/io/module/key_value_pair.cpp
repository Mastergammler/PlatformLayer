#include "../internal.h"

int parse_hex_value(const string input, const string filePath)
{
    uint32_t value = 0;
    if (sscanf_s(input.c_str(), "%x", &value) != 1)
    {
        logf("'%s' is not a valid hex input (%s)",
             input.c_str(),
             filePath.c_str());
    }

    return value;
}

void parse_file(const string filePath, function<void(string, string)> insert)
{
    Clock timer = {};
    timer_start(timer);

    ifstream file(filePath);

    if (!file)
    {
        logf("File '%s' could not be opened!", filePath.c_str());
        return;
    }

    const int bufferSize = 256;
    char buffer[bufferSize];

    // 16 characters max length
    char keyBuffer[16];
    char valueBuffer[16];
    int nextKeyIdx = 0;
    int nextValueIdx = 0;

    bool foundKey = false;
    bool foundValue = false;
    bool ignoreRest = false;
    bool eoVal = false;

    int totalBytesRead = 0;

    while (file.read(buffer, bufferSize) || file.gcount() > 0)
    {
        int bytesRead = file.gcount();
        totalBytesRead += bytesRead;

        for (int i = 0; i < bytesRead; i++)
        {
            char ch = buffer[i];

            if (ch == '\n')
            {
                // TODO: validate buffer length!
                // -> log input too long
                if (foundKey && nextValueIdx > 0)
                {
                    // key index is at next position after adding
                    // the current char! therefore it is equal to
                    // the length
                    string key = string(keyBuffer, nextKeyIdx);
                    string valueStr = string(valueBuffer, nextValueIdx);

                    insert(key, valueStr);
                }

                foundKey = false;
                foundValue = false;
                ignoreRest = false;
                eoVal = false;
                nextKeyIdx = 0;
                nextValueIdx = 0;
            }
            // we have end/abort conditions
            else if (ignoreRest || ch == ' ' || ch == '\r')
            {
                // TODO: hanlde eoval - early "abort"
                eoVal = true;
                continue;
            }
            else if (ch == ':')
            {
                foundKey = true;
                eoVal = false;
            }
            else if (ch == '#')
            {
                ignoreRest = true;
            }
            else
            {
                if (!foundKey)
                {
                    keyBuffer[nextKeyIdx++] = ch;
                }
                else if (!foundValue)
                {
                    valueBuffer[nextValueIdx++] = ch;
                }
            }
        }
    }

    file.close();

    float parsingTime = time_since_start(timer);
    logf("Parsed file: '%s' (%i bytes) within %.2f ms",
         filePath.c_str(),
         totalBytesRead,
         parsingTime);
}

void parse_kvp_file(unordered_map<string, string>* map, const string filePath)
{
    parse_file(filePath, [map](string key, string value) {
        map->insert(make_pair(key, value));
    });
}

void parse_kvp_file(unordered_map<string, uint32_t>* map, const string filePath)
{
    parse_file(filePath, [map, filePath](string key, string value) {
        map->insert(make_pair(key, parse_hex_value(value, filePath)));
    });
}
