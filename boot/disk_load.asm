; load dh sectors to es:bx from drive dl

disk_load:
  
  push dx ; Store dx onto the stack so we can recall 
          ; how many sectors need to be read even
          ; if we alter it 
  mov ah, 0x02 ; bios read sector function
  mov al, dh ; read the DH sectors
  mov ch, 0x00 ; select cylinder 0 
  mov dh, 0x00 ; select head 0
  mov cl, 0x02 ; Start reading from sector 2 (the sector after the boot sector)
  
  int 0x13 ; bios interrupt

  jc disk_error ; Jump if error

  pop dx
  cmp dh, al ; if sectors read (al) != sectors expected (dh)
  jne disk_error ; display error message
  ret

disk_error: 
  mov bx, DISK_ERROR_MSG
  call print_string
  jmp $

DISK_ERROR_MSG:
  db "Disk read error!", 0
