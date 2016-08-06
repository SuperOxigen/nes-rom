#ifndef _EPROM_WRITER_HPP_
#define _EPROM_WRITER_HPP_

#include "serial_io.hpp"

struct EpromWriter
{
    enum OperationState
    {
        LOW_POWER_MODE,
        PROGRAM_MODE,
        VERIFY_L_MODE,
        VERIFT_H_MODE
    };
private:
    typedef unsigned char sequence_t[4];

    SerialIO __serial;
    SerialIO * _serial;
    pin_t _err_one, /* Error Indicator 1 */
          _err_two; /* Error Indicator 2 */
    unsigned short _addr;
    unsigned char _data;
    unsigned char _ctrl;
    OperationState _state;

    void to_byte_squence(unsigned char * byte_squence);

public:
    EpromWriter(SerialIO * serialIO, pin_t err_one, pin_t err_two);
    EpromWriter(pin_t ser, pin_t ser_clk, pin_t ser_clr, pin_t par_clk, pin_t err_one, pin_t err_two);

    void set_mode(EpromWriter::OperationState state);
    OperationState get_mode() const;
    void set_address(unsigned short address);
    void set_data(unsigned char data);
    void commit();

    bool check_error() const;
};

#endif
