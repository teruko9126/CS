@256
D=A
@SP
M=D
@1
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@4
M=D
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@THAT
A=M+D
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@THAT
A=M+D
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M-D
@SP
M=M+1
@0
D=A
@ARG
A=M+D
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
($MAIN_LOOP_START)
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@$COMPUTE_ELEMENT
D;JNE
@$END_PROGRAM
D;JMP
($COMPUTE_ELEMENT)
@0
D=A
@THAT
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@THAT
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M+D
@SP
M=M+1
@2
D=A
@THAT
A=M+D
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@4
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M+D
@SP
M=M+1
@SP
AM=M-1
D=M
@4
M=D
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M-D
@SP
M=M+1
@0
D=A
@ARG
A=M+D
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@$MAIN_LOOP_START
D;JMP
($END_PROGRAM)
