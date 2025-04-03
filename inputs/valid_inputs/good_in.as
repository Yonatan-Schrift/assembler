MACRO1: .macro
mov R1, R2
add #5, R3
sub R4, R5

MACRO1

.data 5, 10, 15, 20, 25, 30
.data -5, -10, -15
.string "Hello, World!"
.string "Test Input"

LABEL1: mov R3, R4
add R1, LABEL1
LABEL2: sub R2, R6
jmp LABEL2

.extern EXTERNAL_LABEL
.entry ENTRY_LABEL

mov EXTERNAL_LABEL, R5
add EXTERNAL_LABEL, R6

ENTRY_LABEL: sub R2, R6
red R7
prn #42

clr R1
not R2
inc R3
dec R4
jmp LABEL1
bne LABEL2
jsr ENTRY_LABEL

.data 100, 200, 300, 400
.string "End of Valid Input"

stop