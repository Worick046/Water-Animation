#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;


//Generate Gerstner Waves and return the position of the vertex.
vec3 calculateGerstnerWaves(int numberOfWaves)
{
    float pi = 3.1415926535897932384626433832795;
    float direction; //Direction is a whole number of radians and is used to set ky and kx variables.
    float amplitude = 0.5;
    float wavelength = 3;

    //The variables x, y, and z will hold the total sum of all the waves and be returned as a position vector.
    float x = 0;
    float y = 0;
    float z = 0;

    //Wavenumber variables. kx and ky determine the wavenumber in the x and y directions and determine the
    //overall direction of the wave. k is the combined wavenumber.
    float ky;
    float kx;
    float k;


    float angular;
    float phase = 10;
    float theta;
    float g = 9.81;

    for (int i = 0; i < numberOfWaves; ++i)
    {
        direction = i;
        ky = 2 * pi / wavelength * cos(direction);
        kx = 2 * pi / wavelength * sin(direction);
        k = sqrt(kx * kx + ky * ky);
        angular = sqrt(g * k * tanh(k));
        theta = kx * pos.x + ky * pos.y - angular * time / 10 - phase;
        x += pos.x - kx / k * amplitude / tanh(k) * sin(theta);
        y += pos.y - ky / k * amplitude / tanh(k) * sin(theta);
        z += amplitude * cos(theta);

        wavelength *= 0.8;
        amplitude *= 0.8;
    }
    z -= 3;
    x -= 30;
    return vec3(x, y, z);
}

void main() {
    gl_Position = projection * view * model * vec4(calculateGerstnerWaves(8), 1.0);
}