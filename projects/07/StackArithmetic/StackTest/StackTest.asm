@256
D=A
@SP
M=D
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
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
D=M
@ZERO0
D;JEQ
@SP
A=M
M=1
(ZERO0)
@SP
A=M
M=M-1
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@16
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
D=M
@ZERO1
D;JEQ
@SP
A=M
M=1
(ZERO1)
@SP
A=M
M=M-1
@SP
M=M+1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
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
D=M
@ZERO2
D;JEQ
@SP
A=M
M=1
(ZERO2)
@SP
A=M
M=M-1
@SP
M=M+1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
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
M=D-M
D=M
@LESSTHAN3
D;JGT
@SP
A=M
M=0
(LESSTHAN3)
@SP
A=M
D=M
@LTZERO3
D;JEQ
@SP
A=M
M=-1
(LTZERO3)
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@892
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
M=D-M
D=M
@LESSTHAN4
D;JGT
@SP
A=M
M=0
(LESSTHAN4)
@SP
A=M
D=M
@LTZERO4
D;JEQ
@SP
A=M
M=-1
(LTZERO4)
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
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
M=D-M
D=M
@LESSTHAN5
D;JGT
@SP
A=M
M=0
(LESSTHAN5)
@SP
A=M
D=M
@LTZERO5
D;JEQ
@SP
A=M
M=-1
(LTZERO5)
@SP
M=M+1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
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
D=M
@GREATERTHAN6
D;JGT
@SP
A=M
M=0
(GREATERTHAN6)
@SP
A=M
D=M
@GTZERO6
D;JEQ
@SP
A=M
M=-1
(GTZERO6)
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32767
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
D=M
@GREATERTHAN7
D;JGT
@SP
A=M
M=0
(GREATERTHAN7)
@SP
A=M
D=M
@GTZERO7
D;JEQ
@SP
A=M
M=-1
(GTZERO7)
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
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
D=M
@GREATERTHAN8
D;JGT
@SP
A=M
M=0
(GREATERTHAN8)
@SP
A=M
D=M
@GTZERO8
D;JEQ
@SP
A=M
M=-1
(GTZERO8)
@SP
M=M+1
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
@53
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
@112
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
@SP
AM=M-1
M=-M
@SP
M=M+1
@SP
AM=M-1
D=M
@SP
AM=M-1
M=M&D
@SP
M=M+1
@82
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
M=M|D
@SP
M=M+1
@SP
AM=M-1
M=!M
@SP
M=M+1