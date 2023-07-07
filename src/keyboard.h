#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL_keyboard.h>

class Keyboard {
public:

	static Keyboard& instance() {
		return s_instance;
	}

	bool is_key_pressed(SDL_Scancode code)
	{
		 const uint8_t* keyboard = SDL_GetKeyboardState(nullptr);
		 return keyboard[code];
	}

private:
	static Keyboard s_instance;

	Keyboard()
	{

	}
};

#endif