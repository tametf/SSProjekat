.extern e

.section sekcija

mov b,%r1
movw c,x
movb e,a(%r1)
sub c, b(%r5)
movb $a, a
movb $b, b

x: jmp *x
jne *2
jmp *c

d: movw $-5,d

.section data
a: .byte 1
b: .word 2

.global c
c: 
.end