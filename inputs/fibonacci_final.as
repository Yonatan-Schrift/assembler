            ;------------------------------------------------------------
            ; Fibonacci Sequence Program using Non-Parameterized Macros
            ;------------------------------------------------------------

FIB0:       .data 0          ; First Fibonacci number (0)
FIB1:       .data 1          ; Second Fibonacci number (1)
FIB2:       .data 0          ; Placeholder for FIB2
FIB3:       .data 0          ; Placeholder for FIB3
FIB4:       .data 0          ; Placeholder for FIB4
FIB5:       .data 0          ; Placeholder for FIB5
FIB6:       .data 0          ; Placeholder for FIB6
FIB7:       .data 0          ; Placeholder for FIB7
FIB8:       .data 0          ; Placeholder for FIB8
FIB9:       .data 0          ; Placeholder for FIB9

;------------------------------------------------------------
; Macro Definitions (non-parameterized)
; Each macro computes the next Fibonacci number and stores it.
;------------------------------------------------------------

mcro CALC_FIB_2
    mov FIB0, r0    ; Load FIB0 into r0
    add FIB1, r0    ; Add FIB1 (r0 now = FIB0 + FIB1)
    mov r0, FIB2    ; Store result into FIB2
mcroend

mcro CALC_FIB_3
    mov FIB1, r0    ; Load FIB1 into r0
    add FIB2, r0    ; Add FIB2 (r0 now = FIB1 + FIB2)
    mov r0, FIB3    ; Store result into FIB3
mcroend

mcro CALC_FIB_4
    mov FIB2, r0    ; Load FIB2 into r0
    add FIB3, r0    ; Add FIB3 (r0 now = FIB2 + FIB3)
    mov r0, FIB4    ; Store result into FIB4
mcroend

mcro CALC_FIB_5
    mov FIB3, r0    ; Load FIB3 into r0
    add FIB4, r0    ; Add FIB4 (r0 now = FIB3 + FIB4)
    mov r0, FIB5    ; Store result into FIB5
mcroend

mcro CALC_FIB_6
    mov FIB4, r0    ; Load FIB4 into r0
    add FIB5, r0    ; Add FIB5 (r0 now = FIB4 + FIB5)
    mov r0, FIB6    ; Store result into FIB6
mcroend

mcro CALC_FIB_7
    mov FIB5, r0    ; Load FIB5 into r0
    add FIB6, r0    ; Add FIB6 (r0 now = FIB5 + FIB6)
    mov r0, FIB7    ; Store result into FIB7
mcroend

mcro CALC_FIB_8
    mov FIB6, r0    ; Load FIB6 into r0
    add FIB7, r0    ; Add FIB7 (r0 now = FIB6 + FIB7)
    mov r0, FIB8    ; Store result into FIB8
mcroend

mcro CALC_FIB_9
    mov FIB7, r0    ; Load FIB7 into r0
    add FIB8, r0    ; Add FIB8 (r0 now = FIB7 + FIB8)
    mov r0, FIB9    ; Store result into FIB9
mcroend

;------------------------------------------------------------
; Code Section: Compute and Print the Fibonacci Numbers
;------------------------------------------------------------
MAIN:
    ; Compute the Fibonacci numbers one by one
    CALC_FIB_2
    CALC_FIB_3
    CALC_FIB_4
    CALC_FIB_5
    CALC_FIB_6
    CALC_FIB_7
    CALC_FIB_8
    CALC_FIB_9

    ; Print the Fibonacci numbers stored in memory
    prn FIB0
    prn FIB1
    prn FIB2
    prn FIB3
    prn FIB4
    prn FIB5
    prn FIB6
    prn FIB7
    prn FIB8
    prn FIB9

    stop         ; End of program
