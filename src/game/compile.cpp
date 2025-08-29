// FIXME: some order issue, when player is not included first???
// -> breaks the compiler, gives bs error messages? (void is an incomplete
// type? - seems like it's not detecting the function as a function? Dunno)
#include "player/init.cpp"
#include "player/movement.cpp"
#include "player/update.cpp"

#include "beat/clock.cpp"
#include "module/gameStub.cpp"

#include "world/init.cpp"
#include "world/update.cpp"
