[bits 16]
switch_to_pm:
  cli ; 1. disable interrupts
  lgdt [gdt_descriptor] ; 2. load the table
  mov eax, cr0
  or eax, 0x1; 3. set 32-bit mode bit in cr0
  mov cr0, eax
  jmp CODE_SEG:init_pm ; 4. jump far away using the code segment in the GDT

[bits 32]
init_pm: ; now using 32-bit instructions
  mov ax, DATA_SEG ; 5. update the segment registers
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000 ; 6. update the stack right at the top of the free space
  mov esp, ebp

  call BEGIN_PM ; 7. Go back to the main code
