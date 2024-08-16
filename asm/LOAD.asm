        .ORIG 1000
        LD R7, MEM1
        ST R7, MEM2
        LD R6, MEM2
        MEM1 .FILL x0023         ; 35 in decimal
        MEM2 .BLKW 1
        .END

