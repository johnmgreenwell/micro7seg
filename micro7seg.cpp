//--------------------------------------------------------------------------------------------------------------------
// Name        : micro7seg.cpp
// Purpose     : QDSP-6064 Bubble 7-Segment Display Driver
// Description : This source file implements header file micro7seg.h.
// Language    : C++
// Platform    : Portable
// Framework   : Portable
// Copyright   : MIT License 2024, John Greenwell
//--------------------------------------------------------------------------------------------------------------------

#include "micro7seg.h"

namespace PeripheralIO {

const uint8_t CONVERSION_TABLE[16] = 
{
    0x3F,  // 0
    0x06,  // 1
    0x5B,  // 2
    0x4F,  // 3
    0x66,  // 4
    0x6D,  // 5
    0x7D,  // 6
    0x07,  // 7
    0x7F,  // 8
    0x67,  // 9
    0x77,  // A
    0x7C,  // B
    0x39,  // C
    0x5E,  // D
    0x79,  // E
    0x71,  // F
};

Micro7Seg::Micro7Seg(const uint8_t* character_pins, const uint8_t* select_pins)
: _character(character_pins, 8)
, _select(select_pins, 4)
, _digit()
, _refreshIterator(0)
{ }

void Micro7Seg::refresh() 
{
    _select.write(8u >> _refreshIterator);
    _character.write(_digit[_refreshIterator].code);
    _refreshIterator = (_refreshIterator + 1) % 4;
}

void Micro7Seg::init()
{
    _character.init();
    _select.init();
    _character.portMode(GPIO_OUTPUT);
    _select.portMode(GPIO_OUTPUT);
}

void Micro7Seg::clear() 
{
    for (uint8_t i = 0; i < 4; i++) 
    {
        _digit[i].code = 0;
        _digit[i].value = 0;
        _digit[i].toChar = '\0';
        _digit[i].hasDec = false;
    }
}

void Micro7Seg::write(uint16_t data) 
{
    for (uint8_t i = 0; i < 4; i++) 
    {
        uint8_t tmp = 0x0F & (uint8_t)(data >> (i * 4));
        _digit[i].value = tmp;
        _digit[i].code = convertTo7SegDigit(tmp);
        _digit[i].toChar = (tmp > 9) ? (tmp + 0x37) : (tmp + 0x30);
        _digit[i].hasDec = false;
    }
}

bool Micro7Seg::print(const char *str)
{
    // TODO: write function
    return false;
}

bool Micro7Seg::print(const String& str) 
{
    bool stringValid = true;

    uint8_t digIt = 0;

    clear();

    for (uint8_t i = str.length(); i > 0; i--) 
    {
        uint8_t data = str[i-1];
        data = toUpperCase(data);
        if (!checkCharValidity(data)) 
        {
            stringValid = false;
            break;
        }
        if (data == '.')
        {
            _digit[digIt].code |= 0x80;
        }
        else 
        {
            _digit[digIt].code |= convertCharToCode(data);
            _digit[digIt].value = convertCharToVal(data);
            _digit[digIt].toChar = data;
            _digit[digIt].hasDec = (_digit[digIt].code & 0x80);
            if (digIt < 3) digIt++;
            else break;
        }
    }
    
    return stringValid;
}

/* Private support function */
uint8_t Micro7Seg::convertCharToVal(uint8_t data) const 
{
    if (data >= '0' && data <= '9') data -= 0x30;
    else if (data >= 'A' && data <= 'F' ) data -= 0x37;
    else data = 0;
    return data;
}

/* Private support function */
uint8_t Micro7Seg::convertCharToCode(uint8_t data) const 
{
    if (data == '-') data = 0x40;
    else if (data == ' ') data = 0;
    else data = convertTo7SegDigit(convertCharToVal(data));
    return data;
}

/* Private support function */
uint8_t Micro7Seg::convertTo7SegDigit(uint8_t nibble) const 
{
    nibble &= 0x0F;
    return CONVERSION_TABLE[nibble];
}

/* Private support function */
bool Micro7Seg::checkCharValidity(char data) const 
{
    bool validityCheck = true;
    if (data < '0' || data > '9')
    {
        if (data < 'A' || data > 'F')
        {
            if (data != '-' && data != '.' && data != ' ')
                validityCheck = false;
        }
    }
    return validityCheck;
}

}

// EOF
