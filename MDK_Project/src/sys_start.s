

	PRESERVE8
    AREA F1C100S_INIT, CODE, READONLY

;--------------------------------------------
; Mode bits and interrupt flag (I&F) defines
;--------------------------------------------
USR_MODE    EQU     0x10
FIQ_MODE    EQU     0x11
IRQ_MODE    EQU     0x12
SVC_MODE    EQU     0x13
ABT_MODE    EQU     0x17
UDF_MODE    EQU     0x1B
SYS_MODE    EQU     0x1F
I_BIT       EQU     0x80
F_BIT       EQU     0x40



RAM_Limit        EQU      (0x80000000+0x2000000)           ; For unexpanded hardware board

; original edition	
;USR_Stack				EQU				RAM_Limit
;SVC_Stack       		EQU     		(USR_Stack-1024*50)      ; SVC stack at top of memory
;FIQ_Stack       		EQU     		(SVC_Stack-1024*50)     ; followed by IRQ stack
;IRQ_Stack       		EQU     		(FIQ_Stack-1024*50)       ; followed by IRQ stack
;Abort_Stack				EQU				(IRQ_Stack-1024*100)
;UND_Stack				EQU				(Abort_Stack-1024*50)	

; stack become bigger, lion test: software run in svc_stack
USR_Stack				EQU				RAM_Limit
SVC_Stack       		EQU     		(USR_Stack-1024*64)      ; SVC stack at top of memory
FIQ_Stack       		EQU     		(SVC_Stack-1024*256)     ; followed by IRQ stack
IRQ_Stack       		EQU     		(FIQ_Stack-1024*64)       ; followed by IRQ stack
Abort_Stack				EQU				(IRQ_Stack-1024*128)
UND_Stack				EQU				(Abort_Stack-1024*64)
	
  
    ENTRY
    EXPORT  Reset_Go

	
Reset_Go

    ;--------------------------------
    ; Initial Stack Pointer register
    ;--------------------------------
    ;INIT_STACK
     MSR    CPSR_c, #UDF_MODE :OR: I_BIT :OR: F_BIT
     LDR    SP, =UND_Stack

     MSR    CPSR_c, #ABT_MODE :OR: I_BIT :OR: F_BIT
     LDR    SP, =Abort_Stack

     MSR    CPSR_c, #IRQ_MODE :OR: I_BIT :OR: F_BIT
     LDR    SP, =IRQ_Stack

     MSR    CPSR_c, #FIQ_MODE :OR: I_BIT :OR: F_BIT
     LDR    SP, =FIQ_Stack

     MSR    CPSR_c, #SYS_MODE :OR: I_BIT :OR: F_BIT
     LDR    SP, =USR_Stack

     MSR    CPSR_c, #SVC_MODE :OR: I_BIT :OR: F_BIT
     LDR    SP, =SVC_Stack

    ;------------------------------------------------------
    ; Set the normal exception vector of CP15 control bit
    ;------------------------------------------------------
        MRC p15, 0, r0 , c1, c0     ; r0 := cp15 register 1
        bic r0, r0, #0x2000         ; Clear bit13 in r1 =0�Ͷ������� =1�߶�������
        MCR p15, 0, r0 , c1, c0     ; cp15 register 1 := r0

  IMPORT Vector_Table_1


	ldr r0, = Vector_Table_1
	mrc p15, 0, r2, c1, c0, 0	 ;����CP15
	ands r2, r2, #(1 << 13)    ;ȡ13λ�ж�
	ldreq r1, =0x00000000      ;ֵ=0  Z=1���
	ldrne r1, =0xFFFF0000			 ;ֵ!=0 Z=0 �����
	;��R0��ַ����16�����ݵ�R1
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}
	ldmia r0!, {r2-r8, r10}
	stmia r1!, {r2-r8, r10}


;	mrc	p15, 0, r0, c1, c0, 0
;	orr	r0, r0, #0x00001000	; bit 12 (I) I-cache
;	orr	r0, r0, #4          ; bit 12 (D) D-cache MMU���ÿ�
;	mcr	p15, 0, r0, c1, c0, 0
	
   
        IMPORT  __main


    ;-----------------------------
    ;   enter the C code
    ;-----------------------------
        B   __main




    END




