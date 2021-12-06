;This file isn't used, it is only in the project for bug testing 32-bit pm mode.

[bits 32] ; using 32-bit protected mode

VIDEO_MEMORY equ 0xb8000 ; Where in memory we write to in order to write to our screen
WHITE_ON_BLACK equ 0x0f ; the color byte for each character

; Loops through a string until we reach the null character and then we return back to the program
print_string_pm:
    pusha ; Push all register values onto the stack
    mov edx, VIDEO_MEMORY ; Copy the video memory location into edx

print_string_pm_loop:
    mov al, [ebx] ; [ebx] is the address of our character
    mov ah, WHITE_ON_BLACK ; The lower register stores the colour combination

    cmp al, 0 ; check if end of string
    je print_string_pm_done ; jump if equal

    mov [edx], ax ; store character + attribute in video memory
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position (Each is 2 bytes long because of char and colour)

    jmp print_string_pm_loop

print_string_pm_done:
    popa ; Pop all the values off the stack, back onto the register
    ret ; Return
