/* Name: main.c
 * Project: AVR bootloader HID
 * Author: Christian Starkjohann
 * Creation Date: 2007-03-19
 * Tabsize: 4
 * Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt)
 * This Revision: $Id$
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <string.h>
#include <util/delay.h>
#include <stdbool.h>

#define BOOTLOADER_CAN_EXIT     1

static void leaveBootloader() __attribute__((__noreturn__));

uint32_t MagicBootKey __attribute__((section(".bootkey")));


static inline bool bootLoaderCondition() 
{
    
    if(MagicBootKey == 0xDEADBEEF)
    {
        return true;
    }

    if(pgm_read_word_near(0) == 0xFFFF)
    {
        return true;
    }

    PORTD = 1 << 4; /* activate pull-up for key */
    _delay_us(10);  /* wait for levels to stabilize */
   return ((PIND & (1<<4)) == 0);
}

#include "usbdrv.h"
#include "oddebug.h"
#include "usbhiddescriptor.h"
//#include "bootloaderconfig.h"


/* ------------------------------------------------------------------------ */

#ifndef ulong
#   define ulong    unsigned long
#endif
#ifndef uint
#   define uint     unsigned int
#endif

#if (FLASHEND) > 0xffff /* we need long addressing */
#   define addr_t           ulong
#else
#   define addr_t           uint
#endif

static addr_t           currentAddress; /* in bytes */
static uchar            offset;         /* data already processed in current transfer */
#if BOOTLOADER_CAN_EXIT
static uchar            exitMainloop;
#endif



/* compatibility with ATMega88 and other new devices: */
#ifndef TCCR0
#define TCCR0   TCCR0B
#endif
#ifndef GICR
#define GICR    MCUCR
#endif

static void (*nullVector)(void) __attribute__((__noreturn__));

static void leaveBootloader()
{
    DBG1(0x01, 0, 0);
    MagicBootKey = 0;
    cli();
    boot_rww_enable();
    USB_INTR_ENABLE = 0;
    USB_INTR_CFG = 0;       /* also reset config bits */
#if F_CPU == 12800000
    TCCR0 = 0;              /* default value */
#endif
    GICR = (1 << IVCE);     /* enable change of interrupt vectors */
    GICR = (0 << IVSEL);    /* move interrupts to application flash section */
/* We must go through a global function pointer variable instead of writing
 *  ((void (*)(void))0)();
 * because the compiler optimizes a constant 0 to "rcall 0" which is not
 * handled correctly by the assembler.
 */
    nullVector();
}

uchar   usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;
    static uchar    replyBuffer[7] = 
    {
        1,                              /* report ID */
        SPM_PAGESIZE & 0xff,
        SPM_PAGESIZE >> 8,
        ((long)FLASHEND + 1) & 0xff,
        (((long)FLASHEND + 1) >> 8) & 0xff,
        (((long)FLASHEND + 1) >> 16) & 0xff,
        (((long)FLASHEND + 1) >> 24) & 0xff
    };

    if(rq->bRequest == USBRQ_HID_SET_REPORT)
    {
        if(rq->wValue.bytes[0] == 2)
        {
            offset = 0;
            return USB_NO_MSG;
        }
#if BOOTLOADER_CAN_EXIT
        else
        {
            exitMainloop = 1;
        }
#endif
    }
    else if(rq->bRequest == USBRQ_HID_GET_REPORT)
    {
        usbMsgPtr = (usbMsgPtr_t)replyBuffer;
        return 7;
    }
    return 0;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    union {
        addr_t  l;
        uint    s[sizeof(addr_t)/2];
        uchar   c[sizeof(addr_t)];
    }       address;
    uchar   isLast;

    address.l = currentAddress;
    if(offset == 0)
    {
        DBG1(0x30, data, 3);
        address.c[0] = data[1];
        address.c[1] = data[2];
#if (FLASHEND) > 0xffff /* we need long addressing */
        address.c[2] = data[3];
        address.c[3] = 0;
#endif
        data += 4;
        len -= 4;
    }
    DBG1(0x31, (void *)&currentAddress, 4);
    offset += len;
    isLast = offset & 0x80; /* != 0 if last block received */
    do
    {
        addr_t prevAddr;
#if SPM_PAGESIZE > 256
        uint pageAddr;
#else
        uchar pageAddr;
#endif
        DBG1(0x32, 0, 0);
        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
        if(pageAddr == 0)
        {              /* if page start: erase */
            DBG1(0x33, 0, 0);
#ifndef TEST_MODE
            cli();
            boot_page_erase(address.l); /* erase page */
            sei();
            boot_spm_busy_wait();       /* wait until page is erased */
#endif
        }
        cli();
        boot_page_fill(address.l, *(short *)data);
        sei();
        prevAddr = address.l;
        address.l += 2;
        data += 2;
        /* write page when we cross page boundary */
        pageAddr = address.s[0] & (SPM_PAGESIZE - 1);
        if(pageAddr == 0)
        {
            DBG1(0x34, 0, 0);
#ifndef TEST_MODE
            cli();
            boot_page_write(prevAddr);
            sei();
            boot_spm_busy_wait();
#endif
        }
        len -= 2;
    } while(len);
    currentAddress = address.l;
    DBG1(0x35, (void *)&currentAddress, 4);
    return isLast;
}

static void initForUsbConnectivity(void)
{
    uchar i = 0;

#if F_CPU == 12800000
    TCCR0 = 3;          /* 1/64 prescaler */
#endif
    usbInit();
    /* enforce USB re-enumerate: */
    usbDeviceDisconnect();  /* do this while interrupts are disabled */
    do
    {             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    } while(--i);
    usbDeviceConnect();
    sei();
}

int __attribute__((noreturn)) main(void)
{
    odDebugInit();
    DBG1(0x00, 0, 0);
    /* jump to application if jumper is set */
    if(bootLoaderCondition())
    {
        uchar i = 0, j = 0;
#ifndef TEST_MODE
        GICR = (1 << IVCE);  /* enable change of interrupt vectors */
        GICR = (1 << IVSEL); /* move interrupts to boot flash section */
#endif
        initForUsbConnectivity();
        while(true)
        { /* main event loop */
            wdt_reset();
            usbPoll();
#if BOOTLOADER_CAN_EXIT
            if(exitMainloop)
            {
#if F_CPU == 12800000
                break;  /* memory is tight at 12.8 MHz, save exit delay below */
#endif
                if(--i == 0)
                {
                    if(--j == 0)
                        break;
                }
            }
#endif
        }
    }
    leaveBootloader();
}

