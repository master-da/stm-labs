	AREA NOKI, CODE, READONLY
    ENTRY
;	EXPORT main
;	EXPORT adder
;		
;main

;	MOV r1, #0xFFFF	;values in r1 and r2 to be restored upon function return
;	MOV r2, #0xFFFF
;	MOV r5, #1		;values in r5, r6 to be added
;	MOV r6, #2
;	
;	
;	PUSH {r1, r2}
;	MOV r1, r5
;	MOV r2, r6
;	BL adder
;	POP {r1, r2}
;	MOV r0, r3
;	
;Stop B Stop
;	
;adder
;	ADD r1, r2
;	MOV r3, r1
;	BX LR

;	END