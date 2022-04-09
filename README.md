# Hazlab2
A "2D random maze" game, navigate through it in 3D

This is a rewritten-from-scratch, modified version of [my previous Hazlab project](https://github.com/zareldyn/hazlab).  
The goal is still to provide an example of simple 3D-graphics application, while being a programming training for me.

Changes from Hazlab:
- command line for choosing the size (rather than a graphical menu)
- the command line also permits to make a no-loop ("perfect") maze, by default a non-perfect maze is generated
- fixed window size (not resizable, no fullscreen)
- written in C99 instead of C90
- use of GLFW instead of GLUT
- better code organization and more conventional coding style
- focus on portability across POSIX systems only (no longer Window$-friendly)
- better algorithms

THIS IS STILL A WORK IN PROGRESS (3D part not started yet)
