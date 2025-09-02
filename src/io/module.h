#pragma once

#include "../rendering/types.h"
#include "types.h"

#include <string>
#include <unordered_map>
#include <vector>

// NOTE: using std::string breaks method reference & linking
//  unsure why exactly this happens

void parse_kvp_file(std::unordered_map<std::string, std::string>* map,
                    const std::string filePath);

void parse_kvp_file(std::unordered_map<std::string, uint32_t>* map,
                    const std::string filePath);

void parse_number_file(std::vector<int>& values, const std::string filePath);
/*
 *  RB swapping is required for display via win32 GDI
    // TODO: check how this is done on Linux / MacOs
 */
void load_sprite(PixelBuffer& buffer, std::string path, bool swapRB = true);

void load_sheet(SpriteSheet& sheet,
                std::string imgPath,
                v2 tileSize,
                bool swapRB = true);

void parse_config_file(const std::string filePath, ConfigStore valueStore);
