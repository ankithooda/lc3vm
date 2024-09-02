        .ORIG 1233
        
MAIN
        JSR FUNC1
        LEA R0, MAIN_MSG
        PUTS


FUNC1
        LEA R0, HELLO_MSG
        PUTS
        RET
        HELLO_MSG .STRINGZ "Hello FUNC\n"
        MAIN_MSG .STRINGZ "Hello Main\n"

        .END
