#pragma once

#include <string>

bool is_upper(char c);
bool is_lower(char c);
bool is_number(char c);
bool contains(const std::string& input, char c);
bool starts_with(const std::string& input, char c);

bool case_insensitive_compare(char a, char b);
bool contains(const std::string& input, const std::string& substring);

bool starts_with(const std::string& input,
                 const std::string& startSequence,
                 bool caseSensitive = false);

bool ends_with(const std::string& input,
               const std::string& substring,
               bool caseSensitive = false);
void trim(std::string& input, std::string additionalTrimChars = "");
void trim_start(std::string& input, const std::string& sequence);
void remove_until(std::string& input, char c, bool last);
bool case_insensitive_equals(const std::string a, const std::string b);

std::string trim_to_var_name(std::string s);
