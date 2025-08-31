#include "../internal.h"

void draw_ui()
{
    rendering_draw_text(Buffer,
                        Sprites.Font,
                        format("Offset: %.f ms", SongClock.offset * 1000),
                        v2{8, 8},
                        true);
    if (MusicStarted && !PlayerWon)
    {
        rendering_draw_text(Buffer,
                            Sprites.Font,
                            format("Beat: %i %i",
                                   MeasureDivision->current_division,
                                   (BeatDivision->current_division -
                                    1) % SongClock.beats_per_measure + 1),
                            v2{Buffer.width - 8, 8},
                            false);
    }

    string text;
    if (PlayerWon)
    {
        text = "Congratiulations  here is your Biscuit";
    }
    else
    {
        text = "R  Start Reset    Space  Jump    Esc  quit";
    }
    rendering_draw_text(Buffer,
                        Sprites.Font,
                        text,
                        v2{8, Buffer.height - 16},
                        true);
}
