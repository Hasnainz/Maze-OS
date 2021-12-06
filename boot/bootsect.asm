; The main controller of all the parts of loading the kernel
[org 0x7c00] ; Sets the global memory offset
KERNEL_OFFSET equ 0x1000 ; The same offset we used when linking the kernel

    mov [BOOT_DRIVE], dl ; BIOS sets the boot drive in dl on boot
    mov bp, 0x9000 ; Move the stack (base pointer)
    mov sp, bp

    mov bx, MSG_REAL_MODE ; Print string
    call print
    call print_nl ; print new line

    call load_kernel ; read the kernel from disk

    mov ah, 0x00 ; Switch into VGA mode so we can draw pixels instead of writing text
    mov al, 13h ; If you check the interrupt wiki, 13h then int 0x10 switches to VGA mode
    int 0x10

    call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'

    jmp $ ;Should never be executed

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/32bit_print.asm"
%include "boot/switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, 31 ; How many sectors to read 
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET ; Give control to the kernel
    jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0   
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0
MSG_RETURNED_KERNEL db "Returned from kernel. Error?", 0

; padding because the way that a sector is seen as a bootloader is if it ends in 0xaa55
times 510 - ($-$$) db 0
dw 0xaa55
