        .ORIG 3000
        ADD R0, R0, 13
        STI R0, MEM1
        LDI R1, MEM1
        MEM1 .FILL x4321        ; 17185 in Decimal
        .END
