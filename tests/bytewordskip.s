.global c
.extern f
.section text
.byte a
.byte 5
.byte c, f
.byte 5, 6
.word 8, 5
.word a, c
.skip 10
x: .byte r



.section data
.byte 5,c
a: .byte 0x20, -0x05
b: .byte 0x5, -1, a
c: .byte c

.section text

r: .skip 0x000A

.end
