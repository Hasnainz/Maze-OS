[org 0x7c00] ; since offsetting 0x7c00 everywhere is very inconvenient, assemblers let us define a 
             ; global offset for every memory location, with the org command

mov ah, 0x0e ;Oe = 0Eh : display character function loaded into the upper register 
mov al, 'H' ; move 'H' into the register
int 0x10 ; Call the video interupt vector
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
int 0x10
mov al, 'o'
int 0x10
mov al, ' '
int 0x10

mov bx, the_secret ; Move the same offset of the 'the_secret' data from the start of the binary file into bx 
add bx, 0x7c00 ; Add the memory location where the binary file starts in memory so now we have where 'the_secret' lies in
               ; memory
mov al, [bx] ; Moves the value of the memory address contained in bx into al
int 0x10

mov al, [0x7c2a] ; Precalculating where X will be in memory by examining the binary code. Each byte (2 digits)
                 ;
int 0x10

jmp $

the_secret:
    db "X" ;Stores X as 0x58 (hex ascii)

times 510-($-$$) db 0
dw 0xaa55 ;little-endian format - less siginifcant bytes process more significant ones. It will be 55 aa in memory

