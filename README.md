# Hazlab2
A "2D random maze" game, navigate through it in 3D

This is a rewritten-from-scratch, modified version of [my previous Hazlab project](https://github.com/zareldyn/hazlab).  
The goal is still to provide an example of simple 3D-graphics application, while being a programming training for me.

Changes from Hazlab:
- command line for choosing the size (rather than a graphical menu)
- the command line also permits to make a no-loop ("perfect") maze, by default a non-perfect maze is generated
- fixed window size (not resizable, no fullscreen)
- written in C99 instead of C90
- use of cglm/glad/GLFW instead of GLUT
- more conventional coding style
- focus on portability across POSIX systems only (no longer Window$-friendly)
- better algorithms

THIS IS STILL A WORK IN PROGRESS

## Build

You need the GLFW3 library; for example on Debian run `apt install libglfw3 libglfw3-dev`.

Install [glad](https://glad.dav1d.de/).  
Select Language C/C++, Specification OpenGL, API gl 3.3 (gles1/2/3 None), Profile Core, no extension, Generate a loader.  
Copy the content of the generated `include` folder to your system (`/usr/local/include/`).

Install (system-wide) [cglm 0.8.5](https://github.com/recp/cglm/tree/v0.8.5#-build).
