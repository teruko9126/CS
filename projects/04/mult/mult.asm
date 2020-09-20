// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

//M:Aに入っている値をメモリとして見たときの中身
//D:値
@2
M = 0
@3
M = 0
@4
M = 0
@5
M = 0
@0
D = M
@3
M = D //R3 = R0
@4
M = 1 //R4 = 1
@5
M = 0

(loop)
@4
D = M
@0
M = M&D //D = R0 & R4(R0の1桁目が1かどうか確認している)
@4
D = M
D = M+D
M = D
@0
D = M

@notloop
D;JEQ

@1
D = M
@2
M = M+D

(notloop)
@1
D = M
D = M+D
M = D
@3
D = M
@0
M = D
@5
M = M+1
D = M
@20
D = D - A

@loop
D;JLE

(END)
@END
0;JMP