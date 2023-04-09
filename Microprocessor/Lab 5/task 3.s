	AREA NOKI, CODE, READONLY
    ENTRY
;	EXPORT main
;	EXPORT adder
;		
;main

;	MOV r1, #1	;values in r1 and r2 to be restored upon function return
;	MOV r2, #2
;	
;	BL adder
;	MOV r0, r3
;	
;Stop B Stop
;	
;adder
;	ADD r1, r2
;	MOV r3, r1
;	BX LR

;	END