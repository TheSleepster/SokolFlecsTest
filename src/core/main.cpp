#include "headers.hpp"
#include "main.hpp"

#define AlpineMain main

global application application;

global char *shaderSource;
global unsigned int shaderProgram;

global char glInfoLog[512];
global int success;

int AlpineMain() { 
    AlpineCreateWindow();
    GetOpenGLVersionInfo();

    while(!application.closeRequest) {
        handleApplicationEvents();
        handleApplicationInput();

        //drawing
        
        SDL_GL_SwapWindow(application.window);
    }
}

void AlpineCreateWindow() {    
    application.closeRequest = false;
    application.window = NULL;
    application.context = NULL;

    application.window = 
        SDL_CreateWindow(
            "Hello, Window!", 
             WINDOW_WIDTH,
             WINDOW_HEIGHT, 
             SDL_WINDOW_OPENGL);

    if(application.window == nullptr) {
        printf("Failed to create the application window!\n");
    }

    ConfigureOpenGL();

    application.context = SDL_GL_CreateContext(application.window);
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to initialize GLAD!\n");
        exit(1);
    }

    glViewport(0, 0, 1280, 720);

}

void handleApplicationEvents() {
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt)) {
        switch(evnt.type) {
            case SDL_EVENT_QUIT:
                application.closeRequest = true;
                break;
        }
    }
}

void handleApplicationInput() {
    const u8 *keystate = SDL_GetKeyboardState(NULL);
    if(keystate[SDL_SCANCODE_ESCAPE]) {
        application.closeRequest = true;
    } 
}

void ConfigureOpenGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void GetOpenGLVersionInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Shading Language: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

//NOTE: VERTEX MANAGEMENT

    

// NOTE: SHADER MANAGEMENT


void handleOpenGLPipeline() {
    const char *vertexShaderSource = getShaderFromFile("../src/.shader/vertexshader.glsl");
    const char *fragmentShaderSource = getShaderFromFile("../src/.shader/fragmentshader.glsl");

    shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    free((void *)shaderSource);
}

unsigned int createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
    unsigned int programObject = glCreateProgram();

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    glLinkProgram(programObject);

    glGetProgramiv(programObject, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(programObject, 512, NULL, glInfoLog);
        printf("Failed to link with the ProgramObject!\n");
        printf("Info log: %s", glInfoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return(programObject); 
}

unsigned int CompileShader(unsigned int type, const char *source) {
    unsigned int shaderObject;

    if(type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else if(type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }

    glShaderSource(shaderObject, 1, &source, nullptr);
    glCompileShader(shaderObject);
    
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderObject, 512, NULL, glInfoLog);
        printf("Failed to create the shader Object!\n");
    }

    return(shaderObject);
}

// NOTE: FILE PARSER

const char *getShaderFromFile(const char *filepath) {
    FILE* file;
    long size;

    fopen_s(&file, filepath, "rb");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filepath);
        exit(1);
    }

    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    shaderSource = (char*)malloc(size + 1);
    if (shaderSource == NULL) {
        printf("Failed to allocate memory for shader source\n");
        exit(1);
    }

    fread(shaderSource, 1, size, file);
    shaderSource[size] = '\0';

    fclose(file);

    return(shaderSource);
}
