
***A kernel that can only run my version of the maze environment.***
***
**Overview**
This kernel will generate a maze using Prims, solve it using DFS and then show the path using backtracking - it will repeat this until the machine is shut down. Using the Up and Down arrow keys, you can slow down and speed up the environment. By pressing the down button enough, the DFS has the instant solve mode because otherwise it can take a lot of time to solve.

We can run the binary using a virtual machine (or real hardware if you are brave enough, I don't recommend this because it may damage your system). I will be testing this when I get my hands on a 32-bit machine that runs a i386 processor.

**Running this environment**

**$ make run** or **$ qemu-system-i386 -fda os-image.bin** 
Both will load the binary into the virtual machine.

You need to have the qemu virtual machine installed on your computer in order for this to work (It was available on my package manager (apt)).

Everything below this point is supplementary information and is not necessary for running the kernel.

**Compilation:**

If you would like to compile this into a binary yourself it will require quite a few steps because I used the nasm assembler and a cross-compiler. We need a cross-compiler because using your normal compiler (e.g. gcc for linux) to compile the c code doesn't always work on the target system.

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

