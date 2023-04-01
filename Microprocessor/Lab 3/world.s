
AREA noki,CODE,READONLY
	ENTRY
	EXPORT main
	EXPORT sub
	EXPORT moresub
	EXPORT ml
X EQU 9
Y EQU 8
Z EQU 58
main
	MOV r1, #2
	MOV r2, #4
	MOV r3, #5
	ADD r0,r1,r2
	ADD r0, r0, r3
sub
	MOV r1, #4
	MOV r2, #2
	SUB r0, r1, r2
moresub
	MOV r1, #12
	MOV r2, #4
	MOV r3, #5
	SUB r0, r1, r2
	SUB r0, r0, r3
ml
	MOV r1, #12
	MOV r2, #4
	MUL r0, r1, r2
addconstants
	MOV r4, #X
	MOV r3, #Y
	MOV r2, #Z
	ADD r0, r4, r3
	ADD r0, r0, r2

STOP B STOP
;S_1 DCB "Hello World",0
	END
	