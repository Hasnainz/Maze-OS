**This is my attempt at "Robot of the Year" where I have written a kernel
that has the sole purpose to run a self-built version of the robot maze.**

**Running the Kernel: qemu-system-x86-64 os.bin**

In order to run, we need to use a processor with x86 architecture.
I used "qemu-system-x86_64 os.bin" in order to load the binary onto 
a virtual machine so that I don't accidently brick my computer but it should
"in theory" work on any x86 processor. 

Compilation:

If you would like to compile this into a binary yourself it will require quite
a few steps because I used the nasm assembler and a cross-compiler. We need
a cross-compiler because using your normal system compiler to compile the 
c code doesn't always work on the target system.

https://wiki.osdev.org/GCC_Cross-Compiler 

This is where I got a lot of information from regarding the cross-compiler (and a lot
of other information for this project).

I found a shell script on github that installs these dependencies for you 
but it written for debian-based linux distros. I will also take a long time
to make the GCC cross compiler so I don't recommend re-compiling yourself 
unless you already have all the dependencies or you have a lot of time.
Our cross-comiler works for i386 systems because that is what was recommended
from a number of sources. 

To get the debian-based script:
wget https://raw.githubusercontent.com/mell-o-tron/MellOs/main/A_Setup/setup-gcc.sh

To run it - I don't recommend it because it will take a long time:
chmod 711 setup-gcc.sh && ./setup-gcc.sh

I had to add the path variable to my etc/environment and then "source /etc/environment"
for the PATH variable to last across sessions:
export PATH=$PATH:/usr/local/i386elfgcc/bin

So now we can compile c code for our target machine (e.g. a function.c file) by making 
an object file and linking it with our bootloader written in assembly

i386-elf-gcc -ffreestanding -c function.c -o function.o

a freestanding environment is one where we don't have the standard library and where the program
may not be at main - so this flag tells the compiler DON'T assume that the standard functions 
have their normal definitions.

How this works - from the start:





My main sources of guidence and information regarding this project:

https://github.com/cfenollosa/os-tutorial
https://github.com/mell-o-tron/MellOs
https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
https://wiki.osdev.org/Main_Page

