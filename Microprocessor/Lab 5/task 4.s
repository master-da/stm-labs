	AREA NOKI, CODE, READONLY
    ENTRY
;	EXPORT main
;	EXPORT smolest
;		
;arr DCDU 0xFF0000, 0xFF000000, 0xFF00, 0xFF

;main
;	LDR r1, =arr	; declare r0 to hold address to arr
;	MOV r2, #0		; r2 is the index of array
;	MOV r0, #0xFFFFFFFF		; smolest r0 = 0xFFFFFFFF
;	BL smolest
;	
;Stop B Stop
;	
;smolest
;	LDR r3, [r1, r2, LSL #2] 	;r3 = arr[i]
;	CMP r3, r0					
;	MOVLS r0, r3	;if r3 > r0, r0 = r3
;	ADD r2, #1					;i++
;	CMP r2, #4					
;	BLE smolest					;if i != 4, return to largest
;	BX LR
;	
;	END
