#define KERNEL_BASE 0x80000000

.global __read_control_register
__read_control_register:
	mrc p15, 0, r0, cr1, cr0, 0 /*operate-0 from co-processor-p15 to R0, cr1 cr0 are registers on co-proc*/
	mov pc, lr /*return*/

.global __set_control_register
__set_control_register:
	mcr p15, 0, r0, cr1, cr0, 0 /*operate-0 from R0 to co-processor-p15*/
	mov pc, lr /*return*/

.global __set_domain_access_control
__set_domain_access_control:
	mcr p15, 0, r0, cr3, cr0, 0
	mov pc, lr

.global __set_translation_table_base
__set_translation_table_base:
	mcr p15, 0, r0, c2, c0, 0 //set ttbase
	mcr p15, 0, r0, c8, c7, 0 //flush tlb
	mov pc, lr

.global __enable_paging
__enable_paging:
	mov r2, lr /*lr(R14) return PC stored*/

	mov r0, #1
	bl __set_domain_access_control /*system.S*/

	ldr r0, =_startup_page_dir /*kernel/src/mm/startup.c*/
	sub r0, #KERNEL_BASE /*get physical address from virtual address*/
	bl __set_translation_table_base /*system.S*/

	bl __read_control_register /*system.S*/
	orr r0, #1 /*enable paging*/
	bl __set_control_register /*system.S*/

	mov pc, r2 /*return*/

.global __jump2_high_mem
__jump2_high_mem:
	add lr, #KERNEL_BASE
	mov pc, lr

#ifdef CPU_NUM
.global __get_cpu_id
__get_cpu_id:
	MRC p15, 0, r0, c0, c0, 5
	AND r0, r0, #0x03
	// read CPU ID register, mask in the CPUID field
	MOV pc, lr

.global __enable_scu
__enable_scu:
	MRC p15, 4, r0, c15, c0, 0
	LDR r1, [r0, #0x0] // Read periph base address
	// Read the SCU Control Register
	ORR r1, r1, #0x1 // Set bit 0 (The Enable bit)
	STR r1, [r0, #0x0] // Write back modifed value
	BX lr
#endif

.global __irq_enable
__irq_enable:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r1
	mov pc, lr

.global __irq_disable
__irq_disable:
	mrs r0, cpsr
	orr r0, r0, #0x80
	msr cpsr, r0
	mov pc, lr

.global __int_off // SR = int_off()
__int_off: 
	mrs r0, cpsr
	mov r1, r0
	orr r1, r1, #0x80
	msr cpsr, r1
  mov pc, lr

.global __int_on
__int_on:
	msr cpsr, r0
  mov pc, lr

.global __use_high_interrupts
__use_high_interrupts:
	push {lr}
	bl __read_control_register
	orr r0, #0x2000
	bl __set_control_register
	pop {lr}

	mov pc, lr

#ifdef CPU_NUM
// spinlock, 0=UNLOCKED, 1=LOCKED
.global __s_lock
__s_lock: // slock(int *spin): acquire spinlock
	ldrex r1, [r0]     // read spinlock value
	cmp r1, #0x0       // compare with 0
	WFENE              // not 0 means already locked: do WFE
	bne __s_lock          // try again after woken up by event // set r1=1
	mov r1, #1
	strex r2, r1, [r0] // try to store 1 to [r0]; r2=return value
	cmp r2, #0x0       // check return value in r2
	bne __s_lock          // not 0 means failed; retry to lock again
	DMB                // memory barrier BEFORE accessing CR
	bx lr              // return only if has acquired the spinlock

.global __s_unlock
__s_unlock:
	mov r1, #0x0 
	DMB
	str r1, [r0]
	DSB
	SEV
	bx lr
#endif

.global __mem_barrier
__mem_barrier:
	mov r0, #0 @ <Rd> should be zero (SBZ) for this
	mcr p15, 0, r0, c7, c10, 5 @ data memory barrier
	mov pc, lr
