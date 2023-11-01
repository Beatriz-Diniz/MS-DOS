/* Linker script */

/* Output flat binary (no structure) */
OUTPUT_FORMAT(binary)
SECTIONS{
	/* Load address */
	. = 0x7c00;

	/* Stage 1 */
	.text :
	{
		rt0.o (.text .data .bss .rodata)
		stage1.o (.text .data .bss .rodata)
		utils.o (.text .data .bss .rodata)
	}

	/* Boot signature address */
	. = 0x7c00 + 510;

	/* Boot signature */
	.signature :
	{
		BYTE(0x55)
		BYTE(0xAA)
	}

	/* For load_sector() */
	_STAGE2_ADDR = .;

	/* Stage 2*/
	.text2 :
	{
		stage2.o (.text .data .bss .rodata)
	}

	_STAGE2_SIZE = . - _STAGE2_ADDR;

	/* Just bellow 7c00h */
	_END_STACK = 0x7c00;
}

/* Prepend with the start file */
STARTUP(rt0.o)
