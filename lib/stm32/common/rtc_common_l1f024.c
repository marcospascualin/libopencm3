/** @addtogroup rtc_file RTC peripheral API
@ingroup peripheral_apis

@author @htmlonly &copy; @endhtmlonly 2012 Karl Palsson <karlp@tweak.net.au>

*/

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Karl Palsson <karlp@tweak.net.au>
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

#include <libopencm3/stm32/rtc.h>

static uint8_t _rtc_dec_to_bcd(uint8_t dec)
{
	return ((dec / 10) << 4) | (dec % 10);
}

/*---------------------------------------------------------------------------*/
/** @brief Set RTC prescalars.

This sets the RTC synchronous and asynchronous prescalars.
*/

void rtc_set_prescaler(uint32_t sync, uint32_t async)
{
	/*
	 * Even if only one of the two fields needs to be changed,
	 * 2 separate write accesses must be performed to the RTC_PRER register.
	 */
	RTC_PRER = (sync & RTC_PRER_PREDIV_S_MASK);
	RTC_PRER |= (async << RTC_PRER_PREDIV_A_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Wait for RTC registers to be synchronised with the APB1 bus

 Time and Date are accessed through shadow registers which must be synchronized
*/

void rtc_wait_for_synchro(void)
{
	/* Unlock RTC registers */
	RTC_WPR = 0xca;
	RTC_WPR = 0x53;

	RTC_ISR &= ~(RTC_ISR_RSF);

	while (!(RTC_ISR & RTC_ISR_RSF))
		;

	/* disable write protection again */
	RTC_WPR = 0xff;
}

/*---------------------------------------------------------------------------*/
/** @brief Unlock write access to the RTC registers

*/
void rtc_unlock(void)
{
	RTC_WPR = 0xca;
	RTC_WPR = 0x53;
}

/*---------------------------------------------------------------------------*/
/** @brief Lock write access to the RTC registers

*/
void rtc_lock(void)
{
	RTC_WPR = 0xff;
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the wakeup time auto-reload value

*/
void rtc_set_wakeup_time(uint16_t wkup_time, uint8_t rtc_cr_wucksel)
{
	/* FTFM:
	 * The following sequence is required to configure or change the wakeup
	 * timer auto-reload value (WUT[15:0] in RTC_WUTR):
	 * 1. Clear WUTE in RTC_CR to disable the wakeup timer.
	 */
	RTC_CR &= ~RTC_CR_WUTE;
	/* 2. Poll WUTWF until it is set in RTC_ISR to make sure the access to
	 *    wakeup auto-reload counter and to WUCKSEL[2:0] bits is allowed.
	 *    It takes around 2 RTCCLK clock cycles (due to clock
	 *    synchronization).
	 */
	while (!((RTC_ISR) & (RTC_ISR_WUTWF)))
		;
	/* 3. Program the wakeup auto-reload value WUT[15:0], and the wakeup
	 *    clock selection (WUCKSEL[2:0] bits in RTC_CR).Set WUTE in RTC_CR
	 *    to enable the timer again. The wakeup timer restarts
	 *    down-counting.
	 */
	RTC_WUTR = wkup_time;
	RTC_CR &= ~(RTC_CR_WUCLKSEL_MASK << RTC_CR_WUCLKSEL_SHIFT);
	RTC_CR |= (rtc_cr_wucksel << RTC_CR_WUCLKSEL_SHIFT);
	RTC_CR |= RTC_CR_WUTE;
}

/*---------------------------------------------------------------------------*/
/** @brief Clears the wakeup flag

@details This function should be called first in the rtc_wkup_isr()
*/
void rtc_clear_wakeup_flag(void)
{
	RTC_ISR &= ~RTC_ISR_WUTF;
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the initialization flag

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
void rtc_set_init_flag(void)
{
	RTC_ISR |= RTC_ISR_INIT;
}

/*---------------------------------------------------------------------------*/
/** @brief Clears (resets) the initialization flag

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
void rtc_clear_init_flag(void)
{
	RTC_ISR &= ~RTC_ISR_INIT;
}

/*---------------------------------------------------------------------------*/
/** @brief Returns if the RTC_ISR init flag RTC_ISR_INITF is set

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
bool rtc_init_flag_is_ready(void)
{
	return (RTC_ISR & RTC_ISR_INITF);
}

/*---------------------------------------------------------------------------*/
/** @brief Waits infinitely for initialization flag to be set in RTC_ISR

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
void rtc_wait_for_init_ready(void)
{
	while (!rtc_init_flag_is_ready())
		;
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the bypass shadow bit in RTC_CR

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
void rtc_enable_bypass_shadow_register(void)
{
	RTC_CR |= RTC_CR_BYPSHAD;
}

/*---------------------------------------------------------------------------*/
/** @brief Clears the bypass shadow bit in RTC_CR

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
void rtc_disable_bypass_shadow_register(void)
{
	RTC_CR &= ~RTC_CR_BYPSHAD;
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC control register hour format to AM (24h)

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
void rtc_set_am_format(void)
{
	RTC_CR &= ~RTC_CR_FMT;
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC control register hour format to PM (12h)

@details Requires unlocking backup domain write protection (PWR_CR_DBP)
*/
void rtc_set_pm_format(void)
{
	RTC_CR |= RTC_CR_FMT;
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD calendar year value

@details Requires unlocking the RTC write-protection (RTC_WPR)

The year value should only be the abbreviated year tens, meaning if 2021 is
desired pass in only 21.
*/
void rtc_calendar_set_year(uint8_t year)
{
	uint8_t bcd_year = _rtc_dec_to_bcd(year);
	RTC_DR &= ~(RTC_DR_YT_MASK << RTC_DR_YT_SHIFT | RTC_DR_YU_MASK << RTC_DR_YU_SHIFT);
	RTC_DR |= (((bcd_year >> 4) & RTC_DR_YT_MASK) << RTC_DR_YT_SHIFT) |
			  ((bcd_year & RTC_DR_YU_MASK) << RTC_DR_YU_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD calendar weekday

@details Requires unlocking the RTC write-protection (RTC_WPR)
*/
void rtc_calendar_set_weekday(enum rtc_weekday rtc_dr_wdu)
{
	RTC_DR &= ~(RTC_DR_WDU_MASK << RTC_DR_WDU_SHIFT);
	RTC_DR |= (rtc_dr_wdu << RTC_DR_WDU_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD calendar month value

@details Requires unlocking the RTC write-protection (RTC_WPR)
*/
void rtc_calendar_set_month(uint8_t month)
{
	uint8_t bcd_month = _rtc_dec_to_bcd(month);
	RTC_DR &= ~(RTC_DR_MT_MASK << RTC_DR_MT_SHIFT | RTC_DR_MU_MASK << RTC_DR_MU_SHIFT);
	RTC_DR |= (((bcd_month >> 4) & RTC_DR_MT_MASK) << RTC_DR_MT_SHIFT) |
			  ((bcd_month & RTC_DR_MU_MASK) << RTC_DR_MU_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD calendar day value

@details Requires unlocking the RTC write-protection (RTC_WPR)
*/
void rtc_calendar_set_day(uint8_t day)
{
	uint8_t bcd_day = _rtc_dec_to_bcd(day);
	RTC_DR &= ~(RTC_DR_DT_MASK << RTC_DR_DT_SHIFT | RTC_DR_DU_MASK << RTC_DR_DU_SHIFT);
	RTC_DR |= (((bcd_day >> 4) & RTC_DR_DT_MASK) << RTC_DR_DT_SHIFT) |
			  ((bcd_day & RTC_DR_DU_MASK) << RTC_DR_DU_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD calendar value

@details Requires unlocking the RTC write-protection (RTC_WPR)

The year value should only be the abbreviated year tens, meaning if 2021 is
desired pass in only 21.
*/
void rtc_calendar_set_date(uint8_t year, uint8_t month, uint8_t day, enum rtc_weekday rtc_dr_wdu)
{
	rtc_calendar_set_year(year);
	rtc_calendar_set_month(month);
	rtc_calendar_set_weekday(rtc_dr_wdu);
	rtc_calendar_set_day(day);
}

void rtc_set_date(uint32_t format, uint8_t year, uint8_t month, uint8_t day, enum rtc_weekday rtc_dr_wdu)
{
	uint32_t tmpreg = 0;

	if ((format == RTC_Format_BIN) && ((month & 0x10) == 0x10))
	{
		month = (month & (uint32_t) ~(0x10)) + 0x0A;
	}

	/* Check the input parameters format */
	if (format != RTC_Format_BIN)
	{
		tmpreg = ((((uint32_t)year) << 16) |
				  (((uint32_t)month) << 8) |
				  ((uint32_t)day) |
				  (((uint32_t)rtc_dr_wdu) << 13));
	}
	else
	{
		tmpreg = (((uint32_t)_rtc_dec_to_bcd(year) << 16) |
				  ((uint32_t)_rtc_dec_to_bcd(month) << 8) |
				  ((uint32_t)_rtc_dec_to_bcd(day)) |
				  ((uint32_t)rtc_dr_wdu << 13));
	}

	rtc_unlock();
	rtc_set_init_flag();
	rtc_wait_for_init_ready();

	/* Set the RTC_DR register */
	RTC_DR = (uint32_t)(tmpreg & RTC_DR_RESERVED_MASK);

	rtc_clear_init_flag();

	rtc_lock();

	rtc_wait_for_synchro();
}
/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD time hour value

@details Requires unlocking the RTC write-protection (RTC_WPR)

Pass true to use_am_notation to use 24-hour input time; pass false to
use_am_notation to use 12-hour (AM/PM) input time
*/
void rtc_time_set_hour(uint8_t hour, bool use_am_notation)
{
	if (use_am_notation)
	{
		RTC_TR &= ~(RTC_TR_PM);
	}
	else
	{
		RTC_TR |= RTC_TR_PM;
	}

	uint8_t bcd_hour = _rtc_dec_to_bcd(hour);
	RTC_TR &= ~(RTC_TR_HT_MASK << RTC_TR_HT_SHIFT | RTC_TR_HU_MASK << RTC_TR_HU_SHIFT);
	RTC_TR |= (((bcd_hour >> 4) & RTC_TR_HT_MASK) << RTC_TR_HT_SHIFT) |
			  ((bcd_hour & RTC_TR_HU_MASK) << RTC_TR_HU_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD time minute value

@details Requires unlocking the RTC write-protection (RTC_WPR)
*/
void rtc_time_set_minute(uint8_t minute)
{
	uint8_t bcd_minute = _rtc_dec_to_bcd(minute);
	RTC_TR &= ~(RTC_TR_MNT_MASK << RTC_TR_MNT_SHIFT | RTC_TR_MNU_MASK << RTC_TR_MNU_SHIFT);
	RTC_TR |= (((bcd_minute >> 4) & RTC_TR_MNT_MASK) << RTC_TR_MNT_SHIFT) |
			  ((bcd_minute & RTC_TR_MNU_MASK) << RTC_TR_MNU_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD time second value

@details Requires unlocking the RTC write-protection (RTC_WPR)
*/
void rtc_time_set_second(uint8_t second)
{
	uint8_t bcd_second = _rtc_dec_to_bcd(second);
	RTC_TR &= ~(RTC_TR_ST_MASK << RTC_TR_ST_SHIFT | RTC_TR_SU_MASK << RTC_TR_SU_SHIFT);
	RTC_TR |= (((bcd_second >> 4) & RTC_TR_ST_MASK) << RTC_TR_ST_SHIFT) |
			  ((bcd_second & RTC_TR_SU_MASK) << RTC_TR_SU_SHIFT);
}

/*---------------------------------------------------------------------------*/
/** @brief Sets the RTC BCD time

@details Requires unlocking the RTC write-protection (RTC_WPR)
*/
void rtc_time_set_time(uint8_t hour, uint8_t minute, uint8_t second, bool use_am_notation)
{
	rtc_time_set_hour(hour, use_am_notation);
	rtc_time_set_minute(minute);
	rtc_time_set_second(second);
}

void rtc_set_time(uint32_t format, uint8_t hour, uint8_t minute, uint8_t second, bool use_am_notation)
{
	uint32_t tmpreg = 0;
	uint8_t h12 = use_am_notation ? RTC_H12_AM : RTC_H12_PM;

	if (format != RTC_Format_BIN)
	{
		tmpreg = (((uint32_t)(hour) << 16) |
				  ((uint32_t)(minute) << 8) |
				  ((uint32_t)second) |
				  ((uint32_t)(h12) << 16));
	}
	else
	{
		tmpreg = (uint32_t)(((uint32_t)_rtc_dec_to_bcd(hour) << 16) |
							((uint32_t)_rtc_dec_to_bcd(minute) << 8) |
							((uint32_t)_rtc_dec_to_bcd(second)) |
							(((uint32_t)h12) << 16));
	}
	//	  /* Disable the write protection for RTC registers */
	//	  RTC_WPR = 0xCA;
	//	  RTC_WPR = 0x53;
	//
	//	  /* Set Initialization mode */
	//	  if (rtc_enter_init_mode() == ERROR)
	//	  {
	//	    status = ERROR;
	//	  }
	//	  else
	//	  {
	rtc_unlock();
	rtc_set_init_flag();
	rtc_wait_for_init_ready();

	/* Set the RTC_TR register */
	RTC_TR = (uint32_t)(tmpreg & RTC_TR_RESERVED_MASK);

	/* Exit Initialization mode */
	rtc_clear_init_flag();
	rtc_lock();

	rtc_wait_for_synchro();

	//	  }
	//	  /* Enable the write protection for RTC registers */
	//	  RTC_WPR = 0xFF;
}

void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *day)
{
	//  uint32_t rtc_date = RTC_DR;

	*year = (RTC_DR >> RTC_DR_YU_SHIFT) & RTC_DR_YU_MASK;
	*year += ((RTC_DR >> RTC_DR_YT_SHIFT) & RTC_DR_YT_MASK) * 10;

	*month = (RTC_DR >> RTC_DR_MU_SHIFT) & RTC_DR_MU_MASK;
	*month += ((RTC_DR >> RTC_DR_MT_SHIFT) & RTC_DR_MT_MASK) * 10;

	*day = (RTC_DR >> RTC_DR_DU_SHIFT) & RTC_DR_DU_MASK;
	*day += ((RTC_DR >> RTC_DR_DT_SHIFT) & RTC_DR_DT_MASK) * 10;
}

void rtc_get_time(uint8_t *hour, uint8_t *minute, uint8_t *second)
{
	//  uint32_t rtc_time = RTC_TR;

	*hour = (RTC_TR >> RTC_TR_HU_SHIFT) & RTC_TR_HU_MASK;
	*hour += ((RTC_TR >> RTC_TR_HT_SHIFT) & RTC_TR_HT_MASK) * 10;

	*minute = (RTC_TR >> RTC_TR_MNU_SHIFT) & RTC_TR_MNU_MASK;
	*minute += ((RTC_TR >> RTC_TR_MNT_SHIFT) & RTC_TR_MNT_MASK) * 10;

	*second = (RTC_TR >> RTC_TR_SU_SHIFT) & RTC_TR_SU_MASK;
	*second += ((RTC_TR >> RTC_TR_ST_SHIFT) & RTC_TR_ST_MASK) * 10;
}

void rtc_init(uint8_t am_format, uint32_t sync, uint32_t async)
{
	rtc_unlock();
	rtc_set_init_flag();
	rtc_wait_for_init_ready();
	if (am_format)
		rtc_set_am_format();
	else
		rtc_set_pm_format();

	rtc_set_prescaler(sync, async);
	rtc_clear_init_flag();

	rtc_lock();
}
#define RTC_FLAGS_MASK ((uint32_t)(RTC_ISR_TSOVF | RTC_ISR_TSF | RTC_ISR_WUTF |    \
								   RTC_ISR_ALRBF | RTC_ISR_ALRAF | RTC_ISR_INITF | \
								   RTC_ISR_RSF | RTC_ISR_INITS | RTC_ISR_WUTWF |   \
								   RTC_ISR_ALRBWF | RTC_ISR_ALRAWF | RTC_ISR_TAMP1F))

/**
 * @brief  Checks whether the specified RTC flag is set or not.
 * @param  RTC_FLAG: specifies the flag to check.
 *          This parameter can be one of the following values:
 *            @arg RTC_ISR_TAMP1F: Tamper 1 event flag
 *            @arg RTC_ISR_TSOVF: Time Stamp OverFlow flag
 *            @arg RTC_ISR_TSF: Time Stamp event flag
 *            @arg RTC_ISR_WUTF: WakeUp Timer flag
 *            @arg RTC_ISR_ALRBF: Alarm B flag
 *            @arg RTC_ISR_ALRAF: Alarm A flag
 *            @arg RTC_ISR_INITF: Initialization mode flag
 *            @arg RTC_ISR_RSF: Registers Synchronized flag
 *            @arg RTC_ISR_INITS: Registers Configured flag
 *            @arg RTC_ISR_WUTWF: WakeUp Timer Write flag
 *            @arg RTC_ISR_ALRBWF: Alarm B Write flag
 *            @arg RTC_ISR_ALRAWF: Alarm A write flag
 * @retval The new state of RTC_FLAG (1 or 0).
 */
uint8_t rtc_get_flag_status(uint32_t RTC_FLAG)
{
	uint8_t bitstatus = 0;
	uint32_t tmpreg = 0;

	/* Get all the flags */
	tmpreg = (uint32_t)(RTC_ISR & RTC_FLAGS_MASK);

	/* Return the status of the flag */
	if ((tmpreg & RTC_FLAG) != (uint32_t)0)
	{
		bitstatus = 1;
	}
	else
	{
		bitstatus = 0;
	}
	return bitstatus;
}

/**
 * @brief  Clears the RTC's pending flags.
 * @param  RTC_FLAG: specifies the RTC flag to clear.
 *          This parameter can be any combination of the following values:
 *            @arg RTC_ISR_TAMP1F: Tamper 1 event flag
 *            @arg RTC_ISR_TSOVF: Time Stamp Overflow flag
 *            @arg RTC_ISR_TSF: Time Stamp event flag
 *            @arg RTC_ISR_WUTF: WakeUp Timer flag
 *            @arg RTC_ISR_ALRBF: Alarm B flag
 *            @arg RTC_ISR_ALRAF: Alarm A flag
 *            @arg RTC_ISR_RSF: Registers Synchronized flag
 * @retval None
 */
void rtc_clear_flag(uint32_t RTC_FLAG)
{
	/* Clear the Flags in the RTC_ISR register */
	RTC_ISR = (uint32_t)((uint32_t)(~((RTC_FLAG | RTC_ISR_INIT) & 0x0000FFFF) | (uint32_t)(RTC_ISR & RTC_ISR_INIT)));
}

/**
 * @brief  Enables or disables the specified RTC interrupts.
 * @param  RTC_IT: specifies the RTC interrupt sources to be enabled or disabled.
 *          This parameter can be any combination of the following values:
 *            @arg RTC_IT_TS:  Time Stamp interrupt mask
 *            @arg RTC_IT_WUT:  WakeUp Timer interrupt mask
 *            @arg RTC_IT_ALRB:  Alarm B interrupt mask
 *            @arg RTC_IT_ALRA:  Alarm A interrupt mask
 *            @arg RTC_IT_TAMP: Tamper event interrupt mask
 * @param  NewState: new state of the specified RTC interrupts.
 *          This parameter can be: 1 (ENABLE) or 0 (DISABLE).
 * @retval None
 */
void rtc_interrupt_enable(uint32_t RTC_IT, uint8_t NewState)
{

	/* Disable the write protection for RTC registers */
	rtc_unlock();

	if (NewState != 0)
	{
		/* Configure the Interrupts in the RTC_CR register */
		RTC_CR |= (uint32_t)(RTC_IT & ~RTC_TAFCR_TAMPIE);
		/* Configure the Tamper Interrupt in the RTC_TAFCR */
		RTC_TAFCR |= (uint32_t)(RTC_IT & RTC_TAFCR_TAMPIE);
	}
	else
	{
		/* Configure the Interrupts in the RTC_CR register */
		RTC_CR &= (uint32_t) ~(RTC_IT & (uint32_t)~RTC_TAFCR_TAMPIE);
		/* Configure the Tamper Interrupt in the RTC_TAFCR */
		RTC_TAFCR &= (uint32_t) ~(RTC_IT & RTC_TAFCR_TAMPIE);
	}
	/* Enable the write protection for RTC registers */
	rtc_lock();
}

/**
 * @brief  Checks whether the specified RTC interrupt has occurred or not.
 * @param  RTC_IT: specifies the RTC interrupt source to check.
 *          This parameter can be one of the following values:
 *            @arg RTC_IT_TS: Time Stamp interrupt
 *            @arg RTC_IT_WUT: WakeUp Timer interrupt
 *            @arg RTC_IT_ALRB: Alarm B interrupt
 *            @arg RTC_IT_ALRA: Alarm A interrupt
 *            @arg RTC_IT_TAMP1: Tamper 1 event interrupt
 * @retval The new state of RTC_IT (1 or 0).
 */
uint8_t rtc_get_interrupt_status(uint32_t RTC_IT)
{
	uint8_t bitstatus = 0;
	uint32_t tmpreg = 0, enablestatus = 0;

	/* Get the TAMPER Interrupt enable bit and pending bit */
	tmpreg = (uint32_t)(RTC_TAFCR & (RTC_TAFCR_TAMPIE));

	/* Get the Interrupt enable Status */
	enablestatus = (uint32_t)((RTC_CR & RTC_IT) | (tmpreg & (RTC_IT >> 15)));

	/* Get the Interrupt pending bit */
	tmpreg = (uint32_t)((RTC_ISR & (uint32_t)(RTC_IT >> 4)));

	/* Get the status of the Interrupt */
	if ((enablestatus != (uint32_t)0) && ((tmpreg & 0x0000FFFF) != (uint32_t)0))
	{
		bitstatus = 1;
	}
	else
	{
		bitstatus = 0;
	}
	return bitstatus;
}

/**
 * @brief  Clears the RTC's interrupt pending bits.
 * @param  RTC_IT: specifies the RTC interrupt pending bit to clear.
 *          This parameter can be any combination of the following values:
 *            @arg RTC_IT_TS: Time Stamp interrupt
 *            @arg RTC_IT_WUT: WakeUp Timer interrupt
 *            @arg RTC_IT_ALRB: Alarm B interrupt
 *            @arg RTC_IT_ALRA: Alarm A interrupt
 *            @arg RTC_IT_TAMP1: Tamper 1 event interrupt
 * @retval None
 */
void rtc_clear_interrupt_pending_bit(uint32_t RTC_IT)
{
	uint32_t tmpreg = 0;

	/* Get the RTC_ISR Interrupt pending bits mask */
	tmpreg = (uint32_t)(RTC_IT >> 4);

	/* Clear the interrupt pending bits in the RTC_ISR register */
	RTC_ISR = (uint32_t)((uint32_t)(~((tmpreg | RTC_ISR_INIT) & 0x0000FFFF) | (uint32_t)(RTC_ISR & RTC_ISR_INIT)));
}

/**@}*/
