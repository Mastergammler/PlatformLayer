#include "../internal.h"

void draw_ui()
{
    rendering_draw_text(Buffer,
                        Sprites.Font,
                        format("Nudge "
                               "offset:"
                               " %.f "
                               "ms",
                               SongClock.offset * 1000),
                        v2{8, 8},
                        true);
    rendering_draw_text(Buffer,
                        Sprites.Font,
                        format("Beat: %i %i",
                               MeasureDivision->current_division,
                               BeatDivision->current_division % SongClock.beats_per_measure +
                                                                       1),
                        v2{Buffer.width - 8, 8},
                        false);
}
