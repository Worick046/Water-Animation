#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <fstream>
#include <string>
#include "OpenGLRenderer.h"

//Initialize OpenGL with an SDL window
void OpenGLRenderer::init(SDL_Window* window)
{
    //Create OpenGL context for the window an initialize Glad
    SDL_GLContext glContext = SDL_GL_CreateContext( window );
    if(glContext == nullptr)
    {
        std::cout << "Failed to create Opengl Context" << std::endl;
    }
    if(!gladLoadGL())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw -1;
    }

    //Query the window size and set the viewport size to match
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //Set the clear color of the viewport to black
    glClearColor(0, 0, 0, 0);

    //Enable multisampling and depth testing
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    //Enble wireframe drawing and backface culling
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Initialize draw count to zero. This counts how many frames have been drawn
    drawCount = 0;

    //Create model, view, and projection matrices to set up a 3D camera view.
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1.0f));
    model = glm::mat4(1.0f);
    
}


//Draw a frame
void OpenGLRenderer::draw()
{
    //Clear the viewport to black and reset the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Bind shader and vertex array object for drawing
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    //Draw the frame 
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    //Increment draw count
    drawCount++;
}

//Resize the OpenGL rendering viewport
void OpenGLRenderer::resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

//Get the number of total draw calls over the lifetime of the program
unsigned int OpenGLRenderer::getDrawCount()
{
    return drawCount;
}

//Load the mesh into the renderer to be given to the GPU
void OpenGLRenderer::setMesh(mesh surface)
{
    for(unsigned int i = 0; i < surface.vertices.size(); ++i)
    {
        //Split each vertex into their x, y, z components as separate indexes
        //The shader program puts them back together again.
        this->vertices.push_back(surface.vertices[i].x);
        this->vertices.push_back(surface.vertices[i].y);
        this->vertices.push_back(surface.vertices[i].z);
    }
    this->indices = surface.indices;
}


//Helper function to load the contents of the shader files
std::string readfile(std::string filename)
{
    std::ifstream in {filename};
    std::string inputline;
    std::string fileContent;
    while(getline(in, inputline))
    {
        fileContent += inputline;
        fileContent += '\n';
    }
    in.close();
    return fileContent;
}

//Read vertex.glsl and fragment.glsl and compile them into a shader program
void OpenGLRenderer::createShaders()
{
    //Load file data
    std::string vertexShaderSourceString { readfile("./Debug/vertex.glsl") };

    std::string fragmentShaderSourceString { readfile("./Debug/fragment.glsl") };

    //temporary variables to store shader components in.
    unsigned int vertexShader;
    unsigned int fragmentShader;

    //Get the underlying character arrays for the data as the OpenGL calls are c functions and can't
    //use c++ strings
    const char* vertexShaderSource = vertexShaderSourceString.data();
    const char* fragmentShaderSource = fragmentShaderSourceString.data();

    //Compile the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Check the compilation status and output an error if the compilation failed
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Compile the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //Check the compilation status and output an error if the compilation failed
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Link the vertex and fragment shaders together into a shader program
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Check the link status and output an error if the linking failed
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKER::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Delete the temporary shader variables
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Set the shader program as active
    glUseProgram(shaderProgram);

    //Initialize the model, view, and projection uniforms in the shader
    int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    int viewLocation = glGetUniformLocation(shaderProgram, "view");
    int modelLocation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

//Update a 4x4 matrix uniform in the shader program
void OpenGLRenderer::setUniformMatrix(glm::mat4& matrix, std::string uniformName)
{
    const char* name = uniformName.data();
    int matrixLocation = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

//Update a float uniform in the shader program
void OpenGLRenderer::setUniformFloat(float value, std::string uniformName)
{
    const char* name = uniformName.data();
    int uniformLocation = glGetUniformLocation(shaderProgram, name);
    glUniform1f(uniformLocation, value);
}

//Send the mesh data to the GPU
void OpenGLRenderer::commit()
{
    //Create buffers on the GPU
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Set the vertex array object as active
    glBindVertexArray(VAO);

    //Sent vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    //Tell the GPU how to read the vertex data. In this case it will group the vertex data into threes(xyz)
    //Then jump ahead by three to get to the next vertex.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    //Send index data to the GPU
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW); 
}