#ifndef INPUT_H
#define INPUT_H

    #include <stdint.h>

    typedef enum
    {
        GT_CONTROLLER,
        GT_KEYBOARD
    } InputDevice;

    uint8_t GetKeyboardInput();
    uint8_t GetControllerInput();
    uint8_t GetInput(uint8_t input);
    int GetQuitRequest();

#endif