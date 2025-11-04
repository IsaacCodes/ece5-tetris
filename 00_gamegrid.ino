//Game grid info
constexpr uint8_t gameHeight = 20, gameWidth = 10;
bool gameGrid[gameHeight][gameWidth] = {0};

/*
May want to convert this to a class
Should probably convert this to a bit-packed version
basically an array of 8-bit ints with each bool stored as a bit in one of the ints
cpp may already have this, but if not i can write it
*/