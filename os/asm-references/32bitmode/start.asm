[org 0x7c00]


mov bp, 0x9000 ; set the stack
mov sp, bp ; stack pointer onto the base pointer

mov bx, MSG_REAL_MODE
call print_string ; this will be written after the bios msg

call switch_to_pm
jmp $ ; Shouldn't be executed

%include "print_string.asm"
%include "32bitprint.asm"
%include "gdt.asm"
%include "32bitswitch.asm"

[bits 32]
BEGIN_PM:
  mov ebx, MSG_PROT_MODE
  call print_string_pm ; prints in protected mode
  jmp $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Started in 32-bit protected mode noobs", 0


BOOT_DISK: db 0                                     
times 510-($-$$) db 0
dw 0xaa55
