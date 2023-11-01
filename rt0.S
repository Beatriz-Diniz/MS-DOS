	.code16
	.global _start
	.global drive
	.global size2

/* 
   Some modern BIOSes expect that the boot media have a MS-DOS
   compatible VBR (Volume Boot Record) header, typical of 
   FAT-formatted media, stored at the begining of the first sector.
   Such BIOSes may either refuse to boot or corrupt the bootstrap 
   code if a VBR is not present. 
*/

_start:
	jmp 		code 			/* VBR signature for the case it's expected, */
	.fill 	60, 1, 0 			/* and dummy BPB (BIOS Parameter Block) */

/* Canonicalize segment registers to 0x0:7c00. */
code:
	cli 						/* Disable interrupts */
	xor 		%ax, %ax
	mov 		%ax, %ds 			/* Zero data segment */
	mov 		%ax, %es 			/* Zero extra segment */
	mov 		%ax, %fs 			/* Zero extra segment 2 */
	mov 		%ax, %gs 			/* Zero extra segment 3 */
	mov 		%ax, %ss 			/* Zero stack segment */
	mov 		$_END_STACK, %sp 	/* Setup stack (from linker script) */
	mov 		%dl, drive 		/* Save boot drive number for later */
	jmp 		$0x0, $init0 		/* Set memory segment 0 and adjust ip */

init0:
	sti 						/* Enable interrupts */
	call 	main 			/* Call main*/

halt:
	hlt 						/* On return from main, halt */
	jmp 		halt

/* For later use (boot device) */
drive:
	.byte 0x0

/* Size of stage2 (in sectors) */
size2:
	.byte 0x0
