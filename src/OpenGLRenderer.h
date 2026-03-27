#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"

//Renderer class that calls OpenGL
class OpenGLRenderer
{
    private:

        //Number of times draw has been called
        unsigned int drawCount;

        //Model data
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        
        //Shader program
        unsigned int shaderProgram;

        //Vertex and index data buffers
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        //Object transformation matrices
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
    public:
        //Initialize OpenGL with an SDL window
        void init(SDL_Window* window);

        //Draw a frame
        void draw();

        //Resize the OpenGL rendering viewport
        void resize(int width, int height);

        //Get the number of total draw calls over the lifetime of the program
        unsigned int getDrawCount();

        //Load the mesh into the renderer to be given to the GPU
        void setMesh(mesh surface);

        //Read vertex.glsl and fragment.glsl and compile them into a shader program
        void createShaders();

        //Update a 4x4 matrix uniform in the shader program
        void setUniformMatrix(glm::mat4& matrix, std::string uniformName);

        //Update a float uniform in the shader program
        void setUniformFloat(float value, std::string uniformName);

        //Send the mesh data to the GPU
        void commit();
};