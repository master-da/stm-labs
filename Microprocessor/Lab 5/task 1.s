	AREA NOKI, CODE, READONLY
    ENTRY
;	EXPORT main
;	EXPORT prime
;	EXPORT is_prime_init
;	EXPORT is_prime_check
;	EXPORT end_check

;arr DCD 2, 3, 4, 5, 6

;main
;	LDR r1, =arr	;r1 := arr[0]
;	MOV r2, #0		;r2 = i = 0
;	
;	BL prime
;	MOV r0, #0xFFFFFFFF

;Stop B Stop
;	
;prime
;	LDR r3, [r1, r2, LSL #2]	;r3 = arr[i]
;	MOV r4, #2					;r4 = 2, to iterate from 2 to r3 to check if number is prime
;	MOV r5, #1					;r5 = isPrimeFlag = true
;	
;	PUSH {LR}
;	BL is_prime_init			;check if prime
;	POP {LR}
;	CMP r5, #1
;	MOVEQ r0, r3
;	MOVNE r0, #0xFFFF
;	ADD r2, #1
;	CMP r2, #5					;if i >= 4, exit out of loop
;	BLT prime					;else move onto next number
;	BX LR
;	
;is_prime_init
;	MOV r6, #2					;r6 = j = 2 to start loopin
;	CMP r3, #2;
;	BGT is_prime_check			;if arr[i] <= 2, number prime else check
;	BX LR						
;	
;is_prime_check
;	UDIV r7, r3, r6				;r7 = quotient = arr[i] / j
;	MUL r7, r6					;quotient = quotient * j
;	CMP r3, r7
;	MOVEQ r5, #0				;if arr[i] == quotient * j, there is no remainder. arr[i] is composite
;	BEQ end_check
;	ADD r6, #1					;j++
;	MUL r7, r6, r6				;r7 = j * j
;	CMP r7, r3
;	BGT end_check				;if j * j > arr[i], prime check complete
;	B is_prime_check
;	
;end_check
;	BX LR
;	
;	END
