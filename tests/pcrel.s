.section data
.global d
.extern c# bvjbhjv
.global x#ffj

#fffdfd
a: .byte 0x0002551
b: .word -0x54
d: .word 80


.section text
y: .byte d
x: .byte 0x6
z: .byte 8

.section data

p: movw c(%pc),y 
t: jmp *t(%pc) 
tamara: xchg a, b
jne *d
sub a(%pc), 5
mov $3, (%r6)

.end


Izlaz testa pcrel.s:
