B start

SYMBOLS		DEFB "abcdefghijklmnopqrstuvwxyz ,0123456789\0"
INPUT		DEFB "a ,9h,9hthax9ah9a8,4z\0"
ALIGN

; calculate the length of a null-terminated string
; Arguments:
;	r0: address to the string
; Return
; r0: length of the string
strlen
	MOV 	r1, #0
	_L1
		LDRB 	r2, [r0,r1]
		CMP 	r2, #0
		BEQ 	_strlen_return
		ADD 	r1, r1, #1
		B 		_L1
_strlen_return
	MOV 	r0, r1
	MOV		PC, LR

divide  
	MOV 	R3, R1
    CMP 	R3, R0 LSR #1

	dl1     
		MOVLS 	R3, R3 LSL #1
        CMP 	R3, R0 LSR #1
        BLS 	dl1
    PUSH 	{R4}
    MOV 	R4, #0

	dl2     
		CMP R0, R3
        SUBCS R0, R0, R3
        ADC R4,R4,R4
        MOV R3, R3 LSR #1
        CMP R3, R1
        BHS dl2
     
     STR 	R0, [r2]
     MOV 	R0, R4
     POP	{R4}
     MOV 	PC, LR

start
	MOV 	SP, #0x10000 ; stack address initialisation
	MOV		FP, SP		 ; frame pointer set to the base of stack

	ADRL 	r0, SYMBOLS
	BL 		strlen
	PUSH 	{r0}		; [FP-4] = length of symbols

	MOV 	r0, r0 LSR #1 ; divide strlen(symbols) by 2 to get the b_param of the formula
	PUSH {r0}	;[FP-8] = b_param

	ADRL 	r0, INPUT
	BL 		strlen
	PUSH 	{r0}		; [FP-12] = length of input

	;allocate OUTPUT string on stack (with alignment)
	ADD 	r0, r0, #1
	MOV 	r1, #3     ; 4-byte alignment
	ADD 	r0, r0, r1
	BIC 	r0, r0, r1

	PUSH 	{r0} ; length of output string (padded) [fp-16]


	SUB		sp, sp, r0	

	MOV		r1, #0
	MOV 	r2, #0

	;memset(output, 0, r0+1)
	_L2
		STRB 	r2, [sp,r1]
		ADD 	r1,r1,#1
		CMP 	r1, r0
		BEQ 	_END_L2
		B 		_L2
	_END_L2

	MOV 	r4, sp        ; r4 contains the address of the output string
	MOV		r5, #0        ; r5 counter outer loop
	ADRL	r6, SYMBOLS   ; r6 = address of SYMBOL
	LDR		r8, [fp,#-12] ; length of input string
	ADRL	r9, INPUT     ; r9 = address of INPUT

	SUB 	SP,SP, #4 ;allocate another variable on stack for the modulo

	_L3
		MOV		r0, #0 ; k = index of output char in symbols
		LDRB	r1, [r9, r5]
		LDR 	r2, [fp, #04]

		_L3_1 			; find i-th input char in SYMBOL
			LDRB	r2,	[r6, r0]
			CMP		r1, r2
			BEQ 	_END_L3_1
			ADD 	r0, r0, #1
			CMP		r0,	r2
			BLT     _L3_1
		_END_L3_1

		MOV		r1, #0

		_L3_2
			PUSH	{r1}
			LDR 	r1, [fp,#-8]
			ADD 	r0, r0, r1
			LDR 	r1, [fp, #-4]
			ADD		r2,	sp,	#4

			BL 		divide
			LDR 	r0, [r2]


			POP		{r1}
			ADD 	r1, r1, #1
			CMP		r1, #3
			BLT 	_L3_2

		LDRB 	r2, [r6,r0]
		STRB	r2,	[r4,r5]


		ADD 	r5, r5, #1
		CMP		r5, r8
		BLT		_L3

	; print result
	MOV 	r0, r4
	SWI 	3

	; restore stack
	LDR 	R0,[fp,#-16]
	ADD 	SP, SP, r0
	ADD 	SP, SP, #20

	SWI 	2
