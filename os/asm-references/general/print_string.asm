print_string:
  pusha ; Push all register values onto the stack
  
start:
  mov al, [bx] ; Move the value of the address stored at bx into al
  cmp al, 0 ; compare al == 0 and if it is then the string has ended
  je done ; jump to done if they are equal
  
  mov ah, 0x0e
  int 0x10
  
  add bx, 1
  jmp start

done: 
  popa
  ret

print_nl:
  pusha

  mov ah, 0x0e

  mov al, 0x0a ; print a \n
  int 0x10

  mov al, 0x0d ; print a carriage return which moves the next string line to the start of the line
  int 0x10

  popa
  ret
