# Hazlab2
A "2D random maze" game, navigate through it in 3D

This is a rewritten-from-scratch, modified version of [my previous Hazlab project](https://github.com/zareldyn/hazlab).  
The goal is still to provide an example of simple 3D-graphics application, while being a programming training for me.

Changes from Hazlab:
- command line for choosing the size (rather than a graphical menu)
- the command line also permits to make a no-loop ("perfect") maze, by default a non-perfect maze is generated
- no textures
- fixed window size (not resizable, no fullscreen)
- written in C99 instead of C90
- use of GLFW instead of GLUT
- more conventional coding style
- focus on portability across POSIX systems only (no longer Window$-friendly)
- better algorithms
- MSAA

THIS IS STILL A WORK IN PROGRESS  
TODOS:
- add something?
- change colors?
- nice ending when the exit is reached (instead of brutal program quit)

## Build

You need the GLFW3 library; for example on Debian run `apt install libglfw3 libglfw3-dev`.

Install [glad](https://glad.dav1d.de/).  
Select Language C/C++, Specification OpenGL, API gl 3.3 (gles1/2/3 None), Profile Core, no extension, Generate a loader.  
Copy the content of the generated `include` folder to your system (`/usr/local/include/`).  
Also, copy `glad.c` into the hazlab2 project folder, next to the other C source files.

Install (system-wide) [cglm 0.8.5](https://github.com/recp/cglm/tree/v0.8.5#-build).

You can now compile it: `make release`. It produces an executable named "hazlab2" in the project folder.

## Usage
([docopt notation](http://docopt.org/) is used)

`./hazlab2 [-s<n>] [-p] [-a<n>]`

The `-s` option controls the size of the randomly-generated square maze. `n` must be between 1 and 16, the default is 8.  
A value of 1 is of no interest (just for test/debug purpose).

The `-p` option tells hazlab2 to create a perfect maze (no loops). Otherwise there *may* be loops in it (the smaller the maze is, the highest probability it's perfect, even without `-p`).

The `-a` option controls MSAA (`n` samples). The default is 2, the max is 8. Set it to 0 to disable AA.
