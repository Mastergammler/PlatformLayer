#pragma once

#include <string>
#include <unordered_map>

// FIXME: doing it this way breaks linking ...
// using std::string;
// using std::unordered_map;

void parse_kvp_file(std::unordered_map<std::string, std::string>* map,
                    const std::string filePath);

void parse_kvp_file(std::unordered_map<std::string, uint32_t>* map,
                    const std::string filePath);
