#include <SDL2/SDL.h>
#include <stdbool.h>

struct mouseInput {
    int x, y; //Mouse's x and y position.
    bool moving; //Whether the mouse is moving.
    bool buttons[4]; //The current state of the mouse buttons.
    bool lastButtons[4]; //The state of the mouse buttons from the previous tick.
};
typedef struct mouseInput MouseInput;

MouseInput Mi;

bool mousePressed(int mouseButton);
bool mouseReleased(int mouseButton);
bool mouseDown(int mouseButton);
bool mouseMoving();
void updateMouse(SDL_Event ev);
