#include <avr/wdt.h>
#include <stdbool.h>

uint32_t MagicBootKey __attribute__((section(".bootkey")));

void bootloader_enter()
{
    MagicBootKey = 0xDEADBEEF;
    wdt_enable(WDTO_250MS);
    while(true);

}