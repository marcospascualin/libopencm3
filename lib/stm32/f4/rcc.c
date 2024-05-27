/** @defgroup rcc_file RCC peripheral API
 *
 * @ingroup peripheral_apis
 *
 * @section rcc_f4_api_ex Reset and Clock Control API.
 *
 * @brief <b>libopencm3 STM32F4xx Reset and Clock Control</b>
 *
 * @author @htmlonly &copy; @endhtmlonly 2013 Frantisek Burian <BuFran at seznam.cz>
 *
 * @date 18 Jun 2013
 *
 * This library supports the Reset and Clock Control System in the STM32 series
 * of ARM Cortex Microcontrollers by ST Microelectronics.
 *
 * LGPL License Terms @ref lgpl_license
 */

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Federico Ruiz-Ugalde <memeruiz at gmail dot com>
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
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

#include <libopencm3/cm3/assert.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/flash.h>

/**@{*/

/* Set the default clock frequencies after reset. */
uint32_t rcc_ahb_frequency = 16000000;
uint32_t rcc_apb1_frequency = 16000000;
uint32_t rcc_apb2_frequency = 16000000;

const struct rcc_clock_scale rcc_hsi_configs[RCC_CLOCK_3V3_END] = {
	{ /* 84MHz */
		.pllm = 16,
		.plln = 336,
		.pllp = 4,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSI_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV2,
		.ppre2 = RCC_CFGR_PPRE_NODIV,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_2WS,
		.ahb_frequency  = 84000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 96MHz */
		.pllm = 8,
		.plln = 96,
		.pllp = 2,
		.pllq = 4,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSI_CLK,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_3WS,
		.ahb_frequency  = 96000000,
		.apb1_frequency = 48000000,
		.apb2_frequency = 96000000
	},	
	{ /* 168MHz */
		.pllm = 16,
		.plln = 336,
		.pllp = 2,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSI_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 168000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 180MHz */
		.pllm = 16,
		.plln = 360,
		.pllp = 2,
		.pllq = 8,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSI_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 180000000,
		.apb1_frequency = 45000000,
		.apb2_frequency = 90000000,
	},
};

const struct rcc_clock_scale rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_END] = {
	{ /* 84MHz */
		.pllm = 8,
		.plln = 336,
		.pllp = 4,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV2,
		.ppre2 = RCC_CFGR_PPRE_NODIV,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_2WS,
		.ahb_frequency  = 84000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 96MHz */
		.pllm = 4,
		.plln = 96,
		.pllp = 2,
		.pllq = 4,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_3WS,
		.ahb_frequency  = 96000000,
		.apb1_frequency = 48000000,
		.apb2_frequency = 96000000
	},	
	{ /* 168MHz */
		.pllm = 8,
		.plln = 336,
		.pllp = 2,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 168000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 180MHz */
		.pllm = 8,
		.plln = 360,
		.pllp = 2,
		.pllq = 8,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 180000000,
		.apb1_frequency = 45000000,
		.apb2_frequency = 90000000,
	},
};

const struct rcc_clock_scale rcc_hse_12mhz_3v3[RCC_CLOCK_3V3_END] = {
	{ /* 84MHz */
		.pllm = 12,
		.plln = 336,
		.pllp = 4,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV2,
		.ppre2 = RCC_CFGR_PPRE_NODIV,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_2WS,
		.ahb_frequency  = 84000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 96MHz */
		.pllm = 6,
		.plln = 96,
		.pllp = 2,
		.pllq = 4,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_3WS,
		.ahb_frequency  = 96000000,
		.apb1_frequency = 48000000,
		.apb2_frequency = 96000000
	},	
	{ /* 168MHz */
		.pllm = 12,
		.plln = 336,
		.pllp = 2,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 168000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 180MHz */
		.pllm = 12,
		.plln = 360,
		.pllp = 2,
		.pllq = 8,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 180000000,
		.apb1_frequency = 45000000,
		.apb2_frequency = 90000000,
	},
};

const struct rcc_clock_scale rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_END] = {
	{ /* 84MHz */
		.pllm = 16,
		.plln = 336,
		.pllp = 4,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV2,
		.ppre2 = RCC_CFGR_PPRE_NODIV,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_2WS,
		.ahb_frequency  = 84000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 96MHz */
		.pllm = 8,
		.plln = 96,
		.pllp = 2,
		.pllq = 4,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_3WS,
		.ahb_frequency  = 96000000,
		.apb1_frequency = 48000000,
		.apb2_frequency = 96000000
	},	
	{ /* 168MHz */
		.pllm = 16,
		.plln = 336,
		.pllp = 2,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 168000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 180MHz */
		.pllm = 16,
		.plln = 360,
		.pllp = 2,
		.pllq = 8,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 180000000,
		.apb1_frequency = 45000000,
		.apb2_frequency = 90000000,
	},
};

const struct rcc_clock_scale rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_END] = {
	{ /* 84MHz */
		.pllm = 25,
		.plln = 336,
		.pllp = 4,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV2,
		.ppre2 = RCC_CFGR_PPRE_NODIV,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_2WS,
		.ahb_frequency  = 84000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 96MHz */
		.pllm = 25,
		.plln = 192,
		.pllp = 2,
		.pllq = 4,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_DIV_NONE,
		.ppre1 = RCC_CFGR_PPRE_DIV_2,
		.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_3WS,
		.ahb_frequency  = 96000000,
		.apb1_frequency = 48000000,
		.apb2_frequency = 96000000
	},	
	{ /* 168MHz */
		.pllm = 25,
		.plln = 336,
		.pllp = 2,
		.pllq = 7,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 168000000,
		.apb1_frequency = 42000000,
		.apb2_frequency = 84000000,
	},
	{ /* 180MHz */
		.pllm = 25,
		.plln = 360,
		.pllp = 2,
		.pllq = 8,
		.pllr = 0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
		.hpre = RCC_CFGR_HPRE_NODIV,
		.ppre1 = RCC_CFGR_PPRE_DIV4,
		.ppre2 = RCC_CFGR_PPRE_DIV2,
		.voltage_scale = PWR_SCALE1,
		.flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN |
				FLASH_ACR_LATENCY_5WS,
		.ahb_frequency  = 180000000,
		.apb1_frequency = 45000000,
		.apb2_frequency = 90000000,
	},
};

void rcc_osc_ready_int_clear(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		RCC_CIR |= RCC_CIR_PLLRDYC;
		break;
	case RCC_HSE:
		RCC_CIR |= RCC_CIR_HSERDYC;
		break;
	case RCC_HSI:
		RCC_CIR |= RCC_CIR_HSIRDYC;
		break;
	case RCC_LSE:
		RCC_CIR |= RCC_CIR_LSERDYC;
		break;
	case RCC_LSI:
		RCC_CIR |= RCC_CIR_LSIRDYC;
		break;
	case RCC_PLLSAI:
		RCC_CIR |= RCC_CIR_PLLSAIRDYC;
		break;
	case RCC_PLLI2S:
		RCC_CIR |= RCC_CIR_PLLI2SRDYC;
		break;
	}
}

void rcc_osc_ready_int_enable(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		RCC_CIR |= RCC_CIR_PLLRDYIE;
		break;
	case RCC_HSE:
		RCC_CIR |= RCC_CIR_HSERDYIE;
		break;
	case RCC_HSI:
		RCC_CIR |= RCC_CIR_HSIRDYIE;
		break;
	case RCC_LSE:
		RCC_CIR |= RCC_CIR_LSERDYIE;
		break;
	case RCC_LSI:
		RCC_CIR |= RCC_CIR_LSIRDYIE;
		break;
	case RCC_PLLSAI:
		RCC_CIR |= RCC_CIR_PLLSAIRDYIE;
		break;
	case RCC_PLLI2S:
		RCC_CIR |= RCC_CIR_PLLI2SRDYIE;
		break;
	}
}

void rcc_osc_ready_int_disable(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		RCC_CIR &= ~RCC_CIR_PLLRDYIE;
		break;
	case RCC_HSE:
		RCC_CIR &= ~RCC_CIR_HSERDYIE;
		break;
	case RCC_HSI:
		RCC_CIR &= ~RCC_CIR_HSIRDYIE;
		break;
	case RCC_LSE:
		RCC_CIR &= ~RCC_CIR_LSERDYIE;
		break;
	case RCC_LSI:
		RCC_CIR &= ~RCC_CIR_LSIRDYIE;
		break;
	case RCC_PLLSAI:
		RCC_CIR &= ~RCC_CIR_PLLSAIRDYIE;
		break;
	case RCC_PLLI2S:
		RCC_CIR &= ~RCC_CIR_PLLI2SRDYIE;
		break;
	}
}

int rcc_osc_ready_int_flag(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		return ((RCC_CIR & RCC_CIR_PLLRDYF) != 0);
	case RCC_HSE:
		return ((RCC_CIR & RCC_CIR_HSERDYF) != 0);
	case RCC_HSI:
		return ((RCC_CIR & RCC_CIR_HSIRDYF) != 0);
	case RCC_LSE:
		return ((RCC_CIR & RCC_CIR_LSERDYF) != 0);
	case RCC_LSI:
		return ((RCC_CIR & RCC_CIR_LSIRDYF) != 0);
	case RCC_PLLSAI:
		return ((RCC_CIR & RCC_CIR_PLLSAIRDYF) != 0);
	case RCC_PLLI2S:
		return ((RCC_CIR & RCC_CIR_PLLI2SRDYF) != 0);
	}
	return 0;
}

void rcc_css_int_clear(void)
{
	RCC_CIR |= RCC_CIR_CSSC;
}

int rcc_css_int_flag(void)
{
	return ((RCC_CIR & RCC_CIR_CSSF) != 0);
}

bool rcc_is_osc_ready(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		return RCC_CR & RCC_CR_PLLRDY;
	case RCC_HSE:
		return RCC_CR & RCC_CR_HSERDY;
	case RCC_HSI:
		return RCC_CR & RCC_CR_HSIRDY;
	case RCC_LSE:
		return RCC_BDCR & RCC_BDCR_LSERDY;
	case RCC_LSI:
		return RCC_CSR & RCC_CSR_LSIRDY;
	case RCC_PLLSAI:
		return RCC_CR & RCC_CR_PLLSAIRDY;
	case RCC_PLLI2S:
		return RCC_CR & RCC_CR_PLLI2SRDY;
	}
	return false;
}

void rcc_wait_for_osc_ready(enum rcc_osc osc)
{
	while (!rcc_is_osc_ready(osc));
}

void rcc_wait_for_sysclk_status(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		while (((RCC_CFGR >> RCC_CFGR_SWS_SHIFT) & RCC_CFGR_SWS_MASK) !=
			RCC_CFGR_SWS_PLL);
		break;
	case RCC_HSE:
		while (((RCC_CFGR >> RCC_CFGR_SWS_SHIFT) & RCC_CFGR_SWS_MASK) !=
			RCC_CFGR_SWS_HSE);
		break;
	case RCC_HSI:
		while (((RCC_CFGR >> RCC_CFGR_SWS_SHIFT) & RCC_CFGR_SWS_MASK) !=
			RCC_CFGR_SWS_HSI);
		break;
	default:
		/* Shouldn't be reached. */
		break;
	}
}

void rcc_osc_on(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		RCC_CR |= RCC_CR_PLLON;
		break;
	case RCC_HSE:
		RCC_CR |= RCC_CR_HSEON;
		break;
	case RCC_HSI:
		RCC_CR |= RCC_CR_HSION;
		break;
	case RCC_LSE:
		RCC_BDCR |= RCC_BDCR_LSEON;
		break;
	case RCC_LSI:
		RCC_CSR |= RCC_CSR_LSION;
		break;
	case RCC_PLLSAI:
		RCC_CR |= RCC_CR_PLLSAION;
		break;
	case RCC_PLLI2S:
		RCC_CR |= RCC_CR_PLLI2SON;
		break;
	}
}

void rcc_osc_off(enum rcc_osc osc)
{
	switch (osc) {
	case RCC_PLL:
		RCC_CR &= ~RCC_CR_PLLON;
		break;
	case RCC_HSE:
		RCC_CR &= ~RCC_CR_HSEON;
		break;
	case RCC_HSI:
		RCC_CR &= ~RCC_CR_HSION;
		break;
	case RCC_LSE:
		RCC_BDCR &= ~RCC_BDCR_LSEON;
		break;
	case RCC_LSI:
		RCC_CSR &= ~RCC_CSR_LSION;
		break;
	case RCC_PLLSAI:
		RCC_CR &= ~RCC_CR_PLLSAION;
		break;
	case RCC_PLLI2S:
		RCC_CR &= ~RCC_CR_PLLI2SON;
		break;
	}
}

void rcc_css_enable(void)
{
	RCC_CR |= RCC_CR_CSSON;
}

void rcc_css_disable(void)
{
	RCC_CR &= ~RCC_CR_CSSON;
}

/**
 * Set the dividers for the PLLI2S clock outputs
 * @param n valid range depends on target device, check your RefManual.
 * @param r valid range is 2..7
 */
void rcc_plli2s_config(uint16_t n, uint8_t r)
{
	RCC_PLLI2SCFGR = (
	  ((n & RCC_PLLI2SCFGR_PLLI2SN_MASK) << RCC_PLLI2SCFGR_PLLI2SN_SHIFT) |
	  ((r & RCC_PLLI2SCFGR_PLLI2SR_MASK) << RCC_PLLI2SCFGR_PLLI2SR_SHIFT));
}

/**
 * Set the dividers for the PLLSAI clock outputs
 * divider p is only available on F4x9 parts, pass 0 for other parts.
 * @param n valid range is 49..432
 * @param p 0 if unused, @ref rcc_pllsaicfgr_pllsaip
 * @param q valid range is 2..15
 * @param r valid range is 2..7
 * @sa rcc_pllsai_postscalers
 */
void rcc_pllsai_config(uint16_t n, uint16_t p, uint16_t q, uint16_t r)
{
	RCC_PLLSAICFGR = (
	  ((n & RCC_PLLSAICFGR_PLLSAIN_MASK) << RCC_PLLSAICFGR_PLLSAIN_SHIFT) |
	  ((p & RCC_PLLSAICFGR_PLLSAIP_MASK) << RCC_PLLSAICFGR_PLLSAIP_SHIFT) |
	  ((q & RCC_PLLSAICFGR_PLLSAIQ_MASK) << RCC_PLLSAICFGR_PLLSAIQ_SHIFT) |
	  ((r & RCC_PLLSAICFGR_PLLSAIR_MASK) << RCC_PLLSAICFGR_PLLSAIR_SHIFT));
}


/**
 * Set the dedicated dividers after the PLLSAI configuration.
 *
 * @param q dedicated PLLSAI divider, for either A or B
 * @param r dedicated LCD-TFT divider, see LTDC
 * @sa rcc_pllsai_config
 */
void rcc_pllsai_postscalers(uint8_t q, uint8_t r)
{
	uint32_t reg32 = RCC_DCKCFGR;
	reg32 &= ((RCC_DCKCFGR_PLLSAIDIVR_MASK << RCC_DCKCFGR_PLLSAIDIVR_SHIFT)
		| (RCC_DCKCFGR_PLLSAIDIVQ_MASK << RCC_DCKCFGR_PLLSAIDIVQ_SHIFT));
	RCC_DCKCFGR = reg32 | ((q << RCC_DCKCFGR_PLLSAIDIVQ_SHIFT) |
		(r << RCC_DCKCFGR_PLLSAIDIVR_SHIFT));
}


void rcc_set_sysclk_source(uint32_t clk)
{
	uint32_t reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 1) | (1 << 0));
	RCC_CFGR = (reg32 | clk);
}

void rcc_set_pll_source(uint32_t pllsrc)
{
	uint32_t reg32;

	reg32 = RCC_PLLCFGR;
	reg32 &= ~(1 << 22);
	RCC_PLLCFGR = (reg32 | (pllsrc << 22));
}

void rcc_set_ppre2(uint32_t ppre2)
{
	uint32_t reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 13) | (1 << 14) | (1 << 15));
	RCC_CFGR = (reg32 | (ppre2 << 13));
}

void rcc_set_ppre1(uint32_t ppre1)
{
	uint32_t reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 10) | (1 << 11) | (1 << 12));
	RCC_CFGR = (reg32 | (ppre1 << 10));
}

void rcc_set_hpre(uint32_t hpre)
{
	uint32_t reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
	RCC_CFGR = (reg32 | (hpre << 4));
}

void rcc_set_rtcpre(uint32_t rtcpre)
{
	uint32_t reg32;

	reg32 = RCC_CFGR;
	reg32 &= ~((1 << 16) | (1 << 17) | (1 << 18) | (1 << 19) | (1 << 20));
	RCC_CFGR = (reg32 | (rtcpre << 16));
}

/**
 * Reconfigures the main PLL for a HSI source.
 * Any reserved bits are kept at their reset values.
 * @param pllm Divider for the main PLL input clock
 * @param plln Main PLL multiplication factor for VCO
 * @param pllp Main PLL divider for main system clock
 * @param pllq Main PLL divider for USB OTG FS, SDMMC & RNG
 * @param pllr Main PLL divider for DSI (for parts without DSI, provide 0 here)
 */
void rcc_set_main_pll_hsi(uint32_t pllm, uint32_t plln, uint32_t pllp,
			  uint32_t pllq, uint32_t pllr)
{
	/* Use reset value if not legal, for parts without pllr */
	if (pllr < 2) {
		pllr = 2;
	}
	RCC_PLLCFGR = 0 | /* HSI */
		((pllm & RCC_PLLCFGR_PLLM_MASK) << RCC_PLLCFGR_PLLM_SHIFT) |
		((plln & RCC_PLLCFGR_PLLN_MASK) << RCC_PLLCFGR_PLLN_SHIFT) |
		((((pllp >> 1) - 1) & RCC_PLLCFGR_PLLP_MASK) << RCC_PLLCFGR_PLLP_SHIFT) |
		((pllq & RCC_PLLCFGR_PLLQ_MASK) << RCC_PLLCFGR_PLLQ_SHIFT) |
		((pllr & RCC_PLLCFGR_PLLR_MASK) << RCC_PLLCFGR_PLLR_SHIFT);
}

/**
 * Reconfigures the main PLL for a HSE source.
 * Any reserved bits are kept at their reset values.
 * @param pllm Divider for the main PLL input clock
 * @param plln Main PLL multiplication factor for VCO
 * @param pllp Main PLL divider for main system clock
 * @param pllq Main PLL divider for USB OTG FS, SDMMC & RNG
 * @param pllr Main PLL divider for DSI (for parts without DSI, provide 0 here)
 */
void rcc_set_main_pll_hse(uint32_t pllm, uint32_t plln, uint32_t pllp,
			  uint32_t pllq, uint32_t pllr)
{
	/* Use reset value if not legal, for parts without pllr */
	if (pllr < 2) {
		pllr = 2;
	}
	RCC_PLLCFGR = RCC_PLLCFGR_PLLSRC | /* HSE */
		((pllm & RCC_PLLCFGR_PLLM_MASK) << RCC_PLLCFGR_PLLM_SHIFT) |
		((plln & RCC_PLLCFGR_PLLN_MASK) << RCC_PLLCFGR_PLLN_SHIFT) |
		((((pllp >> 1) - 1) & RCC_PLLCFGR_PLLP_MASK) << RCC_PLLCFGR_PLLP_SHIFT) |
		((pllq & RCC_PLLCFGR_PLLQ_MASK) << RCC_PLLCFGR_PLLQ_SHIFT) |
		((pllr & RCC_PLLCFGR_PLLR_MASK) << RCC_PLLCFGR_PLLR_SHIFT);
}

uint32_t rcc_system_clock_source(void)
{
	/* Return the clock source which is used as system clock. */
	return (RCC_CFGR & 0x000c) >> 2;
}

/**
 * Setup clocks to run from PLL.
 *
 * The arguments provide the pll source, multipliers, dividers, all that's
 * needed to establish a system clock.
 *
 * @param clock clock information structure.
 */
void rcc_clock_setup_pll(const struct rcc_clock_scale *clock)
{
	/* Enable internal high-speed oscillator (HSI). */
	rcc_osc_on(RCC_HSI);
	rcc_wait_for_osc_ready(RCC_HSI);

	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(RCC_CFGR_SW_HSI);

	/* Enable external high-speed oscillator (HSE). */
	if (clock->pll_source == RCC_CFGR_PLLSRC_HSE_CLK) {
		rcc_osc_on(RCC_HSE);
		rcc_wait_for_osc_ready(RCC_HSE);
	}

	/* Set the VOS scale mode */
	rcc_periph_clock_enable(RCC_PWR);
	pwr_set_vos_scale(clock->voltage_scale);

	/*
	 * Set prescalers for AHB, ADC, APB1, APB2.
	 * Do this before touching the PLL (TODO: why?).
	 */
	rcc_set_hpre(clock->hpre);
	rcc_set_ppre1(clock->ppre1);
	rcc_set_ppre2(clock->ppre2);

	/* Disable PLL oscillator before changing its configuration. */
	rcc_osc_off(RCC_PLL);

	/* Configure the PLL oscillator. */
	if (clock->pll_source == RCC_CFGR_PLLSRC_HSE_CLK) {
		rcc_set_main_pll_hse(clock->pllm, clock->plln,
				clock->pllp, clock->pllq, clock->pllr);
	} else {
		rcc_set_main_pll_hsi(clock->pllm, clock->plln,
				clock->pllp, clock->pllq, clock->pllr);
	}

	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(RCC_PLL);
	rcc_wait_for_osc_ready(RCC_PLL);

	/* Configure flash settings. */
	if (clock->flash_config & FLASH_ACR_DCEN) {
		flash_dcache_enable();
	} else {
		flash_dcache_disable();
	}
	if (clock->flash_config & FLASH_ACR_ICEN) {
		flash_icache_enable();
	} else {
		flash_icache_disable();
	}
	flash_set_ws(clock->flash_config);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(RCC_CFGR_SW_PLL);

	/* Wait for PLL clock to be selected. */
	rcc_wait_for_sysclk_status(RCC_PLL);

	/* Set the peripheral clock frequencies used. */
	rcc_ahb_frequency  = clock->ahb_frequency;
	rcc_apb1_frequency = clock->apb1_frequency;
	rcc_apb2_frequency = clock->apb2_frequency;

	/* Disable internal high-speed oscillator. */
	if (clock->pll_source == RCC_CFGR_PLLSRC_HSE_CLK) {
		rcc_osc_off(RCC_HSI);
	}
}

/**
 * Setup clocks with the HSE.
 *
 * @deprecated replaced by rcc_clock_setup_pll as a drop in replacement.
 * @see rcc_clock_setup_pll which supports HSI as well as HSE, using the same
 * clock structures.
 */
void rcc_clock_setup_hse_3v3(const struct rcc_clock_scale *clock)
{
	rcc_clock_setup_pll(clock);
}

/*---------------------------------------------------------------------------*/
/** @brief Get the peripheral clock speed for the USART at base specified.
 * @param usart  Base address of USART to get clock frequency for.
 */
uint32_t rcc_get_usart_clk_freq(uint32_t usart)
{
	/* Handle values with selectable clocks. */
	if (usart == USART1_BASE || usart == USART6_BASE) {
		return rcc_apb2_frequency;
	} else {
		return rcc_apb1_frequency;
	}
}

/*---------------------------------------------------------------------------*/
/** @brief Get the peripheral clock speed for the Timer at base specified.
 * @param timer  Base address of TIM to get clock frequency for.
 */
uint32_t rcc_get_timer_clk_freq(uint32_t timer)
{
	/* Handle APB1 timer clocks. */
	if (timer >= TIM2_BASE && timer <= TIM14_BASE) {
		uint8_t ppre1 = (RCC_CFGR >> RCC_CFGR_PPRE1_SHIFT) & RCC_CFGR_PPRE1_MASK;
		return (ppre1 == RCC_CFGR_PPRE_DIV_NONE) ? rcc_apb1_frequency
			: 2 * rcc_apb1_frequency;
	} else {
		uint8_t ppre2 = (RCC_CFGR >> RCC_CFGR_PPRE2_SHIFT) & RCC_CFGR_PPRE2_MASK;
		return (ppre2 == RCC_CFGR_PPRE_DIV_NONE) ? rcc_apb2_frequency
			: 2 * rcc_apb2_frequency;
	}
}

/*---------------------------------------------------------------------------*/
/** @brief Get the peripheral clock speed for the I2C device at base specified.
 * @param i2c  Base address of I2C to get clock frequency for.
 */
uint32_t rcc_get_i2c_clk_freq(uint32_t i2c __attribute__((unused)))
{
	return rcc_apb1_frequency;
}

/*---------------------------------------------------------------------------*/
/** @brief Get the peripheral clock speed for the SPI device at base specified.
 * @param spi  Base address of SPI device to get clock frequency for (e.g. SPI1_BASE).
 */
uint32_t rcc_get_spi_clk_freq(uint32_t spi) {
	if (spi == SPI2_BASE || spi == SPI3_BASE) {
		return rcc_apb1_frequency;
	} else {
		return rcc_apb2_frequency;
	}
}



//
// void rcc_set_rtc_clock_source(enum rcc_osc rtcclk_source)
// {
// uint32_t reg32;
//
// /* Clear the RTC clock source selection bits */
// RCC_BDCR &= ~(RCC_BDCR_RTCSEL_MASK<< RCC_BDCR_RTCSEL_SHIFT);
//
// /* Set the RTC clock source selection bits */
// switch (rtcclk_source) {
// case RCC_LSE:
// RCC_BDCR |= RCC_BDCR_RTCSEL_LSE << RCC_BDCR_RTCSEL_SHIFT;
// break;
// case RCC_LSI:
// RCC_BDCR |= RCC_BDCR_RTCSEL_LSI << RCC_BDCR_RTCSEL_SHIFT;
// break;
// case RCC_HSE:
// RCC_BDCR |= RCC_BDCR_RTCSEL_HSE << RCC_BDCR_RTCSEL_SHIFT;
// break;
// }
//
// /* Wait for the RTC clock source to be ready */
// reg32 = RCC_BDCR;
// while ((reg32 & (RCC_BDCR_RTCSEL_MASK << RCC_BDCR_RTCSEL_SHIFT))!= (rtcclk_source << RCC_BDCR_RTCSEL_SHIFT))
// {
// reg32 = RCC_BDCR;
// }
// }

void rcc_enable_rtc_clock(void)
{
  RCC_BDCR |= RCC_BDCR_RTCEN;
}

void rcc_backupdomain_reset(void)
{
  RCC_BDCR |= RCC_BDCR_BDRST;
  RCC_BDCR &= ~RCC_BDCR_BDRST;
}
uint32_t rcc_get_hpre_prescaler(void)
{
  uint32_t hpre = (RCC_CFGR >> RCC_CFGR_HPRE_SHIFT) & RCC_CFGR_HPRE_MASK;
  switch (hpre)
  {
  case RCC_CFGR_HPRE_NODIV:
    return 1;
  case RCC_CFGR_HPRE_DIV_2:
    return 2;
  case RCC_CFGR_HPRE_DIV_4:
    return 4;
  case RCC_CFGR_HPRE_DIV_8:
    return 8;
  case RCC_CFGR_HPRE_DIV_16:
    return 16;
  case RCC_CFGR_HPRE_DIV_64:
    return 64;
  case RCC_CFGR_HPRE_DIV_128:
    return 128;
  case RCC_CFGR_HPRE_DIV_256:
    return 256;
  case RCC_CFGR_HPRE_DIV_512:
    return 512;
  default:
    /* Should not happen */
    return 1;
  }
}

static uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

void rcc_get_clocks_freq(rcc_clocks *clocks)
{
  uint32_t tmp = 0, presc = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2, plln = 1;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = (RCC_CFGR >> RCC_CFGR_SWS_SHIFT) & RCC_CFGR_SWS_MASK;

  switch (tmp)
  {
  case RCC_CFGR_SWS_HSI: /* HSI used as system clock source */
    clocks->sysclk_freq = HSI_VALUE;
    break;

  case RCC_CFGR_SWS_HSE: /* HSE used as system clock  source */
    clocks->sysclk_freq = HSE_VALUE;
    break;

  case RCC_CFGR_SWS_PLL: /* PLL used as system clock  source */

    /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
       SYSCLK = PLL_VCO / PLLP
       */
    pllsource = (RCC_PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
    pllm = (RCC_PLLCFGR >> RCC_PLLCFGR_PLLM_SHIFT) & RCC_PLLCFGR_PLLM_MASK;
    plln = ((RCC_PLLCFGR >> RCC_PLLCFGR_PLLN_SHIFT) & RCC_PLLCFGR_PLLN_MASK);

    if (pllsource != 0)
    {
      /* HSE used as PLL clock source */
      pllvco = (HSE_VALUE / pllm) * plln;
    }
    else
    {
      /* HSI used as PLL clock source */
      pllvco = (HSI_VALUE / pllm) * plln;
    }

    pllp = ((((RCC_PLLCFGR >> RCC_PLLCFGR_PLLP_SHIFT) & RCC_PLLCFGR_PLLP_MASK) /*>>16*/) + 1) * 2;
    clocks->sysclk_freq = pllvco / pllp;
    break;

  default:
    clocks->sysclk_freq = HSI_VALUE;
    break;
  }

  /* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/

  /* Get HCLK prescaler */
  tmp = (RCC_CFGR >> RCC_CFGR_HPRE_SHIFT) & RCC_CFGR_HPRE_MASK;

  presc = APBAHBPrescTable[tmp];

  /* HCLK clock frequency */
  clocks->hclk_freq = clocks->sysclk_freq >> presc;

  /* Get PCLK1 prescaler */
  tmp = (RCC_CFGR >> RCC_CFGR_PPRE1_SHIFT) & RCC_CFGR_PPRE1_MASK;

  presc = APBAHBPrescTable[tmp];

  /* PCLK1 clock frequency */
  clocks->pclk1_freq = clocks->hclk_freq >> presc;

  /* Get PCLK2 prescaler */
  tmp = (RCC_CFGR >> RCC_CFGR_PPRE2_SHIFT) & RCC_CFGR_PPRE2_MASK;

  presc = APBAHBPrescTable[tmp];

  /* PCLK2 clock frequency */
  clocks->pclk2_freq = clocks->hclk_freq >> presc;
}
/**
 * @brief  Clears the RCC reset flags.
 *         The reset flags are: RCC_FLAG_PINRST, RCC_FLAG_PORRST,  RCC_FLAG_SFTRST,
 *         RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST
 * @param  None
 * @retval None
 */
void rcc_clear_flag(void)
{
  /* Set RMVF bit to clear the reset flags */
  RCC_CSR |= RCC_CSR_RMVF;
}

uint8_t rcc_get_flag_status(uint32_t rcc_flag)
{
  //	 uint32_t tmp = 0;
  //	  uint32_t statusreg = 0;
  //	  uint8_t bitstatus = 0;
  //
  //	  /* Get the RCC register index */
  //	  tmp = rcc_flag >> 5;
  //	  if (tmp == 1)               /* The flag to check is in CR register */
  //	  {
  //	    statusreg = RCC_CR;
  //	  }
  //	  else if (tmp == 2)          /* The flag to check is in BDCR register */
  //	  {
  //	    statusreg = RCC_BDCR;
  //	  }
  //	  else                       /* The flag to check is in CSR register */
  //	  {
  //	    statusreg = RCC_CSR;
  //	  }
  //
  //	  /* Get the flag position */
  //	  tmp = rcc_flag & RCC_FLAG_MASK;
  //	  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)0)
  //	  {
  //	    bitstatus = 1;
  //	  }
  //	  else
  //	  {
  //	    bitstatus = 0;
  //	  }
  //	  /* Return the flag status */
  //	  return bitstatus;

  //	return ((RCC_CSR & rcc_flag) != 0);
  return ((RCC_CSR & rcc_flag) != 0) ||
         ((RCC_BDCR & rcc_flag) != 0) ||
         ((RCC_CR & rcc_flag) != 0);
}
/**@}*/
