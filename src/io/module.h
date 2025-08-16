#pragma once

#include "../rendering/types.h"

#include <string>
#include <unordered_map>

// FIXME: doing it this way breaks linking ...
// using std::string;
// using std::unordered_map;

void parse_kvp_file(std::unordered_map<std::string, std::string>* map,
                    const std::string filePath);

void parse_kvp_file(std::unordered_map<std::string, uint32_t>* map,
                    const std::string filePath);

/*
 *  RB swapping is required for display via win32 GDI
    // TODO: check how this is done on Linux / MacOs
 */
void load_sprite(PixelBuffer& buffer, std::string path, bool swapRB = true);

void load_sheet(SpriteSheet& sheet,
                std::string imgPath,
                v2 tileSize,
                bool swapRB = true);
