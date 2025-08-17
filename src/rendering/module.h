#pragma once

#include "../define.h"
#include "colors.h"
#include "types.h"

void rendering_init_buffer(DrawBuffer& buffer, int width, int height);
void rendering_clear_screen(DrawBuffer& buffer, u32 bgColor);

void rendering_fill_screen(DrawBuffer& buffer, PixelBuffer& sprite);

void rendering_draw_sprite(DrawBuffer& buffer,
                           PixelBuffer& sprite,
                           v2 position);

template <typename T> using SwapFunction = void (*)(T&, T&, int);
template <typename T> void DefaultSwap(T& source, T& target, int length);

// TODO: where should this live?
template <typename T>
void matrix_row_swap(Matrix<T> m,
                     SwapFunction<T> SwapFunction = DefaultSwap,
                     int length = 0);
