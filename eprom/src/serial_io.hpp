#ifndef _SERIAL_IO_H_
#define _SERIAL_IO_H_

typedef unsigned int pin_t;

class SerialIO
{
    enum Pin
    {
        SERIAL_IN,
        SERIAL_CLK,
        SERIAL_CLR,
        PARALLEL_CLK,
        OUTPUT_ENABLED
    };

    enum Mode : bool
    {
        ACTIVE_HIGH = false,
        ACTIVE_LOW = true
    };

    pin_t   _ser,           /* Serial Out */
            _ser_clk,       /* Serial Clock */
            _ser_clr,       /* Serial Clear */
            _par_clk,       /* Parallel Clock */
            _out_en;        /* Output Enabled */
    bool    _ser_actl,      /* Serial Out - Active Low */
            _ser_clk_actl,  /* Serial Clock - Active Low */
            _ser_clr_actl,  /* Serial Clear - Active Low */
            _par_clk_actl,  /* Parallel Clock - Active Low */
            _out_en_actl,   /* Output Enabled - Active Low */
            _out_en_sprtd;  /* Output Enabled - Supported */

    void bit_load(unsigned int bit);
    void write_byte(unsigned char byte);
    void write_pin(Pin pin, int state);
    void init_pins();
    void clk();
public:
    SerialIO(pin_t ser, pin_t ser_clk, pin_t ser_clr, pin_t par_clk);
    SerialIO(pin_t ser, pin_t ser_clk, pin_t ser_clr, pin_t par_clk, pin_t out_en);

    void write(unsigned char const * data, unsigned int nbytes);
    void load();
    void write_load(unsigned char const * data, unsigned int nbytes);
    void clear();

    void set_active_level(Pin pin, Mode mode);
};

#endif
