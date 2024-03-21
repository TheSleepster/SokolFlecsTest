#pragma once

#include "headers.hpp"

void AlpineCreateWindow(); 
void ConfigureOpenGL();
void GetOpenGLVersionInfo();
void handleOpenGLPipeline();
const char *getShaderFromFile(const char *filepath);
unsigned int createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource);
unsigned int CompileShader(unsigned int type, const char *source);
void handleApplicationEvents();
void handleApplicationInput();
void handleVertexData();
void draw();

struct application {
    SDL_Window *window;
    SDL_GLContext context;
    
    bool closeRequest;
};
