; Global Descripter Table which allows us to access our 32-bit of memory

gdt_start: 
  ; The GDT starts with a null 8-byte
  dd 0x0 ; define double word (4 bytes)
  dd 0x0 ; 4 byte


; GDT for code segment. base = 0x00000000, length = 0xfffff
; (present)1 (privilege)00 (descriptor type)1 -> 1001b 
; (code)1 (conforming)0 (readable)1 (accessed)0 -> 1010b
; (granularity)1 (32-bits default)1 (64-bit seg)0 (AVL)0 -> 1100b
gdt_code:
  dw 0xffff    ; segment length, bits 0-15 define word (2 bytes)
  dw 0x0       ; segment base, bits 0-15
  db 0x0       ; segment baes, bits 16-23
  db 10011010b ; flags (8 bits)
  db 11001111b ; flags (4 bits) + segment length, bits 16-19
  db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags are changed
; type flages (code)0 (expand down)0 (writable)1 (accessed)0 -> 0010
gdt_data:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0

gdt_end: ; Have a label to use to calculate the size of the GDT for the GDT descriptor
  

; GDT descriptor 
gdt_descriptor:
  dw gdt_end - gdt_start - 1; size (16bits), always one less of its true size
  dd gdt_start ; address (32 bit)

; Constants for the GDT segment descriptor offsets, which is what our segment registers
; must contain when in protected mode. For example, when we set DS (data segment) = 0x10 
; in PM (protected mode), the CPU knows that we mean it to use the segment 
; described at offset 0x10 (i.e 16 butes) in our GDT, which in our case is the data segment
; (0x0 -> null; 0x08 -> code; 0x10 -> data)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
