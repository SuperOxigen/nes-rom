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

    SerialIO & _serial;
    unsigned short _addr;
    unsigned char _data;
    unsigned char _ctrl;
    OperationState _state;

    void to_byte_squence(unsigned char * byte_squence);

public:
    EpromWriter(SerialIO & serialIO);

    void set_mode(EpromWriter::OperationState state);
    OperationState get_mode() const;
    void set_address(unsigned short address);
    void set_data(unsigned char data);
    void commit();
};

#endif
