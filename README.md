***A kernel that can only run a maze.***
***
**Overview**
This kernel will generate a maze using Prims, solve it using DFS and then show the path using backtracking - it will repeat this until the machine is shut down. Using the Up and Down arrow keys, you can slow down and speed up the environment. By pressing the Down arrow key enough, the DFS has the instant solve mode because otherwise it can take a lot of time to solve. This is a somewhat simplified version of the maze environment given to us but written from scratch in C.

We can run the binary using a virtual machine (or real hardware if you are brave enough, I don't recommend this because it may damage your system). I will be testing this when I get my hands on a 32-bit machine that runs a i386 processor.

**Running this environment**

**$ make run** or **$ qemu-system-i386 -fda os-image.bin** 
Both will load the binary into the virtual machine.

You need to have the qemu virtual machine installed on your computer in order for this to work (It was available on my package manager (apt)).
***
Everything below this point is supplementary information and is not necessary for running the kernel.
***

****Project Description:****
This project is split into four main sections and below is an explanation on where to find the related code. All the code is commented which should help if you have not come across some of this stuff before and these descriptions should help you find what to look for. The detailed theory behind how to program a very basic OS is below - the same documents that I learned from and adapted into the maze environment. This uses the 32-bit i386 processor because of its simplicity but it also means that we need a cross-compiler if you would like to compile this code for its target machine. And so, since I have already compiled it down into a binary, just running the binary on the virtual machine would be easier than installing a cross-compiler.

**Boot loading the kernel:**
Going into the *boot* folder and opening the *bootsect.asm* file will show the file that begins execution. These files have the goal of loading the kernel into memory and switching into 32-bit protected mode (pm). The theory behind why we have taken the steps that we have taken is outlined in the os-dev document at the bottom of the readme.

**CPU related processes:**
The CPU folder contains interrupts and the PIT (programmable interval timer) so that we  can do things at certain intervals. The interrupts are for keyboard input and so that errors won't restart the processor. We currently only use the up and down arrow keys to decrease and increase the speed respectively which is the only input that this program currently takes. The PIT is used for seeding our random number generator as well as allowing time between drawing pixels so that we get the illusion of animation (this is a sleep function).

**Drivers:**
These files manage the displaying of the maze environment onto the screen by drawing pixels into the correct memory location, keyboard input handling as well as taking input and output from ports (for handling keyboard input).

**Kernel and the Maze Environment:**
This is where the bulk of the actual maze environment is, the rest of it is allowing the kernel to interact with the hardware. The kernel file is what bridges the maze environment to the drivers and CPU and it is the starting location for our environment. The maze files handles generating, solving and tracing the root as well as animating them. We use Prims for generating, DFS for solving and then use the same stack from DFS to trace the root back to the start. This doesn't work on mazes with loops but this isn't a problem because Prims doesn't generates loops. We also always get the shortest path because Prims generates perfect mazes. 

We also have the interesting problem of how to generate random numbers. The standard way would be to have a library to do it for you, however we don't have access to any libraries making this impossible. The next best thing would be to use a well known random number generating algorithm and I used the Mersenne twister because I had come across it before. The random.c file has a link to the paper that I used to implement the algorithm. The next problem is then how do we seed this algorithm - the standard would be to use the system clock but we don't have access to it. And so the solution was the PIT where we can use the constantly increasing PIT to generate our random numbers.

**Compilation:**
If you would like to compile this into a binary yourself it will require quite a few steps because we need to install nasm assembler and a cross-compiler (the same one that I used for this project). We need a cross-compiler because using your normal compiler (e.g. gcc for linux) to compile the c code doesn't always work for the target system.

https://wiki.osdev.org/GCC_Cross-Compiler 

This is where I got a lot of information from regarding the cross-compiler (and a lot of other information for this project).

I found a shell script on github that installs these dependencies for you but it written for debian-based linux distros. I will also take a long time (15 minutes) to make the GCC cross compiler so I don't recommend re-compiling yourself unless you already have all the dependencies or you have the of time. Our cross-comiler works for i386 systems because that is what was recommended across many websites for its simplicity. The binary should work without using the below scripts, but if you would like to compile it you can install the following:

To get the debian-based script:
*$ wget https://raw.githubusercontent.com/mell-o-tron/MellOs/main/A_Setup/setup-gcc.sh*

To run it - I don't recommend it because it will take a long time:
*$ chmod 711 setup-gcc.sh && ./setup-gcc.sh*

Add the path variable to etc/environment and then "source /etc/environment" for the PATH variable to last across sessions:
*$ export PATH=$PATH:/usr/local/i386elfgcc/bin*

So now we can compile c code for our target machine (e.g. a function.c file) by making an object file and linking it with our bootloader written in assembly. The makefile is the automated version of the below command.

*$ i386-elf-gcc -ffreestanding -c function.c -o function.o*

A freestanding environment is one where we don't have the standard library and where the program may not start at main - so this flag tells the compiler not to assume that the standard functions have their normal definitions.

My main source of guidance regarding writing an operating system from scratch. This document is extremely helpful but it is only a draft and so it is incomplete and contains mistakes. It was more than enough to get me started.
https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf

