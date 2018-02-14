//Progect: Clock at AT90S1200(CC)
//F_CPU: 8MHz

.include "1200def.inc"

.def temp = r16
.def del1 = r17
.def count = r18
.def seco1 = r19
.def razr2 = r20
.def razr3 = r21
.def indtemp = r22
.def razr4 = r23
.def razr1 = r24
.def seco2 = r25
.def flag = r26
.def ind1 = r27
.def flag1 = r28

.dseg
.cseg

rjmp Reset
reti
.org $002 
rjmp TIM0_OVF // Timer0 Overflow Handler

Reset:

ldi seco1, 0
ldi seco2, 0
ldi razr1, 0
ldi razr2, 0
ldi razr4, 0
ldi razr3, 0
ldi ind1, 0

ldi temp, 0xff 
out DDRB, temp //PORTB 0:7 -> output
ldi temp, 0b0001111
out DDRD, temp //PORTD 0:3 -> output; 4:6 -> input

ldi temp, 0b00000010
out TIMSK, temp
out TIFR, temp

ldi temp, 6//0x83
out TCNT0, temp
 
ldi temp, 0b00000100 // prescaler 256
out TCCR0, temp

sei

//"Array" of data for indication
ldi temp, 63
mov r0, temp
ldi temp, 6
mov r1, temp
ldi temp, 91
mov r2, temp
ldi temp, 79
mov r3, temp
ldi temp, 102
mov r4, temp
ldi temp, 109
mov r5, temp
ldi temp, 125
mov r6, temp
ldi temp, 7
mov r7, temp
ldi temp, 127
mov r8, temp
ldi temp, 111
mov r9, temp


//////////////////////////////////////////
////////********MAIN********//////////////
//////////////////////////////////////////
Main:

sbis PIND, 6//Inc minutes
rcall but11

sbis PIND, 5//Inc hours
rcall but23


ldi temp, 0b1110
out PORTD, temp
//mov indtemp, razr3
mov indtemp, razr1
rcall Ind



ldi temp, 0b1101
out PORTD, temp
//mov indtemp, razr4
mov indtemp, razr2
rcall Ind


ldi temp, 0b0111
out PORTD, temp
//mov indtemp, seco2
mov indtemp, razr3
rcall Ind


ldi temp, 0b1011
out PORTD, temp
//mov indtemp, seco1
mov indtemp, razr4
rcall Ind


rjmp Main
//////////////////////////////////////////


////////////////////////////////////////
/////////******INTERRUPT******//////////
///////////////////////////////////////
TIM0_OVF:
cli 

inc count

/////////////
cpi count, 125//250
breq f
/////////////
rjmp output
//............................................//

f:
ldi flag, 1
cpi flag1, 0
breq f1
ldi flag1, 0
rjmp umn

f1:
ldi flag1, 128

umn:
ldi count, 0
inc seco1
cpi seco1, 10
breq umn11
rjmp output

umn11:
ldi flag, 0
ldi seco1, 0
inc seco2
cpi seco2, 6
breq umn1
rjmp output

umn1:
ldi seco1, 0
ldi seco2, 0
inc razr4
cpi razr4, 10
breq umn2
rjmp output

umn2:
ldi razr4, 0
inc razr3
cpi razr3, 6
breq umn3
rjmp output

umn3:
ldi razr3, 0
inc razr2
cpi razr2, 10
breq umn4
cpi razr2, 4
breq umn41
rjmp output

umn4:
ldi razr2, 0
inc razr1
cpi razr1, 2
breq umn5
rjmp output

umn41:
cpi razr1, 2
breq umn5
rjmp output

umn5:
cpi razr2, 4
breq umn6
rjmp output

umn6:
ldi razr2, 0
ldi razr1, 0

output:
//////////////
ldi temp, 6//0x83
//////////////
out TCNT0, temp

output1:
sei
reti
////////////////////////////////////////////////////



////////////////////////////////////////////////////
///////////////*****BUTTON CLICK*****///////////////
////////////////////////////////////////////////////

but11:
cpi flag, 0
breq butout
ldi count, 0
ldi flag, 0
ldi seco1, 0
ldi seco2, 0
inc razr4
cpi razr4, 10
breq but12
rjmp butout

but12:
ldi razr4, 0
inc razr3
cpi razr3, 6
breq but13
rjmp butout

but13:
ldi razr3, 0
rjmp butout

but23:
cpi flag, 0
breq butout
ldi count, 0
ldi flag, 0
ldi seco1, 0
ldi seco2, 0
inc razr2
cpi razr2, 10
breq but14
cpi razr2, 4
breq but151
rjmp butout

but14:
ldi razr2, 0
inc razr1
cpi razr1, 2
breq but15
rjmp butout

but15:
cpi razr2, 4
breq but16
rjmp butout

but151:
cpi razr1, 2
breq but16
rjmp butout

but16:
ldi razr2, 0
ldi razr1, 0

butout:
ret

//////////////////////////////////////////


//////////////////////////////////////////
////////////***INDICATION***/////////////
////////////////////////////////////////

Ind:

add ZL, indtemp
ld temp, Z
ldi ZL, 0
OR temp, flag1
out PORTB, temp
rcall Delay

ldi temp, 0
or temp, flag1
out PORTB, temp

ret


//////////////////////////////////
///////*****DELAY*****///////////
/////////////////////////////////

Delay:
ldi del1, 255//5

PDelay:
dec del1
brne PDelay

ret
//////////////////////////////////



