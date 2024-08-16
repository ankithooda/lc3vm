        .ORIG 1000
        ADD R7, R7, 12
        ST R7, MEM2
        LD R6, MEM2
        MEM1 .FILL x0023         ; 35 in decimal
        MEM2 .BLKW 1
        .END

