	format zx81

	// hardware options to be set and change defaults in ZX81DEF.INC
	MEMAVL	   =	   MEM_16K	   // can be MEM_1K, MEM_2K, MEM_4K, MEM_8K, MEM_16K, MEM_32K, MEM_48K
	STARTMODE  EQU	   SLOW_MODE	   // SLOW or FAST
	DFILETYPE  EQU	   AUTO 	   // COLLAPSED or EXPANDED or AUTO
	include 'SINCL-ZX\ZX81.INC'

	AUTOLINE 10

	REM _hide _asm
speak1:
	ld	c,$87
	ld	a,(16446)
	ld	b,a
	ld	hl,32768
speak:
	in	a,(c)
	and	1
	jr	z,speak

	ld	a,(hl)
	out	(c),a
	inc	hl
	djnz	speak
	xor	a
	out	(c),a
	ret

status:
	ld	bc,$e007
	ld	a,$c5
	out	(c),a
	ex	(sp),hl
	ex	(sp),hl
	ex	(sp),hl
	ex	(sp),hl
	in	a,(c)
	ld	c,a
	ld	b,0
	ret
	END _asm

//             --------========--------========
	LPRINT "OPE SER 9600"
	PRINT "SPEAK-N-S...ERIAL AT 9600,8,N,1"

AGAIN:	PRINT
	PRINT "WAITING FOR DATA"

WAITER: LET S = USR #status
	IF S = 0 THEN GOTO #WAITER#

	PRINT "READY, PRESS A KEY."
WKEY:	LET A$ = INKEY$
	IF A$ = "" THEN GOTO #WKEY#

	LPRINT "GET SER *32768"

RPEAT:	CLS
	RAND USR #speak1
	PRINT "SPOKE ";PEEK 16446;" ALLOPHONES."
	GOTO #AGAIN#


include 'SINCL-ZX\ZX81DISP.INC'   ;include D_FILE and needed memory areas

VARS_ADDR:
	db 80h
WORKSPACE:

assert ($-MEMST)<MEMAVL
// end of program
