MAIN:   add r3, LIST
LOOP:   prn #48

        mcro a_mc
        cmp K, #-6
        bne &END
        mcroend

        mcro b_mc
        mcroend

        lea STR, r6
        inc r6
        mov r3, K
        sub r1, r4
        bne END
        a_mc
        b_mc
        dec K
        jmp &LOOP
END:    stop
STR:    .string "abcd"
LIST:   .data 6, -9
        .data -100
K:      .data 31