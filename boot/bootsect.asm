[org 0x7c00]
; A boot sector that boots a C kernel in 32-bit protected mode

KERNEL_OFFSET equ 0x1000 ; memory offset to where we will load our kernel

mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in dl

mov bp, 0x9000 ; set up the stack
mov sp, bp ; stack pointer, base pointer

mov bx, MSG_REAL_MODE ; We are starting by booting from 16-bit real mode
call print_string
call print_nl

call load_kernel ; Load kernel

call switch_to_pm ; switch to protected mode, we won't return

jmp $ ; Shouldn't executre

%include "boot/print_string.asm"
%include "boot/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/print_pm.asm"
%include "boot/switch_to_pm.asm"

[bits 16]

; load kernel

load_kernel:
  mov bx, MSG_LOAD_KERNEL
  call print_string
  call print_nl

  mov bx, KERNEL_OFFSET
  mov dh, 15 ; We load the first 15 sectors (excluding the boot sector) from 
  mov dl, [BOOT_DRIVE]; the boot disk (out kernel code) to address KERNEL_OFFSET
  call disk_load

  ret

[bits 32]
; This is where we should arrive after switching to and initalising protected mode

BEGIN_PM:
  mov ebx, MSG_PROT_MODE ; Use out 32-bit print routine
  call print_string_pm ; announce we are in protected mode

  call KERNEL_OFFSET ; jump to our kernel, fingers crossed.

  jmp $

BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

times 510-($-$$) db 0
dw 0xaa55

