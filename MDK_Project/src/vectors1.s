
	PRESERVE8
	AREA Vect1, CODE, READONLY



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

; *****************
; Exception Vectors
; *****************

; Note: LDR PC instructions are used here, though branch (B) instructions
; could also be used, unless the ROM is at an address >32MB.

;        ENTRY
        EXPORT	Vector_Table_1
Vector_Table_1		
        B       Reset_Go
        LDR     PC, Undefined_Addr
        LDR     PC, SWI_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                             ; Reserved vector
        LDR     PC, IRQ_Addr
        LDR     PC, FIQ_Addr
        
				
        IMPORT  Reset_Go           ; In sys_start.s
        
Reset_Addr      DCD     Reset_Go
Undefined_Addr  DCD     Undefined_Handler
SWI_Addr        DCD     SWI_Handler
Prefetch_Addr   DCD     Prefetch_Handler
Abort_Addr      DCD     Abort_Handler
				        DCD		  0
IRQ_Addr        DCD     IRQ_Handler
FIQ_Addr        DCD     FIQ_Handler


; ************************
; Exception Handlers
; ************************

; The following dummy handlers do not do anything useful in this example.
; They are set up here for completeness.

 IMPORT  SWI_Handler1
 IMPORT	 arm32_do_irq
 IMPORT  vPortYieldProcessor
 IMPORT  vPortIrqProcessor

Undefined_Handler
        B       Undefined_Handler
SWI_Handler
		B 		vPortYieldProcessor ;for freerots, if no os, this is SWI_Handler1
Prefetch_Handler
        B       Prefetch_Handler
Abort_Handler
        B       Abort_Handler
		NOP
IRQ_Handler
        B       vPortIrqProcessor ;for freerots, if no os, this is IRQ_Handler2
FIQ_Handler
        B       FIQ_Handler
				
	
			
IRQ_Handler2
	LDR SP,=FIQ_Stack  
	sub sp, sp, #72
	stmia sp, {r0 - r12}
	add r8, sp, #60
	stmdb r8, {sp, lr}^
	str lr, [r8, #0]
	mrs r6, spsr
	str r6, [r8, #4]
	str r0, [r8, #8]
	mov r0, sp
	bl arm32_do_irq
	ldmia sp, {r0 - lr}^
	mov r0, r0
	ldr lr, [sp, #60]
	add sp, sp, #72
	subs pc, lr, #4


	END
