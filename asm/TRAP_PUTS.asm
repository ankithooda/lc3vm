        .ORIG x1234
        LEA R0, MSG
        TRAP x22
        MSG .STRINGZ "Hello World"
        .END
