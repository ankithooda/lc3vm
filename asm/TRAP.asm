        .ORIG 3000
        ADD  R1, R1, 10
        ADD  R1, R1, 10
        ADD  R1, R1, 10

        LEA  R4, TRAP_IN
        STR  R4, R1, 5
        TRAP x23                        ; 35
        TRAP_IN .FILL x1025          ; Add 5 to R
        .END
