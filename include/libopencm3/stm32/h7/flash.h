/** @defgroup flash_defines FLASH Defines
 * @brief <b>Defined Constants and Types for the STM32H7xx Flash
 * controller</b>
 * @ingroup STM32H7xx_defines
 *
 * @author @htmlonly &copy; @endhtmlonly 2019
 * Brian Viele <vielster@allocor.tech>
 */
/*
 * This file is part of the libopencm3 project.
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

#ifndef LIBOPENCM3_FLASH_H
#define LIBOPENCM3_FLASH_H

// #include <libopencm3/stm32/common/flash_common_all.h>
// TODO #include <libopencm3/stm32/common/flash_common_idcache.h>

/**@{*/

/** @defgroup flash_registers Flash Registers
 * @ingroup flash_defines
@{*/
/** Flash Access Control register */
#define FLASH_ACR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x00)
/** Flash Key register */
#define FLASH_KEYR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x04)
/** Flash Option bytes key register */
#define FLASH_OPTKEYR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x08)
/** Flash Control register */
#define FLASH_CR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x0C)
/** Flash Status register*/
#define FLASH_SR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x10)
/** Flash Clear Control register*/
#define FLASH_CCR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x14)
/** Flash Option Control register */
#define FLASH_OPTCR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x18)
/** Flash Option Control register 1 (bank 2) */
#define FLASH_OPTSR_CUR MMIO32(FLASH_MEM_INTERFACE_BASE + 0x1C)
/*@}*/

/** @addtogroup flash_acr_values FLASH_ACR values
 * @ingroup flash_registers
@{*/
#define FLASH_ACR_WRHIGHFREQ_MASK (0x3)
#define FLASH_ACR_WRHIGHFREQ_SHIFT (0x4)
#define FLASH_ACR_WRHF_VOS1_70MHZ (0 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS1_185MHZ (1 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS1_225MHZ (2 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS2_55MHZ (0 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS2_165MHZ (1 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS2_225MHZ (2 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS3_45MHZ (0 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS3_135MHZ (1 << FLASH_ACR_WRHIGHFREQ_SHIFT)
#define FLASH_ACR_WRHF_VOS3_225MHZ (2 << FLASH_ACR_WRHIGHFREQ_SHIFT)
/*@}*/

/** @defgroup flash_latency FLASH Wait States
@ingroup flash_defines
@{*/
#define FLASH_ACR_LATENCY_SHIFT 0
#define FLASH_ACR_LATENCY_MASK 0x0f
#define FLASH_ACR_LATENCY(w) ((w) & FLASH_ACR_LATENCY_MASK)
#define FLASH_ACR_LATENCY_0WS 0x00
#define FLASH_ACR_LATENCY_1WS 0x01
#define FLASH_ACR_LATENCY_2WS 0x02
#define FLASH_ACR_LATENCY_3WS 0x03
#define FLASH_ACR_LATENCY_4WS 0x04
#define FLASH_ACR_LATENCY_5WS 0x05
#define FLASH_ACR_LATENCY_6WS 0x06
#define FLASH_ACR_LATENCY_7WS 0x07
/*@}*/

/* --- FLASH Keys -----------------------------------------------------------*/

#define FLASH_KEYR_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEYR_KEY2 ((uint32_t)0xcdef89ab)

#define FLASH_OPTKEYR_KEY1 ((uint32_t)0x08192a3b)
#define FLASH_OPTKEYR_KEY2 ((uint32_t)0x4c5d6e7f)

/* --- FLASH_CR values ----------------------------------------------------- */

#define FLASH_CR_LOCK (1 << 0)
#define FLASH_CR_PG (1 << 1)
#define FLASH_CR_SER (1 << 2)
#define FLASH_CR_BER (1 << 3)
#define FLASH_CR_PSIZE_SHIFT 4
#define FLASH_CR_PSIZE_MASK 0x03
#define FLASH_CR_FW (1 << 6)
#define FLASH_CR_STRT (1 << 7)
#define FLASH_CR_SNB_SHIFT 8
#define FLASH_CR_SNB_MASK 0x07
#define FLASH_CR_CRCEN (1 << 15)
#define FLASH_CR_EOPIE (1 << 16)
#define FLASH_CR_WRPERRIE (1 << 17)
#define FLASH_CR_PGSERRIE (1 << 18)
#define FLASH_CR_STRBERRIE (1 << 19)
#define FLASH_CR_INCERRIE (1 << 21)
#define FLASH_CR_OPERRIE (1 << 22)
#define FLASH_CR_RDPERRIE (1 << 23)
#define FLASH_CR_RDSERRIE (1 << 24)
#define FLASH_CR_SNECCERRIE (1 << 25)
#define FLASH_CR_DBECCERRIE (1 << 26)
#define FLASH_CR_CRCENDIE (1 << 27)
#define FLASH_CR_CRCRDERRIE (1 << 28)

/* --- FLASH_SR values ----------------------------------------------------- */

#define FLASH_SR_BSY (1 << 0)
#define FLASH_SR_WBNE (1 << 1)
#define FLASH_SR_QW (1 << 2)
#define FLASH_SR_CRCBUSY (1 << 3)
#define FLASH_SR_EOP (1 << 16)
#define FLASH_SR_WRPERR (1 << 17)
#define FLASH_SR_PGSERR (1 << 18)
#define FLASH_SR_STRBERR (1 << 19)
#define FLASH_SR_INCERR (1 << 21)
#define FLASH_SR_OPERR (1 << 22)
#define FLASH_SR_RDPERR (1 << 23)
#define FLASH_SR_RDSERR (1 << 24)
#define FLASH_SR_SNECCERR (1 << 25)
#define FLASH_SR_DBECCERR (1 << 26)
#define FLASH_SR_CRCEND (1 << 27)
#define FLASH_SR_CRCRDERR (1 << 28)

/* --- FLASH_CCR values ----------------------------------------------------- */

#define FLASH_CCR_EOPCLR (1 << 16)
#define FLASH_CCR_WRPERRCLR (1 << 17)
#define FLASH_CCR_PGSERRCLR (1 << 18)
#define FLASH_CCR_STRBERRCLR (1 << 19)
#define FLASH_CCR_INCERRCLR (1 << 21)
#define FLASH_CCR_OPERRCLR (1 << 22)
#define FLASH_CCR_RDPERRCLR (1 << 23)
#define FLASH_CCR_RDSERRCLR (1 << 24)
#define FLASH_CCR_SNECCERRCLR (1 << 25)
#define FLASH_CCR_DBECCERRCLR (1 << 26)
#define FLASH_CCR_CRCENDCLR (1 << 27)
#define FLASH_CCR_CRCRDERRCLR (1 << 28)

/* --- FLASH_OPTCR values -------------------------------------------------- */

#define FLASH_OPTCR_OPTCHANGEERRIE (1 << 30)
#define FLASH_OPTCR_OPTSTRT (1 << 1)
#define FLASH_OPTCR_OPTLOCK (1 << 0)

/** @defgroup flash_cr_program_width Flash programming width
@ingroup flash_group

@{*/
#define FLASH_CR_PROGRAM_X8 0
#define FLASH_CR_PROGRAM_X16 1
#define FLASH_CR_PROGRAM_X32 2
#define FLASH_CR_PROGRAM_X64 3
/**@}*/

/* --- Function prototypes ------------------------------------------------- */

void flash_erase_all_sectors(uint32_t program_size);
void flash_erase_sector(uint8_t sector, uint32_t program_size);
void flash_program_double_word(uint32_t address, uint64_t data);
void flash_program_word(uint32_t address, uint32_t data);
void flash_program_half_word(uint32_t address, uint16_t data);
void flash_program_byte(uint32_t address, uint8_t data);
void flash_program(uint32_t address, const uint8_t *data, uint32_t len);
void flash_program_option_bytes(uint32_t data);
void flash_unlock(void);
void flash_lock(void);
void flash_unlock_option_bytes(void);
void flash_lock_option_bytes(void);

void flash_clear_wrperr_flag(void);
void flash_clear_pgserr_flag(void);
void flash_clear_strberr_flag(void);
void flash_clear_incerr_flag(void);
void flash_clear_operr_flag(void);
void flash_clear_rdperr_flag(void);
void flash_clear_rdserr_flag(void);
void flash_clear_sneccerr_flag(void);
void flash_clear_dbeccerr_flag(void);
void flash_clear_crcend_flag(void);
void flash_clear_crcrderr_flag(void);
void flash_clear_eop_flag(void);
void flash_clear_status_flags(void);

void flash_wait_for_last_operation(void);
void flash_set_ws(uint32_t ws);

/**@}*/

#endif
