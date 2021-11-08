mov ah, 0x0e
mov bx, 100 

cmp bx, 4
jle leq_4 ; Jump to leq_4 if bx is less than or equal to 4 

cmp bx, 40
jl le_40 ; Jump to le_40 if  

mov al, 'C'
jmp end 


leq_4:
  mov al, 'A'
  jmp end
le_40:
  mov al, 'B'
  jmp end
end:

int 0x10

jmp $


times 510-($-$$) db 0
dw 0xaa55
