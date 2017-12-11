#include <stdbool.h>
#include <SDL2/SDL.h>

struct KeyInput {
    Uint8 keys[128];
    Uint8 lastKeys[128];
};
typedef struct KeyInput KeyInput;

KeyInput Ki;

bool keyPressed(int key);
bool keyReleased(int key);
bool keyDown(int key);

void updateKeyboard();
