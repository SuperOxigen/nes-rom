#include "eprom_writer.hpp"
#include <string.h>

/*
 *  EPROM Writer memory layout.  4x8-bit registers
 *
 *  +-----+-----+-----+-----+-----+-----+-----+-----+
 *  |                       | Data      | Control   |
 *  |   Address Register    | Register  | Register  |
 *  |       2 bytes         |  1 byte   |  1 byte   |
 *  +-----+-----+-----+-----+-----+-----+-----+-----+
 * MSB     3           2           1           0   LSB
 *
 *
 */

#define CTRL_OFFSET 0
#define DATA_OFFSET 1
#define ADDR_OFFSET 2

#define CTRL_SIZE 1
#define DATA_SIZE 1
#define ADDR_SIZE 2


enum class ROM_POWER : unsigned char
{
    NORMAL  = 0x00,
    PROGRAM = 0x01,
    MASK    = 0x01,
    NMASK   = 0xFE
};

inline static unsigned char r2c(ROM_POWER power)
{
    return static_cast<unsigned char>(power);
}

enum class ROM_ENABLE : unsigned char
{
    STANDBY = 0x00,
    READ    = 0x02,
    WRITE   = 0x06,
    MASK    = 0x06,
    NMASK   = 0xFB
};

inline static unsigned char r2c(ROM_ENABLE enable)
{
    return static_cast<unsigned char>(enable);
}

EpromWriter::EpromWriter(SerialIO & serialIO):
    _serial(serialIO), _addr(0), _data(0), _ctrl(0), _state(LOW_POWER_MODE)
{
    this->set_mode(this->_state);
}

void EpromWriter::to_byte_squence(unsigned char *byte_squence)
{
    memcpy(byte_squence + CTRL_OFFSET, &this->_ctrl, CTRL_SIZE);
    memcpy(byte_squence + DATA_OFFSET, &this->_data, DATA_SIZE);
    memcpy(byte_squence + ADDR_OFFSET, &this->_addr, ADDR_SIZE);
}

void EpromWriter::set_mode(OperationState state)
{
    switch (state) {
        case LOW_POWER_MODE:
            this->_ctrl &= ~r2c(ROM_POWER::MASK);
            this->_ctrl |= r2c(ROM_POWER::NORMAL);
            this->_ctrl &= ~r2c(ROM_ENABLE::MASK);
            this->_ctrl |= r2c(ROM_ENABLE::STANDBY);
            break;
        case PROGRAM_MODE:
            this->_ctrl &= ~r2c(ROM_POWER::MASK);
            this->_ctrl |= r2c(ROM_POWER::PROGRAM);
            this->_ctrl &= ~r2c(ROM_ENABLE::MASK);
            this->_ctrl |= r2c(ROM_ENABLE::WRITE);
            break;
        case VERIFY_L_MODE:
            this->_ctrl &= ~r2c(ROM_POWER::MASK);
            this->_ctrl |= r2c(ROM_POWER::PROGRAM);
            this->_ctrl &= ~r2c(ROM_ENABLE::MASK);
            this->_ctrl |= r2c(ROM_ENABLE::READ);
            break;
        case VERIFT_H_MODE:
            this->_ctrl &= ~r2c(ROM_POWER::MASK);
            this->_ctrl |= r2c(ROM_POWER::NORMAL);
            this->_ctrl &= ~r2c(ROM_ENABLE::MASK);
            this->_ctrl |= r2c(ROM_ENABLE::READ);
            break;
    }
}

EpromWriter::OperationState EpromWriter::get_mode() const
{
    return this->_state;
}

void EpromWriter::set_data(unsigned char data)
{
    this->_data = data;
}

void EpromWriter::set_address(unsigned short address)
{
    this->_addr = address;
}

void EpromWriter::commit()
{
    sequence_t sequence;
    this->to_byte_squence(sequence);
    this->_serial.write_load(sequence, sizeof(sequence));
}