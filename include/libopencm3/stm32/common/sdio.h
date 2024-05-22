/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2012 Felix Held <felix-libopencm3@felixheld.de>
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

#ifndef LIBOPENCM3_SDIO_H
#define LIBOPENCM3_SDIO_H

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/memorymap.h>

/* --- SDIO registers ------------------------------------------------------ */

/* SDIO power control register (SDIO_POWER) */
#define SDIO_POWER MMIO32(SDIO_BASE + 0x00)

/* SDI clock control register (SDIO_CLKCR) */
#define SDIO_CLKCR MMIO32(SDIO_BASE + 0x04)

/* SDIO argument register (SDIO_ARG) */
#define SDIO_ARG MMIO32(SDIO_BASE + 0x08)

/* SDIO command register (SDIO_CMD) */
#define SDIO_CMD MMIO32(SDIO_BASE + 0x0C)

/* SDIO command response register (SDIO_RESPCMD) */
#define SDIO_RESPCMD MMIO32(SDIO_BASE + 0x10)

/* SDIO response 1..4 register (SDIO_RESPx) */
#define SDIO_RESP1 MMIO32(SDIO_BASE + 0x14)
#define SDIO_RESP2 MMIO32(SDIO_BASE + 0x18)
#define SDIO_RESP3 MMIO32(SDIO_BASE + 0x1C)
#define SDIO_RESP4 MMIO32(SDIO_BASE + 0x20)

/* SDIO data timer register (SDIO_DTIMER) */
#define SDIO_DTIMER MMIO32(SDIO_BASE + 0x24)

/* SDIO data length register (SDIO_DLEN) */
#define SDIO_DLEN MMIO32(SDIO_BASE + 0x28)

/* SDIO data control register (SDIO_DCTRL) */
#define SDIO_DCTRL MMIO32(SDIO_BASE + 0x2C)

/* SDIO data counter register (SDIO_DCOUNT) */
/* read only, write has no effect */
#define SDIO_DCOUNT MMIO32(SDIO_BASE + 0x30)

/* SDIO status register (SDIO_STA) */
#define SDIO_STA MMIO32(SDIO_BASE + 0x34)

/* SDIO interrupt clear register (SDIO_ICR) */
#define SDIO_ICR MMIO32(SDIO_BASE + 0x38)

/* SDIO mask register (SDIO_MASK) */
#define SDIO_MASK MMIO32(SDIO_BASE + 0x3C)

/* SDIO FIFO counter register (SDIO_FIFOCNT) */
#define SDIO_FIFOCNT MMIO32(SDIO_BASE + 0x48)

/* SDIO data FIFO register (SDIO_FIFO) */
/* the SDIO data FIFO is 32 32bit words long, beginning at this address */
#define SDIO_FIFO MMIO32(SDIO_BASE + 0x80)

#endif
