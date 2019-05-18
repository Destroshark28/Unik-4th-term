.model tiny
.code    
.186
org 100h  

start:  

jmp install_handler
 
text db 50 dup(0)  
buf db 400 dup(0)
text_size dw ?
endl db 13,10 
timer db 0 
result_txt db "/result.txt",0 
new_cycle_of_search db 1 
error db "Error! There is no input word for search$",13,10              

handler proc          
    pusha			  
    push 0B800h		
    pop es				
    push cs
    pop ds   
     
    cmp timer,100
    jge main
    inc timer
    jmp not_found

    main:   
        mov new_cycle_of_search, 1
        mov timer, 0      
        xor bx, bx
    
    pre_found:
        xor di, di
        mov al, text[di]       
        
    find_firt_letter:
        cmp es:[bx], al
        je check_word 
        cmp bx, 0FA0h
        je not_found
        inc bx
        jmp find_firt_letter
    
    check_word:					
        add bx,2
        inc di
        cmp di, text_size
        je found 
        mov al, text[di]     
        cmp es:[bx], al
        jne pre_found   
        jmp check_word   
    
    found:         
        lea dx, result_txt       
        mov ah, 3Dh             
        mov al, 01b              
        int 21h
     
        mov si, bx
        push bx
    
        mov bx,ax
             
        mov ah, 42h              
        xor cx, cx  
        xor dx, dx
        mov al, 2
        int 21h      
    
        xor di, di
        sub si, text_size
        sub si, text_size
        sub si, 6
        mov cx, text_size
        add cx, 6          
        
    copy_loop: 				     
        mov al, es:[si]
        mov buf[di], al
        inc di
        add si, 2
        loop copy_loop 
    
        cmp new_cycle_of_search, 1
        jne skip_pref
        mov new_cycle_of_search, 0   

    skip_pref:          
        mov ah, 40h                       
        lea dx, buf  
        mov cx, text_size
        add cx, 6
        int 21h 
       
        mov ah, 40h                       
        lea dx, endl  
        mov cx, 2
        int 21h         
                    
        mov ah,3Eh                      
        int 21h
    
        pop bx
        jmp pre_found
    
    not_found:  
        popa                         
        db 0EAh 
        old_9h dd ?                      
        iret
handler endp                          

install_handler:   
  
    lea di, text         
    call parseCommandLineArgs     
    
    xor cx, cx
    mov ah, 3Ch                                  
    lea dx, result_txt
    int 21h
     
    mov ah,3Eh                                  
    int 21h
    
    mov ah, 35h 								
    mov al, 08h                                
    int 21h                                         
        
    mov word ptr old_9h, bx                     
    mov word ptr old_9h+2, es                   
    
    mov ah, 25h 							    
    mov al, 08h									
    mov dx,offset handler                       
    int 21h 
    
    mov ah, 31h                                 
    mov al, 00h                                  
    mov dx, (install_handler - start + 10Fh) / 16   
    int 21h     
    
parseCommandLineArgs proc                              
    xor cx,cx          
    mov si, 80h
    mov cl, es:[si]                          
    cmp cx, 0
    je error_line_exit
    
    inc si    
    
    cycle:       
        mov al, es:[si] 
        cmp al, ' '
        je next_step 
        cmp al, 13
        je next_step
        mov [di], al   
        inc di 
        inc text_size 
        
    next_step:
        inc si 
        loop cycle
    
        cmp text_size, 0
        je error_line_exit
        cmp text_size, 50
        jge error_line_exit
        ret  
    
    error_line_exit:
        mov ah, 09h  
        lea dx, error
        int 21h
        mov ah, 4ch
        int 21h
endp
    
end start
