
mov ah, 0x0e

mov al, [secret_char] ; We already know that this points to the wrong memory address
int 0x10

mov bx, 0x7c0 ; Can't set ds directly so use bx as an inbetween
mov ds, bx ; If we offset the data segment then we can access secret_char without org
mov al, [secret_char] ; Now we have set the offset to be 0x7x0 * 16 + secret_char
int 0x10

mov al, [es:secret_char] ; use the es segment instead of the ds segment but this is 0 so we get junk
int 0x10

mov es, bx ; If we now copy bx into es 
mov al, [es:secret_char] ; Now the offset will use extra segment instead of data segment.
int 0x10

jmp $
secret_char:
  db "X", 0


times 510-($-$$) db 0
dw 0xaa55
