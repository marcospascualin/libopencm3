/** @addtogroup fmc_file FMC peripheral API
@ingroup peripheral_apis

@author @htmlonly &copy; @endhtmlonly 2012 Ken Sarkies ksarkies@internode.on.net

This library supports the Flexible Memory Controller in the STM32F4xx and
STM32F7xx series of ARM Cortex Microcontrollers by ST Microelectronics.
*/
/*
 *
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

/* Utility functions for the SDRAM component of the FMC */

#include <stdint.h>
#include <libopencm3/stm32/fsmc.h>

/**@{*/

/*
 * Install various timing values into the correct place in the
 * SDRAM Timing Control Register format.
 *
 * Note that the register is 'zero' based to save bits so 1 cycle
 * is stored as '0'. This command takes actual cycles and adjusts
 * by subtracting 1.
 */
uint32_t
sdram_timing(struct sdram_timing *t)
{
	uint32_t result;

	result = 0;
	result |= ((t->trcd - 1) & 0xf) << FMC_SDTR_TRCD_SHIFT;
	result |= ((t->trp - 1) & 0xf) << FMC_SDTR_TRP_SHIFT;
	result |= ((t->twr - 1) & 0xf) << FMC_SDTR_TWR_SHIFT;
	result |= ((t->trc - 1) & 0xf) << FMC_SDTR_TRC_SHIFT;
	result |= ((t->tras - 1) & 0xf) << FMC_SDTR_TRAS_SHIFT;
	result |= ((t->txsr - 1) & 0xf) << FMC_SDTR_TXSR_SHIFT;
	result |= ((t->tmrd - 1) & 0xf) << FMC_SDTR_TMRD_SHIFT;
	return result;
}

/*
 * Send a command to the SDRAM controller, wait until it is not
 * busy before sending. This allows you to chain sending commands
 * and the code will pause as needed between them.
 */
void sdram_command(enum fmc_sdram_bank bank,
				   enum fmc_sdram_command cmd, int autorefresh, int modereg)
{
	uint32_t tmp_reg = 0;

	switch (bank)
	{
	case SDRAM_BANK1:
		tmp_reg = FMC_SDCMR_CTB1;
		break;
	case SDRAM_BANK2:
		tmp_reg = FMC_SDCMR_CTB2;
		break;
	case SDRAM_BOTH_BANKS:
		tmp_reg = FMC_SDCMR_CTB1 | FMC_SDCMR_CTB2;
		break;
	}
	tmp_reg |= autorefresh << FMC_SDCMR_NRFS_SHIFT;
	tmp_reg |= modereg << FMC_SDCMR_MRD_SHIFT;
	switch (cmd)
	{
	case SDRAM_CLK_CONF:
		tmp_reg |= FMC_SDCMR_MODE_CLOCK_CONFIG_ENA;
		break;
	case SDRAM_AUTO_REFRESH:
		tmp_reg |= FMC_SDCMR_MODE_AUTO_REFRESH;
		break;
	case SDRAM_LOAD_MODE:
		tmp_reg |= FMC_SDCMR_MODE_LOAD_MODE_REGISTER;
		break;
	case SDRAM_PALL:
		tmp_reg |= FMC_SDCMR_MODE_PALL;
		break;
	case SDRAM_SELF_REFRESH:
		tmp_reg |= FMC_SDCMR_MODE_SELF_REFRESH;
		break;
	case SDRAM_POWER_DOWN:
		tmp_reg |= FMC_SDCMR_MODE_POWER_DOWN;
		break;
	case SDRAM_NORMAL:
	default:
		break;
	}

	/* Wait for the next chance to talk to the controller */
	while (FMC_SDSR & FMC_SDSR_BUSY)
		;

	/* Send the next command */
	FMC_SDCMR = tmp_reg;
}

void fsmc_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FMC_BCR_MBKEN_SET;
	else
		FSMC_BCR(bank - 1) &= FMC_BCR_MBKEN_RESET;
}

void fsmc_data_address_mux_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_MUXEN;
	else
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_MUXEN;
}

void fsmc_asynchronous_wait_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_ASYNCWAIT;
	else
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_ASYNCWAIT;
}

void fsmc_set_wait_signal_polarity_low(uint8_t bank)
{
	FSMC_BCR(bank - 1) &= ~FSMC_BCR_WAITPOL;
}
void fsmc_set_wait_signal_polarity_high(uint8_t bank)
{
	FSMC_BCR(bank - 1) |= FSMC_BCR_WAITPOL;
}
void fsmc_wait_signal_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_WAITEN;
	else
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_WAITEN;
}

void fsmc_set_wait_signal_active_before(uint8_t bank)
{
	FSMC_BCR(bank - 1) &= ~FSMC_BCR_WAITCFG;
}
void fsmc_set_wait_signal_active_during(uint8_t bank)
{
	FSMC_BCR(bank - 1) |= FSMC_BCR_WAITCFG;
}

void fsmc_set_data_bus_width_16b(uint8_t bank)
{
	FSMC_BCR(bank - 1) |= FSMC_BCR_MWID;
}
void fsmc_set_data_bus_width_8b(uint8_t bank)
{
	FSMC_BCR(bank - 1) &= ~FSMC_BCR_MWID;
}

void fsmc_wrap_mode_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_WRAPMOD;
	else
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_WRAPMOD;
}

void fsmc_burst_access_mode_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_BURSTEN;
	else
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_BURSTEN;
}

void fsmc_write_operation_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_WREN;
	else
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_WREN;
}

void fsmc_write_burst_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_CBURSTRW;
	else
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_CBURSTRW;
}

void fsmc_extended_mode_enable(uint8_t bank, bool enable)
{
	if (enable)
		FSMC_BCR(bank - 1) |= FSMC_BCR_EXTMOD;
	else
	{
		FSMC_BCR(bank - 1) &= ~FSMC_BCR_EXTMOD;
		FSMC_BWTR(bank - 1) = 0x0FFFFFFF;
	}
}

void fsmc_set_memory_type(uint8_t bank, uint8_t mem_type)
{
	// clear bits 2 and 3
	FSMC_BCR(bank - 1) &= ~(FSMC_BCR_MTYP_PSRAM | FSMC_BCR_MTYP_NOR | FSMC_BCR_MTYP_SRAM);

	FSMC_BCR(bank - 1) |= mem_type;

	if (mem_type == FSMC_BCR_MTYP_NOR)
	{
		FSMC_BCR(bank - 1) |= (uint32_t)FSMC_BCR_FACCEN;
	}
	else
	{
		FSMC_BCR(bank - 1) &= ~(uint32_t)FSMC_BCR_FACCEN;
	}
}

void fsmc_rw_timing_config(uint8_t bank, uint32_t add_st, uint32_t add_hld, uint32_t data_st, uint32_t bus_turn, uint32_t clk_div, uint32_t data_lat, uint32_t acc_mode)
{
	uint32_t tmpbtr = 0;
	//  tmpbtr = FSMC_BTR(bank-1);
	//  /* Clear ADDSET, ADDHLD, DATAST, BUSTURN, CLKDIV, DATLAT and ACCMOD bits */
	//  tmpbtr &= ((uint32_t)~(FSMC_BTR_ADDSET | FSMC_BTR_ADDHLD | FSMC_BTR_DATAST | FSMC_BTR_BUSTURN | FSMC_BTR_CLKDIV | FSMC_BTR_DATLAT | FSMC_BTR_ACCMOD));

	tmpbtr |= (uint32_t)(FSMC_BTR_ADDSETx(add_st) | FSMC_BTR_ADDHLDx(add_hld) |
						 FSMC_BTR_DATASTx(data_st) | FSMC_BTR_BUSTURNx(bus_turn) |
						 FSMC_BTR_CLKDIVx(clk_div) | FSMC_BTR_DATLATx(data_lat) | FSMC_BTR_ACCMODx(acc_mode));
	FSMC_BTR(bank - 1) = tmpbtr;
}

// ONLY IF EXTENDED MODE IS USED
void fsmc_write_timing_config(uint8_t bank, uint32_t add_st, uint32_t add_hld, uint32_t data_st, uint32_t clk_div, uint32_t data_lat, uint32_t acc_mode)
{
	uint32_t tmpbwr = 0;
	tmpbwr = FSMC_BWTR(bank - 1);
	/* Clear ADDSET, ADDHLD, DATAST, BUSTURN, CLKDIV, DATLAT and ACCMOD bits */
	tmpbwr &= ((uint32_t) ~(FSMC_BWTR_ADDSET | FSMC_BWTR_ADDHLD | FSMC_BWTR_DATAST |
							FSMC_BWTR_CLKDIV | FSMC_BWTR_DATLAT | FSMC_BWTR_ACCMOD));

	tmpbwr |= FSMC_BWTR_ADDSETx(add_st) | FSMC_BWTR_ADDHLDx(add_hld) |
			  FSMC_BWTR_DATASTx(data_st) | FSMC_BWTR_CLKDIVx(clk_div) |
			  FSMC_BWTR_DATLATx(data_lat) | FSMC_BWTR_ACCMODx(acc_mode);
	FSMC_BWTR(bank - 1) = tmpbwr;
}

/**@}*/
