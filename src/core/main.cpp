#include "main.hpp"

#define AlpineMain main

global application application;

global char *shaderSource;
global unsigned int shaderProgram;
global unsigned int VAO;
global unsigned int VBO;

global char glInfoLog[512];
global int success;

int AlpineMain() { 
    GLenum error;

    AlpineCreateWindow();
    GetOpenGLVersionInfo();

    while(!application.closeRequest) {
        handleApplicationEvents();
        handleApplicationInput();

        draw();
        
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

    handleVertexData();
    handleOpenGLPipeline();
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

// NOTE: VERTEX MANAGEMENT

void handleVertexData() {
    float vertices[] = {
        // Triangle 1
        -0.5f, -0.5f, 0.0f, // Vertex 1 position
         1.0f,  0.0f, 0.0f, // Vertex 1 color (red)
         0.5f, -0.5f, 0.0f, // Vertex 2 position
         0.0f,  1.0f, 0.0f, // Vertex 2 color (green)
         0.5f,  0.5f, 0.0f, // Vertex 3 position
         0.0f,  0.0f, 1.0f, // Vertex 3 color (blue)

        // Triangle 2
         0.5f,  0.5f, 0.0f, // Vertex 4 position
         0.0f,  0.0f, 1.0f, // Vertex 4 color (blue)
        -0.5f,  0.5f, 0.0f, // Vertex 5 position
         0.0f,  1.0f, 0.0f, // Vertex 5 color (green)
        -0.5f, -0.5f, 0.0f, // Vertex 6 position
         1.0f,  0.0f, 0.0f  // Vertex 6 color (red)
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Configure color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

// NOTE: DRAW FUNCTION

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glUseProgram(0);
}

// NOTE: SHADER MANAGEMENT


void handleOpenGLPipeline() {
    const char *vertexShaderSource = getShaderFromFile("../src/.shader/vertexShader.glsl");
    const char *fragmentShaderSource = getShaderFromFile("../src/.shader/fragmentShader.glsl");

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
