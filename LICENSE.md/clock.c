.include "1200def.inc"

.def temp = r16
.def del1 = r17
.def flag1 = r19
.def count = r20
.def seco1 = r21
.def razr2 = r22
.def razr3 = r23
.def indtemp = r24
.def razr4 = r25
.def razr1 = r26
.def seco2 = r18
.def flag = r28
.def addr = r30

.dseg

.cseg
.org 0



rjmp Reset
reti
.org $002 
rjmp TIM0_OVF // Timer0 Overflow Handler


.org $004
//store ind codes here
ind_table:
.db 63, 6, 91, 79, 102, 109, 125, 7, 127, 111

Reset:

//if registers initialized with 0 - can be deleted
//ldi seco1, 0
//ldi seco2, 0
//ldi razr1, 0
//ldi razr2, 0
//ldi razr4, 0
//ldi razr3, 0
//ldi flag, 1
//ldi flag1, 0

ldi flag, 1

// PB0-PB7 as output
ldi temp, 0xff
out DDRB, temp
//PD0-PD3 as output, PD4-PD7 as inputs
ldi temp, 0b0001111
out DDRD, temp
//set PD3
ldi temp, 0b0001000
out PORTD, temp

//enabling tim0 OVF interrupt
ldi temp, 0b00000010
out TIMSK, temp
//seems to be unnecessary
//out TIFR, temp

//unnecessary for me, initialized with 0, upcounter timer
//ldi temp, 0x83//0xc0
//out TCNT0, temp
 
ldi temp, 0b00000100// prescaler 256 -> 32768/256 = 128 times per second (7bit)
out TCCR0, temp

sei
//////////////////////////////////////////
////////********MAIN********//////////////
//////////////////////////////////////////
Main:

sbis PIND, 6
rcall but11

sbis PIND, 5
rcall but23

ldi temp, 0b1110
out PORTD, temp
//mov indtemp, razr3
mov indtemp, razr1
rcall Ind
rcall Delay
rcall nul

ldi temp, 0b1101
out PORTD, temp
//mov indtemp, razr4
mov indtemp, razr2
rcall Ind
rcall Delay
rcall nul

ldi temp, 0b0111
out PORTD, temp
//mov indtemp, seco1
mov indtemp, razr3
rcall Ind
rcall Delay
rcall nul

ldi temp, 0b1011
out PORTD, temp
//mov indtemp, seco1
mov indtemp, razr4
rcall Ind
rcall Delay
rcall nul

rjmp Main
//////////////////////////////////////////

nul:
ldi temp, 0x00
OR temp, flag1
out PORTB, temp
ret

////////////////////////////////////////
/////////******INTERRUPT******//////////
///////////////////////////////////////
TIM0_OVF:
cli 

inc count

cpi count, 125//1
breq nul1

/////////////
cpi count, 250//2
breq f
/////////////

rjmp output
//............................................//
nul1:
ldi flag, 1
rjmp output

f:
cpi flag1, 0 
breq f1
ldi flag1, 0 //replace with CLR (no need to store immediate, smaller instruction?)
rjmp umn

f1:
ldi flag1, 128

umn:
ldi count, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc seco1
cpi seco1, 10
breq umn11
rjmp output

umn11:
ldi flag1, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi seco1, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc seco2
cpi seco2, 6
breq umn1
rjmp output

umn1:
ldi seco1, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi seco2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc razr4
cpi razr4, 10
breq umn2
rjmp output

umn2:
ldi razr4, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc razr3
cpi razr3, 6
breq umn3
rjmp output

umn3:
ldi razr3, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc razr2
cpi razr2, 10
breq umn4
cpi razr2, 4
breq umn41
rjmp output

umn4:
ldi razr2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc razr1
cpi razr1, 2
breq umn5
rjmp output

umn41:
cpi razr1, 2
breq umn5
rjmp butout

umn5:
cpi razr2, 4
breq umn6
rjmp output

umn6:
ldi seco1, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi seco2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi razr4, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi razr3, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi razr2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi razr1, 0 //replace with CLR (no need to store immediate, smaller instruction?)

output:
//////////////
ldi temp, 0x83//0xc0
//////////////
out TCNT0, temp
sei
reti
////////////////////////////////////////////////////



////////////////////////////////////////////////////
///////////////*****BUTTON CLICK*****///////////////
////////////////////////////////////////////////////
but11:
cpi flag, 0  //TST for zero??
breq butout
ldi count, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi flag, 0  //replace with CLR (no need to store immediate, smaller instruction?)
ldi seco1, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi seco2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc razr4 
cpi razr4, 10
breq but12
rjmp butout

but12:
ldi razr4, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc razr3
cpi razr3, 6
breq but13
rjmp butout

but13:
ldi razr3, 0 //replace with CLR (no need to store immediate, smaller instruction?)
rjmp butout

but23:
cpi flag, 0  //TST for zero??
breq butout
ldi count, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi flag, 0  //replace with CLR (no need to store immediate, smaller instruction?)
ldi seco1, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi seco2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
inc razr2
cpi razr2, 10
breq but14
cpi razr2, 4
breq but151
rjmp butout

but14:
ldi razr2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
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
ldi razr2, 0 //replace with CLR (no need to store immediate, smaller instruction?)
ldi razr1, 0 //replace with CLR (no need to store immediate, smaller instruction?)

butout:
ret
//////////////////////////////////////////


//////////////////////////////////////////
////////////***INDICATION***/////////////
////////////////////////////////////////

//check indtemp for ranges
//here
ldi addr, low(ind_table)
//ldi XH, high(ind_table)
add addr, indtemp
ld temp, addr
OR temp, flag1
out PORTB, temp
ret


// Ind:
// cpi indtemp, 1
// breq Ind1
// cpi indtemp, 2
// breq Ind2
// cpi indtemp, 3
// breq Ind3
// cpi indtemp, 4
// breq Ind4
// cpi indtemp, 5
// breq Ind5
// cpi indtemp, 6
// breq Ind6
// cpi indtemp, 7
// breq Ind7
// cpi indtemp, 8
// breq Ind8
// cpi indtemp, 9
// breq Ind9
// cpi indtemp, 0
// breq Ind0


// Ind1:
// ldi temp, 6
// OR temp, flag1
// out PORTB, temp
// ret
// Ind2:
// ldi temp, 91
// OR temp, flag1
// out PORTB, temp
// ret
// Ind3:
// ldi temp, 79
// OR temp, flag1
// out PORTB, temp
// ret
// Ind4:
// ldi temp, 102
// OR temp, flag1
// out PORTB, temp
// ret
// Ind5:
// ldi temp, 109
// OR temp, flag1
// out PORTB, temp
// ret
// Ind6:
// ldi temp, 125
// OR temp, flag1
// out PORTB, temp
// ret
// Ind7:
// ldi temp, 7
// OR temp, flag1
// out PORTB, temp
// ret
// Ind8:
// ldi temp, 127
// OR temp, flag1
// out PORTB, temp
// ret
// Ind9:
// ldi temp, 111
// OR temp, flag1
// out PORTB, temp
// ret
// Ind0:
// ldi temp, 63
// OR temp, flag1
// out PORTB, temp
// ret

//////////////////////////////////

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





