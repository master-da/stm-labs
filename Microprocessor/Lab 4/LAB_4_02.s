	AREA NOKI, CODE, READONLY
	ENTRY
	export main
	export logicalsmol
	export logicalbig
	export shift
	export arithmaticsmol
	export arithmaticbig
	export avginit
	export avg
	export largestinit
	export largest
	export avginitf
	export avgf
		
var_one_16 DCDU 0xFFFCCFFF
var_two_16 DCDU 0x000FF000
arr DCDU 0xFF, 0xFF000000, 0xFF00, 0xFF0000

main
logicalsmol	;Logical Operations for 16bit nubers
	
	LDRH r1, var_one_16
	LDRH r2, var_two_16
	
	AND r3, r1, r2 		; r3 := r1 AND r2
	ORR r4, r1, r2		; r4 := r1 OR r2
	MVN r5, r4			; r5 := r1 NOR r2
	MVN r6, r3			; r6 := r1 NAND r2
	EOR r7, r1, r2		; r7 := r1 XOR r2
	MVN r8, r7			; r8 := r1 XNOR r2
	
logicalbig	;Logical Operations for 32Bit numbers
	MOV r1, #0xFFFCCFFF
	MOV r2, #0x000FF000
	
	AND r3, r1, r2 		; r3 := r1 AND r2
	ORR r4, r1, r2		; r4 := r1 OR r2
	MVN r5, r4			; r5 := r1 NOR r2
	MVN r6, r3			; r6 := r1 NAND r2
	EOR r7, r1, r2		; r7 := r1 XOR r2
	MVN r8, r7			; r8 := r1 XNOR r2


shift	;All shifting operations
	MOV r0, #2147483648 ; this is 0b10000000000000000000000000000000 or 0x80000000
	
	ASR r0, r0, #2	; expected 0b11100000000000000000000000000000 or 0xE0000000
	LSR r0, r0, #1	; expected 0b01110000000000000000000000000000 or 0x70000000
	LSL r0, r0, #1	; expected 0b11100000000000000000000000000000 or 0xE0000000
	ROR r0, r0, #31	; expected 0b11000000000000000000000000000001 or 0xC0000001
	
arithmaticsmol	;Arighmatic Operations for small values restricting overflow
	MOV r0, #3
	MOV r1, #2
	
	ADD r2, r1, r0 ; r2 = r1 + r0
	SUB r3, r0, r1 ; r3 = r0 - r1
	MUL r4, r0, r1 ; r4 = r0 * r1
	UDIV r5, r0, r1 ; r5 = r0 / r1
arithmaticbig	;Arithmatic Operations for large values handling overflow
	MOV r0, #0xFFFFFFFF
	MOV r1, #0x00FF0000
	
	ADDS r2, r1, r0 ; r2 = r1 + r0
	SUBS r3, r0, r1 ; r3 = r0 - r1
	UMULL r5, r4, r0, r1 ; r5,r4 = r0 * r1
	UDIV r6, r0, r1 ; r6 = r0 / r1
	
avginit	;Average of N numbers
	LDR r1, =arr	; declare r0 to hold address to arr
	MOV r2, #0		; r2 is the index of array
	MOV r0, #0		; sum r0 = 0
avg
	LDR r3, [r1, r2, LSL #2]	;r3 = arr[i]
	ADD r0, r3					;r0 = r0 + r3
	ADD r2, #1					;r2++
	MOV r3, r2					;r3 = r2
	SUBS r3, #4					;r3 = r3 - 3
	BNE avg					; return to avgb if r3 != 3
	UDIV r0, r2					;r0 = r0 / 4
	
largestinit	;Largest of N numbers
	LDR r1, =arr	; declare r0 to hold address to arr
	MOV r2, #0		; r2 is the index of array
	MOV r0, #0		; largest r0 = 0
largest
	LDR r3, [r1, r2, LSL #2] 	;r3 = arr[i]
	SUBS r3, r0					;r3 = r3 - r0
	LDRCS r0, [r1, r2, LSL #2]	;r0 = max(r0, arr[i])
	ADD r2, #1					;i++
	MOV r3, r2					;r3 = i
	SUBS r3, #4					;r3 = r3 - 4
	BNE largest					;if r3 != 4, return to largest
	
avginitf	;Average of N numbers using function call
	LDR r1, =arr	; declare r0 to hold address to arr
	MOV r2, #0		; r2 is the index of array
	MOV r0, #0		; sum r0 = 0
	BL avgf
	MOV r0, #0xCC00	;testing return from function
avgf
	LDR r3, [r1, r2, LSL #2]	;r3 = arr[i]
	ADD r0, r3					;r0 = r0 + r3
	ADD r2, #1					;r2++
	MOV r3, r2					;r3 = r2
	SUBS r3, #4					;r3 = r3 - 3
	BNE avgf					;return to avgb if r3 != 3
	UDIV r0, r2					;r0 = r0 / 4
	BX LR						;return to function calling point

	
STOP B STOP
	END