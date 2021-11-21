[bits 32] ; using 32-bit protected mode

; Defining constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x4f ; The colour byte for each character


; Prints a null terminated string where the first element is pointed to by EDX
print_string_pm:
  pusha
  mov edx, VIDEO_MEMORY ; assign edx to modifiy the video memory

print_string_pm_loop:
  mov al, [ebx] ; [ebx] is the value of the address of our character
  mov ah, WHITE_ON_BLACK ; How we want the char to look
  
  cmp al, 0x0 ; Check if we reach the null terminated byte
  je print_string_pm_done

  mov [edx], ax ; store char + attribute in video memory
  add ebx, 1 ; next char
  add edx, 2 ; next video memory position because each space has 2 bytes of info (COLOUR ASCII)
  jmp print_string_pm_loop


print_string_pm_done:
  popa
  ret
