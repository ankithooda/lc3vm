        .ORIG 1233
        JSR LOOPA
EXIT
        LEA R0, MSG2
        PUTS
        HALT
LOOPA
        LD R2, Y_PRESS
        LEA R0, MSG1
        PUTS
        LDI R0, KBSR
        BRn EXIT
        BRzp LOOPA

        ;;  Data
        MSG1 .STRINGZ "Press any other key to exit\n"
        MSG2 .STRINGZ "Thanks for pressing a button\n"
        KBSR .FILL xFE00
        KBDR .FILL xFE02
        Y_PRESS .FILL x0059
        .END
