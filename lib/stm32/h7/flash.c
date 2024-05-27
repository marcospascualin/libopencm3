/** @defgroup flash_file FLASH peripheral API
 *
 * @ingroup peripheral_apis
 */

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2017 Matthew Lai @m@matthewlai.ca>
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
 * Copyright (C) 2010 Mark Butler <mbutler@physics.otago.ac.nz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@{*/

#include <libopencm3/stm32/flash.h>

/**
 * @brief 		Issue Pipeline Stall.
 * 				RM0385: After performing a data write operation and before polling the BSY bit
 * 				to be cleared, the software can issue a DSB instruction to guarantee the
 * 				completion of a previous data write operation.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
static inline void flash_pipeline_stall(void)
{
	__asm__ volatile("dsb" ::: "memory");
}

/**
 * @brief 		Wait until Last Operation has Ended.
 * 				This loops indefinitely until an operation (write or erase) has completed by testing the busy flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_wait_for_last_operation(void)
{
	flash_pipeline_stall();
	while ((FLASH_SR & FLASH_SR_QW) == FLASH_SR_QW)
		;
}

/**
 * @brief 		Clear the Write protection Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_wrperr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_WRPERRCLR;
}

/**
 * @brief 		Clear the Programming sequence Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_pgserr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_PGSERRCLR;
}

/**
 * @brief 		Clear the Strobe Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_strberr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_STRBERRCLR;
}

/**
 * @brief 		Clear the Inconsistency Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_incerr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_INCERRCLR;
}

/**
 * @brief 		Clear the Write/erase Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_operr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_OPERRCLR;
}

/**
 * @brief 		Clear the Read Protection Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_rdperr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_RDPERRCLR;
}

/**
 * @brief 		Clear the Secure Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_rdserr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_RDSERRCLR;
}

/**
 * @brief 		Clear the Single correction Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_sneccerr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_SNECCERRCLR;
}

/**
 * @brief 		Clear the ECC double detection Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_dbeccerr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_DBECCERRCLR;
}

/**
 * @brief 		Clear the CRC end of calculation Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_crcend_flag(void)
{
	FLASH_CCR |= FLASH_CCR_CRCENDCLR;
}

/**
 * @brief 		Clear the CRC read Error Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_crcrderr_flag(void)
{
	FLASH_CCR |= FLASH_CCR_CRCRDERRCLR;
}

/**
 * @brief 		Clear the End-Of-Program Flag.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_eop_flag(void)
{
	FLASH_CCR |= FLASH_CCR_EOPCLR;
}

/**
 * @brief 		Clear All Status Flags.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_clear_status_flags(void)
{
	flash_clear_wrperr_flag();
	flash_clear_pgserr_flag();
	flash_clear_strberr_flag();
	flash_clear_incerr_flag();
	flash_clear_operr_flag();
	flash_clear_rdperr_flag();
	flash_clear_rdserr_flag();
	flash_clear_sneccerr_flag();
	flash_clear_dbeccerr_flag();
	flash_clear_crcend_flag();
	flash_clear_crcrderr_flag();
	flash_clear_eop_flag();
}

/**
 * @brief 		.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_unlock(void)
{
	/* Authorize the FPEC access. */
	FLASH_KEYR = FLASH_KEYR_KEY1;
	FLASH_KEYR = FLASH_KEYR_KEY2;
}

/**
 * @brief 		.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_lock(void)
{
	FLASH_CR |= FLASH_CR_LOCK;
}

/**
 * @brief 		Set the programming word width. Note carefully the power supply voltage
 * 				restrictions under which the different word sizes may be used. See the
 * 				programming manual for more information..
 *
 * @param[in] 	psize	The programming word width one of: @ref flash_cr_program_width
 *
 * @returns		None
 */
static inline void flash_set_program_size(uint32_t psize)
{
	FLASH_CR &= ~(FLASH_CR_PSIZE_MASK << FLASH_CR_PSIZE_SHIFT);
	FLASH_CR |= (psize & FLASH_CR_PSIZE_MASK) << FLASH_CR_PSIZE_SHIFT;
}

/**
 * @brief 		Program a 64 bit Word to FLASH.
 *
 * @note 		This performs all operations necessary to program a 64 bit word to FLASH memory.
 * 				The program error flag should be checked separately for the event that memory
 * 				was not properly erased.
 *
 * @param[in] 	address Starting address in Flash.
 * @param[in] 	data Double word to write
 *
 * @returns		None
 */
void flash_program_double_word(uint32_t address, uint64_t data)
{
	/* Ensure that all flash operations are complete. */
	flash_wait_for_last_operation();
	flash_set_program_size(FLASH_CR_PROGRAM_X64);

	/* Enable writes to flash. */
	FLASH_CR |= FLASH_CR_PG;

	/* Program the double_word. */
	MMIO64(address) = data;

	/* Wait for the write to complete. */
	flash_wait_for_last_operation();

	/* Disable writes to flash. */
	FLASH_CR &= ~FLASH_CR_PG;
}

/**
 * @brief 		Program a 32 bit Word to FLASH.
 *
 * @note 		This performs all operations necessary to program a 64 bit word to FLASH memory.
 * 				The program error flag should be checked separately for the event that memory
 * 				was not properly erased.
 *
 * @param[in] 	address Starting address in Flash.
 * @param[in] 	data data word to write
 *
 * @returns		None
 */
void flash_program_word(uint32_t address, uint32_t data)
{
	/* Ensure that all flash operations are complete. */
	flash_wait_for_last_operation();
	flash_set_program_size(FLASH_CR_PROGRAM_X32);

	/* Enable writes to flash. */
	FLASH_CR |= FLASH_CR_PG;

	/* Program the word. */
	MMIO32(address) = data;

	/* Wait for the write to complete. */
	flash_wait_for_last_operation();

	/* Disable writes to flash. */
	FLASH_CR &= ~FLASH_CR_PG;
}

/**
 * @brief 		Program a Half Word to FLASH.
 *
 * @note 		This performs all operations necessary to program a 64 bit word to FLASH memory.
 * 				The program error flag should be checked separately for the event that memory
 * 				was not properly erased.
 *
 * @param[in] 	address Starting address in Flash.
 * @param[in] 	data data half word to write
 *
 * @returns		None
 */
void flash_program_half_word(uint32_t address, uint16_t data)
{
	flash_wait_for_last_operation();
	flash_set_program_size(FLASH_CR_PROGRAM_X16);

	FLASH_CR |= FLASH_CR_PG;

	MMIO16(address) = data;

	flash_wait_for_last_operation();

	FLASH_CR &= ~FLASH_CR_PG; /* Disable the PG bit. */
}

/**
 * @brief 		Program an 8 bit Byte to FLASH.
 *
 * @note 		This performs all operations necessary to program a 64 bit word to FLASH memory.
 * 				The program error flag should be checked separately for the event that memory
 * 				was not properly erased.
 *
 * @param[in] 	address Starting address in Flash.
 * @param[in] 	data data byte to write
 *
 * @returns		None
 */
void flash_program_byte(uint32_t address, uint8_t data)
{
	flash_wait_for_last_operation();
	flash_set_program_size(FLASH_CR_PROGRAM_X8);

	FLASH_CR |= FLASH_CR_PG;

	MMIO8(address) = data;

	flash_wait_for_last_operation();

	FLASH_CR &= ~FLASH_CR_PG; /* Disable the PG bit. */
}

/**
 * @brief 		Program a Data Block to FLASH.
 *
 * @note 		This programs an arbitrary length data block to FLASH memory.
 * 				The program error flag should be checked separately for the event that memory
 * 				was not properly erased.
 *
 * @param[in] 	address Starting address in Flash.
 * @param[in] 	data Pointer to start of data block.
 * @param[in] 	len Length of data block.
 *
 * @returns		None
 */
void flash_program(uint32_t address, const uint8_t *data, uint32_t len)
{
	/* TODO: Use dword and word size program operations where possible for
	 * turbo speed.
	 */
	uint32_t i;
	for (i = 0; i < len; i++)
	{
		flash_program_byte(address + i, data[i]);
	}
}

/**
 * @brief 		Erase a Sector of FLASH.
 *
 * @note 		This performs all operations necessary to erase a sector in FLASH memory.
 * 				The page should be checked to ensure that it was properly erased. A sector must
 * 				first be fully erased before attempting to program it.
 * 				See the reference manual or the FLASH programming manual for details.
 *
 * @param[in] 	sector (0 - 11 for some parts, 0-23 on others)
 * @param[in] 	program_size: 0 (8-bit), 1 (16-bit), 2 (32-bit), 3 (64-bit)
 *
 * @returns		None
 */
void flash_erase_sector(uint8_t sector, uint32_t program_size)
{
	flash_wait_for_last_operation();
	flash_set_program_size(program_size);

	/* Sector numbering is not contiguous internally! */
	if (sector >= 12)
	{
		sector += 4;
	}

	FLASH_CR &= ~(FLASH_CR_SNB_MASK << FLASH_CR_SNB_SHIFT);
	FLASH_CR |= (sector & FLASH_CR_SNB_MASK) << FLASH_CR_SNB_SHIFT;
	FLASH_CR |= FLASH_CR_SER;
	FLASH_CR |= FLASH_CR_STRT;

	flash_wait_for_last_operation();
	FLASH_CR &= ~FLASH_CR_SER;
	FLASH_CR &= ~(FLASH_CR_SNB_MASK << FLASH_CR_SNB_SHIFT);
}

/**
 * @brief 		Erase FLASH bank.
 *
 * @note 		This performs all operations necessary to erase all sectors in the FLASH memory.
 *
 * @param[in] 	program_size: 0 (8-bit), 1 (16-bit), 2 (32-bit), 3 (64-bit)
 *
 * @returns		None
 */
void flash_erase_all_sectors(uint32_t program_size)
{
	flash_wait_for_last_operation();
	flash_set_program_size(program_size);

	FLASH_CR |= FLASH_CR_BER;  /* Enable mass erase. */
	FLASH_CR |= FLASH_CR_STRT; /* Trigger the erase. */

	flash_wait_for_last_operation();
	FLASH_CR &= ~FLASH_CR_BER; /* Disable mass erase. */
}

/**
 * @brief 		Program the Option Bytes.
 *
 * @note		This performs all operations necessary to program the option bytes.
 * 				The option bytes do not need to be erased first.
 *
 * @param[in] 	data value to be programmed.
 *
 * @returns		None
 */
void flash_program_option_bytes(uint32_t data)
{
	flash_wait_for_last_operation();

	if (FLASH_OPTCR & FLASH_OPTCR_OPTLOCK)
	{
		flash_unlock_option_bytes();
	}

	FLASH_OPTCR = data & ~0x3;
	FLASH_OPTCR |= FLASH_OPTCR_OPTSTRT; /* Enable option byte prog. */
	flash_wait_for_last_operation();
}

/**
 * @brief 		Lock the Option Byte Access.
 *
 * @note 		This disables write access to the option bytes. It is locked by default on reset.
 *
 * @param[in] 	Ninguno
 *
 * @returns		None
 */
void flash_lock_option_bytes(void)
{
	FLASH_OPTCR |= FLASH_OPTCR_OPTLOCK;
}

void flash_unlock_option_bytes(void)
{
	FLASH_OPTKEYR = FLASH_OPTKEYR_KEY1;
	FLASH_OPTKEYR = FLASH_OPTKEYR_KEY2;
}

void flash_set_ws(uint32_t ws)
{
	uint32_t reg32;

	reg32 = FLASH_ACR;
	reg32 &= ~(FLASH_ACR_LATENCY_MASK << FLASH_ACR_LATENCY_SHIFT);
	reg32 |= (ws << FLASH_ACR_LATENCY_SHIFT);
	FLASH_ACR = reg32;
}

/**@}*/
