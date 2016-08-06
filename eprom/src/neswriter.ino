#include <Arduino.h>

#include "eprom_writer.hpp"
#include "serial_io.hpp"

#ifdef _NES_PROGRAM_BUILD_

/* Include program ROM header */

#endif

#define SERIAL_PIN 0
#define SERIAL_CLK_PIN 0
#define SERIAL_CLR_PIN 0
#define PARALLEL_CLK_PIN 0

#define ERROR_ONE_PIN 0
#define ERROR_TWO_PIN 0

static EpromWriter writer(SERIAL_PIN,
                          SERIAL_CLK_PIN,
                          SERIAL_CLR_PIN,
                          PARALLEL_CLK_PIN,
                          ERROR_ONE_PIN,
                          ERROR_TWO_PIN);

void setup()
{
    writer.set_mode(EpromWriter::LOW_POWER_MODE);
}

void loop()
{

}
