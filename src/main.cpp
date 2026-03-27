#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>
#include <chrono>
#include <Windows.h>
#include "OpenGLRenderer.h"
#include "Water_Animation.h"

//SDL window pointer
SDL_Window* window;

//Keeps track of the number of frames that have been rendered over the lifetime of the program
unsigned int frameCount = 0;

//Keeps track of the start time of the program for animation purposes
std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

//This object handles all of the OpenGL calls for the program
OpenGLRenderer renderer;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{

    //Configure SDL for OpenGL version 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Enable multisampling on the SDL window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    
    // create a window
    window = SDL_CreateWindow("Water Animation", 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    
    //Create a timestamp to compare with to measure the time in the program.
    //This is used for animation purposes
    startTime = std::chrono::high_resolution_clock::now();

    //Initialize OpenGL Renderer
    renderer.init(window);

    //Call the initialization code for the Water Animation Project
    Water_Animation_Initialize(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    //Increment the framecount
    frameCount++;

    //Get the time in seconds since the start of the program
    std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();
    double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - startTime).count();

    //Lock the framerate to a maximum of 60 frames per second
    Sleep(16);

    //Call the Water Animation iteration code
    Water_Animation_Iterate(renderer, elapsedTime);

    //Draw a frame
    renderer.draw();

    //std::cout << "FrameCount: " << renderer.getDrawCount() << std::endl;

    //Swap the window buffers to display the newly drawn frame
    SDL_GL_SwapWindow(window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    // Close the window on request
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
    {
        return SDL_APP_SUCCESS;
    }
    // Resize the window on request
    if (event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        int width, height;

        SDL_GetWindowSize(window, &width, &height);
        renderer.resize(width, height);
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // destroy the window
    SDL_DestroyWindow(window);
}