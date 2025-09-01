#include "../internal.h"

void parse_number_file(vector<int>& values, const string filePath)

{
    Clock timer = {};
    timer_start(timer);

    ifstream file(filePath);

    if (!file)
    {
        logf("File '%s' could not be opened!", filePath.c_str());
        return;
    }

    char keyBuffer[8];
    int nextKeyIdx = 0;
    const int bufferSize = 256;
    char buffer[bufferSize];
    int totalBytesRead = 0;

    int isComment = false;
    int valueEnd = false;

    while (file.read(buffer, bufferSize) || file.gcount() > 0)
    {
        int bytesRead = file.gcount();
        totalBytesRead += bytesRead;

        for (int i = 0; i < bytesRead; i++)
        {
            char ch = buffer[i];

            if (ch == '#')
            {
                isComment = true;
            }
            else if (ch == '\n')
            {
                // comment ended
                isComment = false;
                valueEnd = true;
            }
            else if (ch == '\r' || isComment)
                continue;
            // symbol end found
            else if (ch == ' ')
            {

                valueEnd = true;
            }
            else
            {
                keyBuffer[nextKeyIdx++] = ch;
            }

            if (valueEnd)
            {
                if (nextKeyIdx > 0)
                {
                    string substr(keyBuffer, nextKeyIdx);
                    values.push_back(stoi(substr));
                }
                nextKeyIdx = 0;
                valueEnd = false;
            }
        }
    }

    file.close();

    float parsingTime = time_since_start(timer);
    logf("| %.3f ms | Parsed file: '%s' (%i bytes)",
         parsingTime,
         filePath.c_str(),
         totalBytesRead);
}
