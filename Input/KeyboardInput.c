#include "KeyboardInput.h"

KeyInput Ki;

bool keyPressed(int key) {
    return (Ki.keys[key]) && (! Ki.lastKeys[key]);
}
bool keyReleased(int key) {
    return (Ki.lastKeys[key]) && (! Ki.keys[key]);
}
bool keyDown(int key) {
    return Ki.keys[key];
}

void updateKeyboard() {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < 128; i ++) {
        Ki.lastKeys[i] = Ki.keys[i];
    }

    for (int i = 0; i < 128; i ++) {
        Ki.keys[i] = state[i];
    }
}
