# Water Animation Using Gerstner Waves

![Image of wireframe water animation](../result/WaterAnimation.png)

## Description

This project runs a wireframe Gerstner wave simulation using OpenGL for graphical rendering and SDL for windowing and application runtime.

## About Gerstner Waves

Gerstner Wave are a 3D implementation of the Trochoidal wave with a focus on Computer Graphics. The Trochoidal wave works on the idea that each fluid parcel (point) in the wave is moving in a circular motion.

You can learn more about them at https://en.wikipedia.org/wiki/Trochoidal_wave. The equations used for this program are in the In Computer Graphics section.

## Dependencies

This project uses OpenGL, SDL3, Glad, and GLM.

When generating Glad use **gl version 3.3** or higher and change the **profile** to **core**

Dependencies should go in the src/vendor folder
```
src/vendor/SDL
src/vendor/glad
src/vendor/glm
```

## Build

You can build this project using cmake. Go to the build directory and run

```
cmake ../src
cmake --build .
```

Then copy both vertex.glsl and fragment.glsl into the same folder as the generated executable


Finally run the program