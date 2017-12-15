#include "MouseInput.h"

MouseInput Mi; //A global mouse-input structure.

bool mousePressed(int mouseButton) {
    //Check to see if the mouse has just been pressed this tick.
    return Mi.buttons[mouseButton] && ! Mi.lastButtons[mouseButton];
}
bool mouseReleased(int mouseButton) {
    //Check to see if the mouse has just been released this tick.
    return ! Mi.buttons[mouseButton] && Mi.lastButtons[mouseButton];
}
bool mouseDown(int mouseButton) {
    //Check to see if the mouse is down during this tick.
    return Mi.buttons[mouseButton];
}
bool mouseMoving() {
    //Return whether or not the mouse is moving during this tick.
    return Mi.moving;
}
void updateMouse(SDL_Event ev) {
    Uint8 state = SDL_GetMouseState(&Mi.x, &Mi.y); //Update the mouse's x and y coordinates while storing a bit pattern of the state of it's buttons.

    Mi.moving = (ev.type == SDL_MOUSEMOTION);

    for (int i = 0; i < 4; i ++) {
        Mi.lastButtons[i] = Mi.buttons[i]; //Store the current state of the buttons before we update them during this tick.
    }

    for (int i = 1; i < 4; i ++) {
        //We start at 1 as there is no mouseButton with an IDs of 0.
        Mi.buttons[i] = (state >> (i-1)) & 1; //Extract each bit from the bit sequence representing the mouse's state, and store it in the buttons list at an index corresponding to the position of that bit.
    }
}
