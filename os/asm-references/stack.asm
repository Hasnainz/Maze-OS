mov ah, 0x0e

mov bp, 0x8000 ; Set the base of the stack above where bios loads boot sector
mov sp, bp

push 'A' ; Push chars onto the stack for later, these are pushed as 16-bit values so the most 
push 'B' ; significant byte will be added by our assembler as 0x00
push 'C'

pop bx ; We can only pop 16-bits, so pop to bx then copy bl (ie 8-bit char) to al
mov al, bl
int 0x10 ; print(al)

pop bx
mov al, bl
int 0x10

mov al, [0x7ffe] ; To prove the stack grows downwards from bp, we can get the char at 
                 ; 0x8000 - 0x2 (i.e. 16-bits)
int 0x10

pop bx
mov al, bl
int 0x10

jmp $

times 510-($-$$) db 0
dw 0xaa55
