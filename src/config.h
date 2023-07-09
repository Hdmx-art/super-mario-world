#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <string>

#define SDL_CHECK(code) if(code < 0) __debugbreak();
#define BREAK() __debugbreak();

constexpr auto FRAME_TIME_144 = 1.f/144.f;
constexpr auto FRAME_TIME_60 = 1.f/60.f;

#endif