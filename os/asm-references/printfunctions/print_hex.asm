print_hex:
  pusha
  mov cx, 0

hex_loop:
  cmp cx, 4
  je end

  mov ax, dx
  and ax, 0x000f
  add al, '0'
  cmp al, '9'
  jle step2
  add al, 7

step2:
  mov bx, HEX_OUT + 5 ; Move into the register bx, the address of the end of the string
  sub bx, cx ; Take away the index because we are writing the variables right to left
  mov [bx], al ; copy the al char into the position of bx
  ror dx, 4 ; rotate the register dx by 4 bits (one hex digit)

  add cx, 1
  jmp hex_loop

end:
  mov bx, HEX_OUT
  call print_string
  popa
  ret
HEX_OUT: 
  db '0x0000',0 ; Reserve space for HEX_OUT - The variable being overwrittern 