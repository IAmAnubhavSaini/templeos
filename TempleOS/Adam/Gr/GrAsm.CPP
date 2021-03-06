#help_index "Graphics"
I64 *gr_1_to_8_bits,*gr_1_to_8_colors;

#help_index "Graphics/Char;Char/Graphics"
U32 *gr_text_base;
U16 *gr_win_z_buf;

asm {
USE64
// ************************************
_PLOT_SCREEN_U8::
	ENTER	0
	MOV	RBX,U64 SF_ARG2[RBP]
	MOV	RAX,U64 SF_ARG3[RBP]
	OR	RBX,RBX
	JS	@@05
	OR	RAX,RAX
	JS	@@05
	CMP	RBX,TEXT_COLS
	JGE	@@05
	CMP	RAX,TEXT_ROWS
	JGE	@@05
	IMUL2	RAX,TEXT_COLS
	ADD	RBX,RAX
	SHL	RBX,2
	ADD	RBX,U64 [&gr_text_base]
	MOV	RAX,U64 SF_ARG1[RBP]
	MOV	U32 [RBX],EAX

	MOV	RAX,1
	LEAVE
	RET1	24
@@05:	XOR	RAX,RAX
	LEAVE
	RET1	24
// ************************************
_PLOT_WIN_U8::
	ENTER	0
	MOV	RCX,U64 SF_ARG1[RBP]
	MOV	RBX,U64 CTask.win_scroll_x[RCX]
	SAR	RBX,3
	ADD	RBX,U64 SF_ARG3[RBP]
	MOV	RAX,U64 CTask.win_scroll_y[RCX]
	SAR	RAX,3
	ADD	RAX,U64 SF_ARG4[RBP]

	OR	RBX,RBX
	JS	@@05
	ADD	RBX,U64 CTask.win_left[RCX]
	CMP	RBX,U64 CTask.win_right[RCX]
	JG	@@05
	OR	RBX,RBX
	JS	@@05
	CMP	RBX,TEXT_COLS
	JGE	@@05

	OR	RAX,RAX
	JS	@@05
	ADD	RAX,U64 CTask.win_top[RCX]
	CMP	RAX,U64 CTask.win_bottom[RCX]
	JG	@@05
	OR	RAX,RAX
	JS	@@05
	CMP	RAX,TEXT_ROWS
	JGE	@@05

	IMUL2	RAX,TEXT_COLS
	ADD	RBX,RAX
	SHL	RBX,2
	ADD	RBX,U64 [&gr_text_base]
	MOV	RAX,U64 SF_ARG2[RBP]
	MOV	U32 [RBX],EAX

	MOV	RAX,1
	LEAVE
	RET1	32
@@05:	XOR	RAX,RAX
	LEAVE
	RET1	32
// ************************************
/*
Bool PlotWinStr2(CTask *task,U8 *s,I64 len,
		   I64 attr,I64 x,I64 y)
*/
_PLOT_WIN_STR2::
	ENTER	0
	MOV	R9,U64 [&gr_text_base]
	PUSH	RSI
	PUSH	RDI
	MOV	RBX,U64 SF_ARG1[RBP]
	MOV	RSI,U64 SF_ARG2[RBP]
	MOV	RDI,U64 CTask.win_scroll_x[RBX]
	SAR	RDI,3
	ADD	RDI,U64 SF_ARG5[RBP]
	MOV	RCX,U64 SF_ARG3[RBP]

	OR	RDI,RDI
	JNS	@@05
	ADD	RCX,RDI
	SUB	RSI,RDI
	XOR	RDI,RDI
@@05:	ADD	RDI,U64 CTask.win_left[RBX]
	MOV	RDX,RCX
	ADD	RDX,RDI
	DEC	RDX
	CMP	RDX,U64 CTask.win_right[RBX]
	JLE	@@10
	MOV	RAX,RDX
	SUB	RAX,U64 CTask.win_right[RBX]
	SUB	RDX,RAX
	SUB	RCX,RAX
@@10:	OR	RDI,RDI
	JNS	@@15
	ADD	RCX,RDI
	SUB	RSI,RDI
	XOR	RDI,RDI
@@15:	INC	RDX
	SUB	RDX,TEXT_COLS
	JLE	@@20
	SUB	RCX,RDX
@@20:	CMP	RCX,1
	JL	@@30

	MOV	RAX,U64 CTask.win_scroll_y[RBX]
	SAR	RAX,3
	ADD	RAX,U64 SF_ARG6[RBP]
	OR	RAX,RAX
	JS	@@30
	ADD	RAX,U64 CTask.win_top[RBX]
	CMP	RAX,U64 CTask.win_bottom[RBX]
	JG	@@30
	OR	RAX,RAX
	JS	@@30
	CMP	RAX,TEXT_ROWS
	JGE	@@30

	IMUL2	RAX,TEXT_COLS
	ADD	RDI,RAX
	SHL	RDI,2
	ADD	RDI,R9
	MOV	RAX,U64 SF_ARG4[RBP]
@@25:	LODSB
	STOSD
	DEC	RCX
	JNZ	@@25

	POP	RDI
	POP	RSI
	MOV	RAX,TRUE
	LEAVE
	RET1	48

@@30:	POP	RDI
	POP	RSI
	XOR	RAX,RAX
	LEAVE
	RET1	48
// ************************************
/*
Bool PlotAttrStr2(CTask *task,U32 *hl,I64 len,I64 x,I64 y)
*/
_PLOT_ATTR_STR2::
	ENTER	0
	MOV	R9,U64 [&gr_text_base]
	PUSH	RSI
	PUSH	RDI
	MOV	RBX,U64 SF_ARG1[RBP]
	MOV	RSI,U64 SF_ARG2[RBP]
	MOV	RDI,U64 CTask.win_scroll_x[RBX]
	SAR	RDI,3
	ADD	RDI,U64 SF_ARG4[RBP]
	MOV	RCX,U64 SF_ARG3[RBP]

	OR	RDI,RDI
	JNS	@@05
	ADD	RCX,RDI
	SHL	RDI,2
	SUB	RSI,RDI
	XOR	RDI,RDI
@@05:	ADD	RDI,U64 CTask.win_left[RBX]
	MOV	RDX,RCX
	ADD	RDX,RDI
	DEC	RDX
	CMP	RDX,U64 CTask.win_right[RBX]
	JLE	@@10
	MOV	RAX,RDX
	SUB	RAX,U64 CTask.win_right[RBX]
	SUB	RDX,RAX
	SUB	RCX,RAX
@@10:	OR	RDI,RDI
	JNS	@@15
	ADD	RCX,RDI
	SHL	RDI,2
	SUB	RSI,RDI
	XOR	RDI,RDI
@@15:	INC	RDX
	SUB	RDX,TEXT_COLS
	JLE	@@20
	SUB	RCX,RDX
@@20:	CMP	RCX,1
	JL	@@30

	MOV	RAX,U64 CTask.win_scroll_y[RBX]
	SAR	RAX,3
	ADD	RAX,U64 SF_ARG5[RBP]
	OR	RAX,RAX
	JS	@@30
	ADD	RAX,U64 CTask.win_top[RBX]
	CMP	RAX,U64 CTask.win_bottom[RBX]
	JG	@@30
	OR	RAX,RAX
	JS	@@30
	CMP	RAX,TEXT_ROWS
	JGE	@@30

	IMUL2	RAX,TEXT_COLS
	ADD	RDI,RAX
	SHL	RDI,2
	ADD	RDI,R9
@@25:	MOVSD
	DEC	RCX
	JNZ	@@25

	POP	RDI
	POP	RSI
	MOV	RAX,TRUE
	LEAVE
	RET1	40

@@30:	POP	RDI
	POP	RSI
	XOR	RAX,RAX
	LEAVE
	RET1	40
// ************************************
/*
Bool PlotAttr(CTask *task,I64 len,
		I64 attr,I64 x,I64 y)
*/
_PLOT_ATTR::
	ENTER	0
	MOV	R9,U64 [&gr_text_base]
	PUSH	RSI
	PUSH	RDI
	MOV	RBX,U64 SF_ARG1[RBP]
	MOV	RDI,U64 CTask.win_scroll_x[RBX]
	SAR	RDI,3
	ADD	RDI,U64 SF_ARG4[RBP]
	MOV	RCX,U64 SF_ARG2[RBP]

	OR	RDI,RDI
	JNS	@@05
	ADD	RCX,RDI
	XOR	RDI,RDI
@@05:	ADD	RDI,U64 CTask.win_left[RBX]
	MOV	RDX,RCX
	ADD	RDX,RDI
	DEC	RDX
	CMP	RDX,U64 CTask.win_right[RBX]
	JLE	@@10
	MOV	RAX,RDX
	SUB	RAX,U64 CTask.win_right[RBX]
	SUB	RDX,RAX
	SUB	RCX,RAX
@@10:	OR	RDI,RDI
	JNS	@@15
	ADD	RCX,RDI
	XOR	RDI,RDI
@@15:	INC	RDX
	SUB	RDX,TEXT_COLS
	JLE	@@20
	SUB	RCX,RDX
@@20:	CMP	RCX,1
	JL	@@35

	MOV	RAX,U64 CTask.win_scroll_y[RBX]
	SAR	RAX,3
	ADD	RAX,U64 SF_ARG5[RBP]
	OR	RAX,RAX
	JS	@@35
	ADD	RAX,U64 CTask.win_top[RBX]
	CMP	RAX,U64 CTask.win_bottom[RBX]
	JG	@@35
	OR	RAX,RAX
	JS	@@35
	CMP	RAX,TEXT_ROWS
	JGE	@@35

	IMUL2	RAX,TEXT_COLS
	ADD	RDI,RAX
	SHL	RDI,2
	ADD	RDI,R9
	MOV	RBX,U64 SF_ARG3[RBP]
	MOV	RSI,RDI
@@25:	LODSD
	OR	AL,AL
	JNZ	@@30
	MOV	RAX,RBX
	STOSD
	DEC	RCX
	JNZ	@@25

@@30:	POP	RDI
	POP	RSI
	MOV	RAX,TRUE
	LEAVE
	RET1	40

@@35:	POP	RDI
	POP	RSI
	XOR	RAX,RAX
	LEAVE
	RET1	40
// ************************************
_PLOT_BORDER_U8::
	ENTER	0
	MOV	R9,U64 [&gr_text_base]
	MOV	RCX,U64 SF_ARG1[RBP]
	MOV	RBX,U64 CTask.win_scroll_x[RCX]
	SAR	RBX,3
	ADD	RBX,U64 SF_ARG3[RBP]
	MOV	RAX,U64 CTask.win_scroll_y[RCX]
	SAR	RAX,3
	ADD	RAX,U64 SF_ARG4[RBP]

	MOV	RDX,-1
	CMP	RBX,RDX
	JL	@@05
	ADD	RBX,U64 CTask.win_left[RCX]
	MOV	RDX,U64 CTask.win_right[RCX]
	INC	RDX
	CMP	RBX,RDX
	JG	@@05
	OR	RBX,RBX
	JS	@@05
	CMP	RBX,TEXT_COLS
	JGE	@@05

	MOV	RDX,-1
	CMP	RAX,RDX
	JL	@@05
	ADD	RAX,U64 CTask.win_top[RCX]
	MOV	RDX,U64 CTask.win_bottom[RCX]
	INC	RDX
	CMP	RAX,RDX
	JG	@@05
	OR	RAX,RAX
	JS	@@05
	CMP	RAX,TEXT_ROWS
	JGE	@@05

	IMUL2	RAX,TEXT_COLS
	ADD	RBX,RAX
	SHL	RBX,2
	ADD	RBX,R9
	MOV	RAX,U64 SF_ARG2[RBP]
	MOV	U32 [RBX],EAX

	MOV	RAX,TRUE
	LEAVE
	RET1	32
@@05:	XOR	RAX,RAX
	LEAVE
	RET1	32
// ************************************
_GR_UPDATE_LINE32::
	ENTER	0
	PUSH	RSI
	PUSH	RDI

	MOV	RAX,U64 SF_ARG2[RBP]
	MOV	RSI,U64 [RAX]	//rsi=src2
	MOV	RBX,U64 [&reverse_bits_table]
	MOV	RCX,U64 SF_ARG3[RBP] //rcx=d5
	MOV	RAX,U64 SF_ARG4[RBP]
	MOV	RDI,U64 [RAX]	//rdi=vga image

	MOV	RAX,U64 SF_ARG1[RBP]
	MOV	R8,U64 [RAX]	//rbp=vga dst

@@05:	LODSD
	CMP	U32 [RDI],EAX
	JE	@@10
	MOV	U32 [RDI],EAX
	XLATB
	ROR	EAX,8
	XLATB
	ROR	EAX,8
	XLATB
	ROR	EAX,8
	XLATB
	ROR	EAX,8
	MOV	U32 [R8],EAX
@@10:	ADD	RDI,4
	ADD	R8,4
	DEC	RCX
	JNZ	@@05

	MOV	RAX,U64 SF_ARG1[RBP]
	MOV	U64 [RAX],R8
	MOV	RAX,U64 SF_ARG4[RBP]
	MOV	U64 [RAX],RDI
	MOV	RAX,U64 SF_ARG2[RBP]
	MOV	U64 [RAX],RSI
	POP	RDI
	POP	RSI
	LEAVE
	RET1	32
// ************************************
_GR_UPDATE_LINE32_FLUSH_CACHE::
	ENTER	0
	PUSH	RSI
	PUSH	RDI

	MOV	RAX,U64 SF_ARG2[RBP]
	MOV	RSI,U64 [RAX]	//rsi=src2
	MOV	RBX,U64 [&reverse_bits_table]
	MOV	RCX,U64 SF_ARG3[RBP] //rcx=d5
	MOV	RAX,U64 SF_ARG4[RBP]
	MOV	RDI,U64 [RAX]	//rdi=vga image

	MOV	RAX,U64 SF_ARG1[RBP]
	MOV	R8,U64 [RAX]	//rbp=vga dst

@@05:	LODSD
	MOV	U32 [RDI],EAX
	XLATB
	ROR	EAX,8
	XLATB
	ROR	EAX,8
	XLATB
	ROR	EAX,8
	XLATB
	ROR	EAX,8
	MOV	U32 [R8],EAX
	ADD	RDI,4
	ADD	R8,4
	DEC	RCX
	JNZ	@@05

	MOV	RAX,U64 SF_ARG1[RBP]
	MOV	U64 [RAX],R8
	MOV	RAX,U64 SF_ARG4[RBP]
	MOV	U64 [RAX],RDI
	MOV	RAX,U64 SF_ARG2[RBP]
	MOV	U64 [RAX],RSI
	POP	RDI
	POP	RSI
	LEAVE
	RET1	32
// ************************************
_DC_BLOT_COLOR4::
	ENTER	0
	PUSH	RSI
	PUSH	RDI
	PUSH	R10

	MOV	RDI,U64 SF_ARG1[RBP]
	MOV	RSI,U64 SF_ARG2[RBP]
	MOV	RBX,U64 SF_ARG3[RBP]
	MOV	RCX,U64 SF_ARG4[RBP]

	MOV	R8,RDI
	ADD	R8,RCX
	MOV	R9,R8
	ADD	R9,RCX
	MOV	R10,R9
	ADD	R10,RCX

@@5:	LODSQ
	MOV	RDX,U64 [RBX]
	CMP	RAX,RDX
	JE	I32 @@10
	MOV	U64 [RBX],RAX

	XOR	RDX,RDX
	BT	RAX,7*8+0
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,6*8+0
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,5*8+0
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,4*8+0
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,3*8+0
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,2*8+0
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,1*8+0
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,0*8+0
	ADC	RDX,0
	MOV	U8 [RDI],DL

	XOR	RDX,RDX
	BT	RAX,7*8+1
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,6*8+1
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,5*8+1
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,4*8+1
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,3*8+1
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,2*8+1
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,1*8+1
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,0*8+1
	ADC	RDX,0
	MOV	U8 [R8],DL

	XOR	RDX,RDX
	BT	RAX,7*8+2
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,6*8+2
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,5*8+2
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,4*8+2
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,3*8+2
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,2*8+2
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,1*8+2
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,0*8+2
	ADC	RDX,0
	MOV	U8 [R9],DL

	XOR	RDX,RDX
	BT	RAX,7*8+3
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,6*8+3
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,5*8+3
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,4*8+3
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,3*8+3
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,2*8+3
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,1*8+3
	ADC	RDX,0
	SHL1	RDX
	BT	RAX,0*8+3
	ADC	RDX,0
	MOV	U8 [R10],DL

@@10:	ADD	RBX,8

	INC	RDI
	INC	R8
	INC	R9
	INC	R10

	DEC	RCX
	JNZ	I32 @@5

	POP	R10
	POP	RSI
	POP	RDI
	LEAVE
	RET1	32
// ************************************
_IS_PIXEL_COVERED0::
	ENTER	0
#assert FONT_WIDTH==FONT_HEIGHT==8
	MOV	RCX,U64 SF_ARG3[RBP]
	SAR	RCX,3
	MOV	RAX,U64 SF_ARG2[RBP]
	SAR	RAX,3
	MOV	RDX,U64 SF_ARG1[RBP]
	IMUL2	RCX,TEXT_COLS
	MOV	RBX,U64 [&gr_win_z_buf]
	MOV	DX,U16 CTask.win_z_num[RDX]
	ADD	RCX,RAX
	XOR	RAX,RAX
	CMP	DX,U16 [RBX+RCX*2]
	JAE	@@05	//Jump not covered
	INC	RAX
@@05:	LEAVE
	RET1	24
// ************************************
_GR_ROP_EQU_U8_NO_CLIPPING::
//Puts foreground char shape onto gr_dc2.
//ch.u8[1] is color as a whole byte
//ch DOCEt_UNDERLINE attr flag is used.
	ENTER	0
	MOVZX	RAX,U8 SF_ARG1[RBP]
	MOV	RDX,U64 [&gr_font]
	MOV	RAX,U64 [RDX+RAX*8]	//RAX is 8x8 char font data, 64-bits
	BT	U64 SF_ARG1[RBP],DOCEt_UNDERLINE
	JNC	@@05
	MOV	RBX,0xFF00000000000000
	OR	RAX,RBX
@@05:	OR	RAX,RAX
	JZ	I32 @@10

	PUSH	RSI
	PUSH	RDI
	MOVZX	RBX,U8 SF_ARG1+1[RBP]	//Warning color is 8-bit
	MOV	RDX,U64 [&gr_1_to_8_colors]
	MOV	R8,U64 [RDX+RBX*8]	//R8 is color repeated 8 times.
	MOV	R9,U64 SF_ARG3[RBP]	//R9 is width_internal
	MOV	RDI,U64 SF_ARG2[RBP]
	MOV	RSI,U64 [&gr_1_to_8_bits] //RSI is gr_1_to_8_bits

//	m=my_1_to_8_bits[ch&255];ch>>=8;
//	*dst=*dst&~m|c&m;
//	dst(U8 *)+=w1;

	MOVZX	RBX,AL
	MOV	RDX,U64 [RSI+RBX*8]
	SHR	RAX,8
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX
	ADD	RDI,R9

	MOVZX	RBX,AL
	MOV	RDX,U64 [RSI+RBX*8]
	SHR	RAX,8
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX
	ADD	RDI,R9

	MOVZX	RBX,AL
	MOV	RDX,U64 [RSI+RBX*8]
	SHR	RAX,8
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX
	ADD	RDI,R9

	MOVZX	RBX,AL
	MOV	RDX,U64 [RSI+RBX*8]
	SHR	RAX,8
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX
	ADD	RDI,R9

	MOVZX	RBX,AL
	MOV	RDX,U64 [RSI+RBX*8]
	SHR	RAX,8
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX
	ADD	RDI,R9

	MOVZX	RBX,AL
	MOV	RDX,U64 [RSI+RBX*8]
	SHR	RAX,8
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX
	ADD	RDI,R9

	MOVZX	RBX,AL
	MOV	RDX,U64 [RSI+RBX*8]
	SHR	RAX,8
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX
	ADD	RDI,R9

	MOV	RDX,U64 [RSI+RAX*8]
	MOV	RBX,U64 [RDI]
	MOV	RCX,RDX
	NOT	RCX
	AND	RDX,R8
	AND	RCX,RBX
	OR	RCX,RDX
	MOV	U64 [RDI],RCX

	POP	RDI
	POP	RSI
@@10:	LEAVE
	RET1	24
}

#help_index "Graphics/Char;Char/Graphics"
public _extern _PLOT_SCREEN_U8 Bool PlotScreenU8(I64 w,I64 x,I64 y);
public _extern _PLOT_WIN_U8 Bool PlotWinU8(CTask *task,I64 w,I64 x,I64 y);
public _extern _PLOT_WIN_STR2	Bool PlotWinStr2(
	   CTask *task,U8 *s,I64 len,I64 attr,I64 x,I64 y);
public _extern _PLOT_ATTR_STR2 Bool PlotAttrStr2(
	   CTask *task,U32 *hl,I64 len,I64 x,I64 y);

public _extern _PLOT_ATTR	  Bool PlotWinAttr(
	   CTask *task,I64 len,I64 attr,I64 x,I64 y);
public _extern _PLOT_BORDER_U8 Bool PlotBorderU8(CTask *task,I64 w,I64 x,I64 y);
_extern _GR_ROP_EQU_U8_NO_CLIPPING U0 GrRopEquU8NoClipping(I64 ch,U8 *dst,I64 width_internal);

#help_index "Graphics"
//No clipping
public _extern _IS_PIXEL_COVERED0 Bool IsPixelCovered0(CTask *task,I64 x,I64 y);

_extern _GR_UPDATE_LINE32 U0 GrUpdateLine32(
  U32 **_dst3,U32 **_src2,I64 w,U8 **_vga);
_extern _GR_UPDATE_LINE32_FLUSH_CACHE U0 GrUpdateLine32FlushCache(
  U32 **_dst3,U32 **_src2,I64 w,U8 **_vga);
_extern _DC_BLOT_COLOR4 U0 DCBlotColor4(
  U8 *dst,I64 *img,I64 *img_cache,I64 cnt);
#help_index ""
