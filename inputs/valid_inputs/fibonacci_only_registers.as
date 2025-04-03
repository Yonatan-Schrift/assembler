; Fibonacci !

         ; Initialize registers:
         ; r1 will hold the current Fibonacci number (fib0)
         ; r2 will hold the next Fibonacci number (fib1)
         ; r3 is used as a temporary register for the sum
         ; r4 will serve as our loop counter (number of terms)
         
         mov   #0, r1      ; r1 = 0 (first Fibonacci number)
         mov   #1, r2      ; r2 = 1 (second Fibonacci number)
         mov   #10, r4     ; r4 = 10 (we want to print 10 numbers)

LOOP:    prn   r1        ; print current Fibonacci number in r1

         ; Calculate next Fibonacci number:
         mov   r1, r3    ; copy current number (r1) into r3
         add   r2, r3    ; add the next number (r2) to r3 => r3 = r1 + r2
         mov   r2, r1    ; update r1: old r2 becomes new current number
         mov   r3, r2    ; update r2: new Fibonacci number is in r3

         dec   r4        ; decrement our counter
         cmp   r4, #0    ; compare counter with 0
         bne   LOOP      ; if r4 is not equal to 0, repeat the loop

         stop           ; end program
