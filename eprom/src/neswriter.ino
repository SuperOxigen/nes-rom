#include <Arduino.h>

#include "eprom_writer.hpp"
#include "serial_io.hpp"
#include "ctrl_panel.hpp"

#ifdef NES_PROGRAM_BUILD

/* Include program ROM header */
#include "rom/rom.hpp"
#define EPROM_SIZE (64 * 1024)

#endif

// #define SERIAL_PIN 13
// #define SERIAL_CLK_PIN 12
// #define SERIAL_CLR_PIN 11
// #define PARALLEL_CLK_PIN 10
//
// #define ERROR_ONE_PIN 9
// #define ERROR_TWO_PIN 8

#define LED_A_PIN   2
#define LED_B_PIN   7
#define IN_A_PIN    5
#define IN_B_PIN    3
#define RES_A_PIN   6
#define RES_B_PIN   4

static EpromWriter * ewriter;

static ControlPanel * cpanel;

void writter_test(void)
{
    #ifdef NES_PROGRAM_BUILD
        size_t          rom_kb = sizeof(rom_var) / 1024;
        unsigned long   addr;

        ewriter.set_mode(EpromWriter::PROGRAM_MODE);
        for (addr = 0; addr < EPROM_SIZE; addr++)
        {
            ewriter.set_address((unsigned short)addr);
            ewriter.set_data(rom_var[addr]);
            ewriter.commit();
            delayMicroseconds(100 - (4 * 8 * 3));
        }

        ewriter.set_mode(EpromWriter::VERIFY_H_MODE);
        ewriter.commit();

        delay(20000);

        for (addr = 0; addr < EPROM_SIZE; addr++)
        {
            ewriter.set_address((unsigned short)addr);
            ewriter.set_data(rom_var[addr]);
            ewriter.commit();
            delayMicroseconds(100);

            if (ewriter.check_error())
            {
                /* Indicate error */
                break;
            }
        }

        if (addr == EPROM_SIZE)
        {
            ewriter.set_mode(EpromWriter::VERIFY_L_MODE);
            ewriter.commit();
            delay(500);
            for (addr = 0; addr < EPROM_SIZE; addr++)
            {
                ewriter.set_address((unsigned short)addr);
                ewriter.set_data(rom_var[addr]);
                ewriter.commit();
                delayMicroseconds(100);

                if (ewriter.check_error())
                {
                    /* Indicate error */
                    break;
                }
            }
        }

        ewriter.set_mode(EpromWriter::LOW_POWER_MODE);
        ewriter.commit();

        while (true)
        {
            delay(1000);
        }

    #else
        delay(1000);
    #endif
}

static int a_on,
           b_on;

void setup()
{
    // ewriter.set_mode(EpromWriter::LOW_POWER_MODE);
    // ewriter = new EpromWriter(SERIAL_PIN,
    //                           SERIAL_CLK_PIN,
    //                           SERIAL_CLR_PIN,
    //                           PARALLEL_CLK_PIN,
    //                           ERROR_ONE_PIN,
    //                           ERROR_TWO_PIN);
    cpanel = new ControlPanel(LED_A_PIN,
                              IN_A_PIN,
                              RES_A_PIN,
                              LED_B_PIN,
                              IN_B_PIN,
                              RES_B_PIN);
    a_on = 0;
    b_on = 0;
}

void loop()
{
    if (cpanel->read_a() == HIGH)
    {
        if (a_on)
            cpanel->led_a_off();
        else
            cpanel->led_a_on();
        a_on = !a_on;
    }

    if (cpanel->read_b() == HIGH)
    {
        if (b_on)
            cpanel->led_b_off();
        else
            cpanel->led_b_on();
        b_on = !b_on;
    }
    delay(100);
}
