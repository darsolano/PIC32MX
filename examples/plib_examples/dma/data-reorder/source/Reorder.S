###  assm endian/reordering helpers

#include <xc.h>
#include <cp0defs.h>

.global	Le2BeWords
.global SwapHalves
.global Le2BeHalves


#################  low level routines ##################

.set	noat
.set	noreorder


.section .text, code

# void Le2BeWords(unsigned char* dst, unsigned char* src, int size);
# assembles words from source, changes endianness, and stores the result to the destination
# source and dest can be un-aligned
# size is always 4 multiple!
.ent Le2BeWords
Le2BeWords:	
1:
	lwr	t0, 0(a1);
	lwl	t0, 3(a1);
	;# got word
	wsbh	t0, t0
	rotr	t0, t0, 16;	# LE<->BE
	swr	t0, 0(a0)
	swl	t0, 3(a0)
	;# stored to destination
	addiu	a0, a0, 4;
	addiu	a2, a2, -4;
	bne	a2, $0, 1b;
	addiu	a1, a1, 4;
	;# done
	jr	ra;
	nop;
.end Le2BeWords

# void SwapHalves(unsigned char* dst, unsigned char* src, int size);
# assembles words from source, swaps halves, and stores the result to the destination
# source and dest can be un-aligned
# size is always 4 multiple!
.ent SwapHalves
SwapHalves:	
1:	
	lwr	t0, 0(a1);
	lwl	t0, 3(a1);
	;# got word
	;# wsbh	t0, t0
	rotr	t0, t0, 16;	# LE<->BE
	swr	t0, 0(a0)
	swl	t0, 3(a0)
	;# stored to destination
	addiu	a0, a0, 4;
	addiu	a2, a2, -4;
	bne	a2, $0, 1b;
	addiu	a1, a1, 4;
	;# done
	jr	ra;
	nop;
.end SwapHalves

# void Le2BeHalves(unsigned char* dst, unsigned char* src, int size);
# assembles words from source, swaps bytes within halves, and stores the result to the destination
# source and dest can be un-aligned
# size is always 4 multiple!
.ent Le2BeHalves
Le2BeHalves:	
1:	
	lwr	t0, 0(a1);
	lwl	t0, 3(a1);
	;# got word
	wsbh	t0, t0
	;# rotr	t0, t0, 16;	# LE<->BE
	swr	t0, 0(a0)
	swl	t0, 3(a0)
	;# stored to destination
	addiu	a0, a0, 4;
	addiu	a2, a2, -4;
	bne	a2, $0, 1b;
	addiu	a1, a1, 4;
	;# done
	jr	ra;
	nop;
.end Le2BeHalves


.set	at
.set	reorder


