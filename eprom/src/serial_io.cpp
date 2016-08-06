#include "serial_io.hpp"
#include <Arduino.h>

#define LSB 0x01
#define MSB 0x80
#define PULSE_DELAY 3

SerialIO::SerialIO(pin_t ser, pin_t ser_clk, pin_t ser_clr, pin_t par_clk):
    _ser(ser), _ser_clk(ser_clk), _ser_clr(ser_clr), _par_clk(par_clk), _out_en(0),
    _ser_actl(false), _ser_clk_actl(false), _ser_clr_actl(false), _par_clk_actl(false),
    _out_en_actl(false), _out_en_sprtd(false)
{
    this->init_pins();
}

SerialIO::SerialIO(pin_t ser, pin_t ser_clk, pin_t ser_clr, pin_t par_clk, pin_t out_en):
    _ser(ser), _ser_clk(ser_clk), _ser_clr(ser_clr), _par_clk(par_clk), _out_en(out_en),
    _ser_actl(false), _ser_clk_actl(false), _ser_clr_actl(false), _par_clk_actl(false),
    _out_en_actl(false), _out_en_sprtd(true)
{
    this->init_pins();
}

void SerialIO::init_pins()
{
    pinMode(this->_ser, OUTPUT);
    pinMode(this->_ser_clk, OUTPUT);
    pinMode(this->_ser_clr, OUTPUT);
    pinMode(this->_par_clk, OUTPUT);

    this->write_pin(SERIAL_IN, LOW);
    this->write_pin(SERIAL_CLK, LOW);
    this->write_pin(SERIAL_CLR, LOW);
    this->write_pin(PARALLEL_CLK, LOW);

    if (this->_out_en_sprtd)
    {
        pinMode(this->_out_en, OUTPUT);
        this->write_pin(OUTPUT_ENABLED, LOW);
    }

    this->clear();
}

void SerialIO::write_pin(Pin pin, int state)
{
    decltype(this->_ser)        pin_no;
    decltype(this->_ser_actl)   actl;

    if (!this->_out_en_sprtd && pin == OUTPUT_ENABLED) return;

    switch (pin)
    {
        case SERIAL_IN:
            pin_no = this->_ser;
            actl = this->_ser_actl;
            break;
        case SERIAL_CLK:
            pin_no = this->_ser_clk;
            actl = this->_ser_clk_actl;
            break;
        case SERIAL_CLR:
            pin_no = this->_ser_clr;
            actl = this->_ser_clr_actl;
            break;
        case PARALLEL_CLK:
            pin_no = this->_par_clk;
            actl = this->_par_clk_actl;
            break;
        case OUTPUT_ENABLED:
            pin_no = this->_out_en;
            actl = this->_out_en_actl;
            break;
        /* default: - do not add, will provide warnings if Pin enum is extended
            without adding the appropriate case */
    }

    if (actl)
    {
        state = (state == HIGH ? LOW : HIGH);
    }

    digitalWrite(pin_no, state);
}

void SerialIO::write_byte(unsigned char byte)
{
    int     i;

    for (i = 0; i < 8; i++)
    {
        this->bit_load(byte & LSB);
        byte = byte >> 1;
    }
}

void SerialIO::bit_load(unsigned int bit)
{
    if (bit)
    {
        this->write_pin(SERIAL_IN, HIGH);
    }
    else
    {
        this->write_pin(SERIAL_IN, LOW);
    }
    this->clk();
}

void SerialIO::clk()
{
    this->write_pin(SERIAL_CLK, HIGH);
    delayMicroseconds(PULSE_DELAY);
    this->write_pin(SERIAL_CLK, LOW);
    delayMicroseconds(PULSE_DELAY);
}

void SerialIO::write(const unsigned char * data, unsigned int nbytes)
{
    unsigned int i;

    for (i = 0; i < nbytes; i++)
    {
        this->write_byte(data[i]);
    }
}

void SerialIO::load()
{
    this->write_pin(PARALLEL_CLK, HIGH);
    delayMicroseconds(PULSE_DELAY);
    this->write_pin(PARALLEL_CLK, LOW);
    delayMicroseconds(PULSE_DELAY);
}

void SerialIO::write_load(const unsigned char *data, unsigned int nbytes)
{
    this->write(data, nbytes);
    this->load();
}

void SerialIO::clear()
{
    this->write_pin(SERIAL_CLR, HIGH);
    this->clk();
    this->write_pin(SERIAL_CLR, LOW);
    this->load();
}

void SerialIO::set_active_level(SerialIO::Pin pin, SerialIO::Mode mode)
{
    if (!this->_out_en_actl && pin == OUTPUT_ENABLED) return;

    switch (pin)
    {
        case SERIAL:
            this->_ser_actl = (mode == ACTIVE_LOW);
            break;
        case SERIAL_CLK:
            this->_ser_clk_actl = (mode == ACTIVE_LOW);
            break;
        case SERIAL_CLR:
            this->_ser_clr_actl = (mode == ACTIVE_LOW);
            break;
        case PARALLEL_CLK:
            this->_par_clk_actl = (mode == ACTIVE_LOW);
            break;
        case OUTPUT_ENABLED:
            this->_out_en_actl = (mode == ACTIVE_LOW);
            break;
    }
}
