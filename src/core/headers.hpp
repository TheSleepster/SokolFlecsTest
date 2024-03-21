#pragma once
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cmath>
#include <cassert>
#include <string>
#include <memory>
#include <time.h>
#include <math.h>
#include <cstdint>
#include <glad/glad.h>
#include <SDL.h>
#include <flecs.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720 
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

#define global static
#define local_persist static
#define internal static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef float real16;
typedef double real32;
