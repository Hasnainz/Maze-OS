; receiving the data in 'dx'
print_hex:
    pusha

    mov cx, 0 ; our index variable

hex_loop:
    cmp cx, 4 ; loop 4 times
    je end ; finish if cx == 4
    
    ; 1. convert last char of 'dx' to ascii
    mov ax, dx ; we will use 'ax' as our working register
    and ax, 0x000f ; 0xnnnn -> 0x000n by masking first three to zeros where n is any hex char
    add al, 0x30 ; add 0x30 to n to convert it to ascii "n" e.g 0x9 -> 0x39 (0x39 is ascii 9) if you were to check on an ascii table
    cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F' (there is a gap in the ascii table)
    jle step2 ; if we are in range of 0-9 in hex then don't add 7, otherwise add 7
    add al, 7 ; 'A' is ASCII 65 instead of 58, so 65-58=7

step2:
    ; 2. get the correct position of the string to place our ASCII char
    ; bx <- base address + string length - index of char
    mov bx, HEX_OUT + 5 ; base + length, should bring us to the end of the string
    sub bx, cx  ; our index variable, so we can subtract to go the correct position
    mov [bx], al ; copy the ascii char on 'al' to the memort location at 'bx'
    ror dx, 4 ; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234

    ; increment index and then loop
    add cx, 1
    jmp hex_loop

end:
    ; prepare the parameter and call the function print. It receives parameters in 'bx'
    mov bx, HEX_OUT
    call print  

    popa
    ret

HEX_OUT:
    db '0x0000',0 ; reserve memory for our new string
