.section data

.equ tamara, +10+0xA - 15 + a #12 = 0x0c
.equ marko, +0xA + 9 - b #12 = 0x0c
.equ pavle, 20 - 7 + c #118 = 0x76
.equ jovana, +0xAB + 6 -d# = 170 = 0xaa


.global k
.equ r, 0x78
.equ k, 7
 .word 10
 .word 5
 .word 0x9
 .word -0x0008
.equ b, 7
.equ c, r - 15

.section text

.equ a, k
.equ d, a

.byte tamara, marko, pavle, jovana, r, k, b, c, a, d

.end
