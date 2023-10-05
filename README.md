# Fraktale

This project is a fractal visualization program created as a part of the German Gymnasium's "Komplexe Leistung" project. Using OpenGL and ImGui, it enables users to explore and render many kinds of fractals.

<table>
  <tr>
    <td align="center">
      <img src="screenshots/Mandelbrot.png" alt="Mandelbrot Set" width="250"/>
    </td>
    <td align="center">
      <img src="screenshots/MandelbrotZoom.png" alt="Mandelbrot Set zoomed" width="250"/>
    </td>
  </tr>
  <tr>
    <td align="center">
      <img src="screenshots/Julia.png" alt="Julia Set" width="250"/>
    </td>
    <td align="center">
      <img src="screenshots/BurningShip.png" alt="Burning Ship" width="250"/>
    </td>
  </tr>
</table>

## Features

-  Choose between different fractal types: Mandelbrot, Julia, and Burning Ship.
-  Adjust the number of iterations for rendering.
-  Customize color schemes for the fractals.
-  Real-time preview of fractals with user-friendly controls.
-  Interactive interface powered by ImGui for parameter adjustment.

## Dependencies

This project relies on the following libraries and tools:

-  [CMake](https://cmake.org/): An open-source, cross-platform build system.
-  [CMakeRC](https://github.com/vector-of-bool/cmrc): A Resource Compiler in a Single CMake Script.
-  [GLFW](https://www.glfw.org/): A multi-platform library for creating windows with OpenGL contexts.
-  [GLEW](http://glew.sourceforge.net/): The OpenGL Extension Wrangler Library for easier handling of OpenGL extensions.
-  [GLM](https://github.com/g-truc/glm): A header-only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
-  [ImGui](https://github.com/ocornut/imgui): A Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies.
