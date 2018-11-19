; Auxiliary procedures used for file translation.

; Procedure to read an integer.
LerInteiro:

  enter 0, 0      ; Create a stack frame.

  leave           ; Destroy the previously created stack frame.

ret

; Procedure to write an integer.
EscreverInteiro:

  enter 0, 0      ; Create a stack frame.

  leave           ; Destroy the previously created stack frame.

ret

; Procedure to read a character.
LerChar:

              enter 0, 0      ; Create a stack frame.

              ; Save the values of the needed registers in the stack.
              ; Except eax, because we will overwrite it with the return value.

              push  ebx
              push  ecx
              push  edx

              ; Read the char and the '\n' to the given memory position.
              ; Since all allocated memory has the size of a double, the '\n'
              ; can go with the char read without causing any trouble.
              ; Pretty bizarre!

              mov   eax, 3
              mov   ebx, 0
              mov   ecx, [ebp + 8]
              mov   edx, 2
              int   0x80

              ; Set the return value for eax.

              mov   eax, 1

              ; Restore the values of the registers used.

              pop   edx
              pop   ecx
              pop   ebx

              leave           ; Destroy the previously created stack frame.

              ret   4         ; Return from the function call.
                              ; 1 parameter pushed into the pile is gone!

; Procedure to write a character.
EscreverChar:

  enter 0, 0      ; Create a stack frame.

  leave           ; Destroy the previously created stack frame.

ret

; Procedure to read a string.
LerString:

              enter 0, 0      ; Create a stack frame.

              ; Save the values of the needed registers in the stack.
              ; Except eax, because we will overwrite it with the return value.

              push  ebx
              push  ecx
              push  edx
              push  esi

              sub   eax, eax                ; Clear eax. It will be used to
                                            ; count the number of chars read.
              mov   DWORD ecx, [ebp + 8]    ; Load ecx with the maximum ammout
                                            ; of chars to be read.
              mov   esi, [ebp + 12]         ; Load esi with the address for the
                                            ; start of the string.

LS_char_loop: push  eax                     ; Save the char count.
              push  ecx                     ; Save the loop count.

              ; Read one char to the next position in the string.

              mov   eax, 3
              mov   ebx, 0
              mov   ecx, esi
              mov   edx, 1
              int   0x80

              ; Restore the values for the loop count and the char count.

              pop   ecx
              pop   eax

              ; If we got a '\n', the string is over.

              cmp   BYTE [esi], 0x0A
              je    LS_loop_end

              ; Else, we increase the count and update the string address.

              add   eax, 1
              add   esi, 4          ; Remember, our chars have 4 bytes.
              loop  LS_char_loop

              ; If we got to this point, the string is full. But, there might
              ; be an incoming '\n' to finish the string. So, let's just try to
              ; capture that to a unused byte in the string.
              ; Since all allocated memory has the size of a double, the '\n'
              ; can go with the last char read without causing any trouble.
              ; Pretty bizarre!


              push  eax             ; Save the char count. (The '\n' doesn't
                                    ; count!)

              add   esi, 1          ; Still inside the last 4 byte char.

              mov   eax, 3
              mov   ebx, 0
              mov   ecx, esi
              mov   edx, 1
              int   0x80

              pop   eax             ; Restore the char count.

LS_loop_end:

              ; Restore the values of the registers used.

              pop   esi
              pop   edx
              pop   ecx
              pop   ebx

              leave           ; Destroy the previously created stack frame.

              ret   8         ; Return from the function call.
                              ; 2 parameter pushed into the pile are gone!

; Procedure to write a character.
EscreverString:

  enter 0, 0      ; Create a stack frame.

  leave           ; Destroy the previously created stack frame.

ret

; Procedure to warn when a multiplication cause an overflow.
OverflowWarning:

  enter 0, 0      ; Create a stack frame.

  leave           ; Destroy the previously created stack frame.

ret
