section .data
OverflowMessage: db "Você multiplicou dois números grandes demais", 0ah, 0
section .bss
AuxiliaryString: resb 101 ; Reserves a 100 chars string
section .text
; Auxiliary procedures used for file translation.

; stdout = 1 ; stdin = 0 ; read = 3 ; write = 4

; Function to read an integer (Label id: RI).
;
; Parameters:
;   Address to store the number (4 bytes)
;
LerInteiro:

  enter 12, 0      ; Create a stack frame with a 12 byte string.

  ; Save registers
  push ebx
  push ecx
  push edx
  push esi
  push edi

  mov eax, 3        ; Read
  mov ebx, 0        ; Stdin
  mov ecx, ebp      ; Start of the string
  sub ecx, 12	      ; is on ebp-12
  mov edx, 12       ; 11 for number and signal plus \0 or \n
  int 80h	          ; system call

  push eax          ; Saves number of bytes read
  mov esi, ecx      ; esi is the char *
  mov ecx, eax      ; ecx now has the number of bytes read
  dec ecx           ; excluded the \n
  mov edi, 0        ; edi is the signal indicator (0 is positive and 1 is negative)

  cmp byte [esi], 2Dh	  ; tests if the first char is '-' (2Dh = '-')
  jne RI_Positive   ; if it is positive, mantain edx at 0
RI_Negative:
  mov edi, 1	      ; Saves that it is negative
  inc esi           ; Goes to the next number
  dec ecx           ; Decreases the number of chars left
  jz  RI_Error      ; There's an error if there's only a minus
RI_Positive:

  add esi, ecx            ; goes to the least significant digit
  dec esi	                ; actually is there now (length-1)
  mov edx, 1	            ; 10 to the power of 0
  mov eax, 0              ; Starts the accumulation with 0
RI_Loop:
  sub byte [esi], 30h	    ; tests if it is >= '0'
  jl  RI_Error	          ; if it is less than '0' there's an error
  cmp byte [esi], 9       ; tests if it is <= '9'
  jg  RI_Error            ; if it is more than '9' there's an error
  ; [esi] contains the checked value
  push eax	              ; saves for multiplication
  push edx	              ; saves for multiplication
  mov eax, 0	            ; clears eax
  mov al, byte [esi]      ; saves temporarily
  mul edx                 ; multiplies by the base power
  mov ebx, eax	          ; saves result
  mov eax, 10	            ; multiplies by 10
  pop edx	                ; restores ebx
  mul edx	                ; goes to the next power
  mov edx, eax	          ; saves result on edx
  pop eax                 ; restores eax

  add eax, ebx            ; adds to the total
  dec esi	                ; next digit
  dec ecx                 ; counts down
  jnz RI_Loop	            ; tests if it has done all the digits

  cmp edi, 0	            ; tests if there's a signal
  je  RI_NotInvert        ; does not invert of esi = 0
RI_Invert:
  neg eax	                ; negates the signal
RI_NotInvert:
  jmp RI_End              ; finishes the conversion

RI_Error:
  mov eax, 0 ; return as an error
  ;jmp RI_End ; finishes execution

RI_End:

  ; organize outputs
  mov dword [ebp+8], eax    ; Saves the number on the first argument
  pop eax                   ; pops the number of bytes read

  ; Load back registers
  pop edi
  pop esi
  pop edx
  pop ecx
  pop ebx

  leave           ; Destroy the previously created stack frame.

  ret 4           ; Deletes the argument

;------------------------------------------------------------------------------

; Function to write an integer (Label id: WI).
;
; Parameters:
;   Address where the number is being stored (4 bytes).
;
EscreverInteiro:

  enter 0, 0      ; Create a stack frame.

  ; Save the values of the needed registers in the stack.
  ; Except eax, because we will overwrite it with the return value.

  push  ebx
  push  ecx
  push  edx
  push  esi

  sub   esi, esi            ; Clear esi. It will be used to count
                            ; the number of chars printed. (Not
                            ; common practice, but we will need all
                            ; the other registers. If only we had
                            ; more registers...)
  mov   ecx, [ebp + 8]      ; Store the number's address in ecx.
  mov   ebx, [ecx]          ; Store the actual number in ebx.

  cmp   ebx, 0              ; See if the number given is greater
                            ; than 0.
  jge   WI_Positive         ; If it is, the number is positive.

  ; If the above jump doesn't trigger, the number is negative.
  ; Thus, we must print a '-' char and find the 2's complement of
  ; the number.

WI_Negative:
  push  ebx     ; Save the number to be printed.

  ; Store the '-' in the stack.
  sub   esp, 1              ; "Allocate" a byte in the stack.
  mov   byte [esp], 0x2D    ; Save the '-' char.

  ; Print the char in the top of the stack.

  mov   eax, 4
  mov   ebx, 1
  mov   ecx, esp
  mov   edx, 1
  int   0x80

  add   esp, 1        ; Burn the '-' from the stack.
  pop   ebx           ; Restore the original number.

  neg   ebx           ; Find the 2's complement of the number. (That
                      ; way we can reuse the function logic for
                      ; positive numbers).
  add   esi, 1        ; Add 1 to the number of chars printed.

WI_Positive:

  ; Now we need to keep dividing the given number by 10 to obtain
  ; all of it's digits.

  ; Altough we are already counting the number of chars printed in
  ; esi, we will need to count the number of digits in the number in
  ; ecx. This is so because we will store all digits in the stack
  ; and we will need a loop variable to remove and print them. That
  ; variable is ecx.

  sub   ecx, ecx      ; Clear ecx to count the number of digits.
  mov   eax, ebx      ; Move the number to eax for division.

  ; Ok, all set to start dividing!

WI_R_Digit:
  sub   edx, edx      ; Clear edx. (Sign extend a positive number in
                      ; eax).
  mov   ebx, 10       ; Move the divisor to ebx
  div   ebx           ; Divide the number by a 4 byte 10. (Because
                      ; the quotient size is 4 bytes).

  ; Store the remainder (always occupies 1 byte) in the stack.
  sub   esp, 1        ; "Allocate" a byte in the stack.
  add   dl, 0x30      ; Convert digit to char.
  mov   [esp], dl     ; Save char in the stack.

  add   ecx, 1        ; Add 1 to the digit count.

  ; Check the contents of the quotient.
  cmp   eax, 0
  ja    WI_R_Digit    ; If it's not 0, there are more digits to
                      ; store in the stack. Since the quotient is
                      ; already in eax, we just need to clear edx
                      ; and repeat.

  ; If we arrived here, all the digits are neatly stacked and ready
  ; to be printed. We now use ecx as a loop variable to print each
  ; digit in the stack.

WI_P_Digit:
  push  ecx           ; Save the value of ecx.

  ; Print the number digit in the stack.

  mov   eax, 4
  mov   ebx, 1
  mov   ecx, esp
  add   ecx, 4        ; Remeber, the old ecx was pushed to the
                      ; stack. So the digit is in esp + 4.
  mov   edx, 1
  int   0x80

  pop   ecx           ; Restore the value of ecx.
  add   esp, 1        ; Burn the digit printed from the stack.
  add   esi, 1        ; Increase the count of digits printed.

  loop  WI_P_Digit    ; While there are digits left to be printed,
                      ; loop back.

  ; If we got here, all the digits have been printed. Now we just
  ; move the number of chars printed from esi to eax and we're done!

  mov   eax, esi

  ; Restore the values of the registers used.

  pop   esi
  pop   edx
  pop   ecx
  pop   ebx

  leave           ; Destroy the previously created stack frame.

  ret   4         ; Return from the function call.
                  ; And burn 1 function parameter from the stack!

;------------------------------------------------------------------------------

; Function to read a character (Label id: RC).
;
; Parameters:
;   Address where the char will be saved (4 bytes).
;
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
                  ; And burn 1 function parameter from the stack!

;------------------------------------------------------------------------------

; Function to write a character (Label id: WC).
;
; Parameters:
;   Address where the char was saved (4 bytes).
;
EscreverChar:

  enter 0, 0      ; Create a stack frame.

  ; Saves used registers
  push ebx
  push ecx
  push edx

  mov eax, 4        ; Write
  mov ebx, 1        ; Stdout
  mov ecx, [ebp+4]  ; string at ebp+4
  mov edx, 1        ; 1 char

  ; Restores used registers
  pop edx
  pop ecx
  pop ebx

  leave           ; Destroy the previously created stack frame.

  ret 4             ; Deletes arguments
;------------------------------------------------------------------------------

; Function to read a string (Label id: RS).
;
; Parameters:
;   Address where the string will be saved (4 bytes).
;   Maximum number of chars to read (Must have 4 bytes! Max value is 100).
;
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

RS_char_loop:
  push  eax                     ; Save the char count.
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
  je    RS_loop_end

  ; Else, we increase the count and update the string address.

  add   eax, 1
  add   esi, 4          ; Remember, our chars have 4 bytes.
  loop  RS_char_loop

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

RS_loop_end:

  ; End the string read with a '\0'.

  mov   BYTE [esi], 0x00

  ; Restore the values of the registers used.

  pop   esi
  pop   edx
  pop   ecx
  pop   ebx

  leave           ; Destroy the previously created stack frame.

  ret   8         ; Return from the function call.
                  ; And burn 2 function parameters from the stack!

;------------------------------------------------------------------------------

; Function to read a string.
;
; Parameters:
;   Address where the string was saved (4 bytes).
;   Maximum number of chars to write (Must have 4 bytes! Max value is 100).
;
EscreverString:

  enter 0, 0    ; Create a stack frame.

  ; Saving registers
  push ebx
  push ecx
  push edx
  push esi
  push edi

  mov ecx, [ebp+8]          ; Letters counter
  mov esi, [ebp+12]         ; Original string
  mov edi, AuxiliaryString  ; Compressed string

WS_Loop:
  ;mov byte [edi], 0           ; cleans recepient
  mov byte [edi], byte [esi]  ; moves char
  add esi, 4                  ; next 4 byte char
  inc edi                     ; next 1 byte char
  dec ecx                     ; reduces counter
  jnz WS_Loop                 ; do it until the strings ends

  mov eax, 4                  ; Write
  mov ebx, 1                  ; Stdout
  mov ecx, AuxiliaryString    ; First argument (farthest from ebp) string
  mov edx, [ebp+8]            ; Second argument (nearest from ebp) length

  ; Restoring registers
  pop edi
  pop esi
  pop ebx
  pop ecx
  pop edx

  leave           ; Destroy the previously created stack frame.

  ret 8           ; Deletes arguments

;------------------------------------------------------------------------------

; Function to warn when a multiplication cause an overflow.
OverflowWarning:

  enter 0, 0      ; Create a stack frame.

  pusha                     ; saves all

  mov eax, 4                ; Write
  mov ebx, 1                ; Stdout
  mov ecx, OverflowMessage  ; char pointer to the string
  mov edx, 45               ; length of the string

  popa                      ; restores all

  leave           ; Destroy the previously created stack frame.

  ret
