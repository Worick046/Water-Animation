#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Water_Animation.h"
#include "mesh.h"


//Generate a flat surface plane with a sidelength and a triangle count of ~= density * density.
mesh generateSurfaceMesh(float sideLength, unsigned int density)
{
    mesh newSurfaceMesh;

    //Generate Vertices
    for(unsigned int i = 0; i < density; ++i)
    {
        for (unsigned int j = 0; j < density; ++j)
        {
            newSurfaceMesh.vertices.push_back({sideLength / density * j, sideLength / density * i, 0.0});
        }
    }

    //Generate Indices
    for(unsigned int i = 0; i < density - 1; ++i)
    {
        for(unsigned int j = 0; j < density - 1; ++j)
        {
            newSurfaceMesh.indices.push_back(j + i * density);
            newSurfaceMesh.indices.push_back(j + 1 + (i + 1) * density);
            newSurfaceMesh.indices.push_back(j + (i + 1) * density);

            newSurfaceMesh.indices.push_back(j + i * density);
            newSurfaceMesh.indices.push_back(j + 1 + i * density);
            newSurfaceMesh.indices.push_back(j + 1 + (i + 1) * density);
        }
    }
    return newSurfaceMesh;
}

void Water_Animation_Initialize(OpenGLRenderer& renderer)
{
    //Create Surface Mesh
    mesh surface = generateSurfaceMesh(10.0f, 500);

    //Pass the mesh to the renderer
    renderer.setMesh(surface);

    //Read and compile shader files
    renderer.createShaders();

    //Send mesh data to the GPU
    renderer.commit();

    //Move and rotate the model so that the camera gets a top down view.
    glm::mat4 model;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -1.0));
    model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1, 0, 0));
    model = glm::translate(model, glm::vec3(-5, -5, 0));

    //Update model uniform in shader
    renderer.setUniformMatrix(model, "model");
}

void Water_Animation_Iterate(OpenGLRenderer& renderer, double time)
{
    //Update time uniform in shader
    renderer.setUniformFloat(time, "time");
}