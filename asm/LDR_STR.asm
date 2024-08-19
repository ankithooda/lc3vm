        .ORIG 1000
        ADD R0, R0, 3
        ADD R1, R1, 8
        STR R1, R0, 11
        LDR R3, R0, 11
        .END
