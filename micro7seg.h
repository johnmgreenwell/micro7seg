//--------------------------------------------------------------------------------------------------------------------
// Name        : micro7seg.h
// Purpose     : QDSP-6064 Bubble 7-Segment Display Driver
// Description : 
//               This driver is intended for control of QDSP-6064 Bubble 7-Segment display module.
//
//               The circuit used to implement this driver is intentionally flexible. Two Hardware Abstraction Layer
//               (HAL) GPIOPort classes are used for the selection lines and the segment driver lines. Within the
//               HAL GPIOPort implementation, these may be actual GPIO, or shift registers, or IO-expanders etc.
//
//               Character pin IDs [0-7]: { A, B, C, D, E, F, G, DP }
//               Select pin IDs [0-3]:    { Left, ..., Right }
//
// Language    : C++
// Platform    : Portable
// Framework   : Portable
// Copyright   : MIT License 2024, John Greenwell
// Requires    : External : N/A
//               Custom   : hal.h - Custom implementation-defined Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------------------------
#ifndef MICRO7SEG_H
#define MICRO7SEG_H

#include "hal.h"

namespace PeripheralIO
{

class Micro7Seg
{
public:

    /**
     * @brief Constructor for Micro7Seg object
     * @param character_pins Pointer to array of 8 pin numbers interpretable by the HAL as segment pins
     * @param select_pins Pointer to array of 4 pin numbers interpretable by the HAL as selection pins
    */
    Micro7Seg(const uint8_t * character_pins=NULL, const uint8_t * select_pins=NULL);

    /**
     * @brief Initialize Micro7Seg object
    */
    void init();

    /**
     * @brief Periodic function; call at a fixed interval (e.g. every 2.5 ms)
    */
    void refresh();

    /**
     * @brief Clear the display, applied after next call to refresh()
    */
    void clear();

    /**
     * @brief Write raw 16-bit unsigned int to display in hexadecimal; 1 nibble per digit (big endian)
    */
    void write(uint16_t val);

    /**
     * @brief Parse string to hexadecimal display, including decimal point if present
     * @return False for invalid input, true otherwise
    */
    bool print(const char *str);

    /**
     * @brief Parse string to hexadecimal display, including decimal point if present
     * @return False for invalid input, true otherwise
    */
    bool print(const String& str);
 
private:

    struct Digit
    {
        uint8_t code;
        uint8_t value;
        char toChar;
        bool hasDec;
    };
    
    uint8_t convertCharToVal(uint8_t data) const;
    uint8_t convertCharToCode(uint8_t data) const;
    uint8_t convertTo7SegDigit(uint8_t) const;
    bool checkCharValidity(char data) const;

    HAL::GPIOPort _character;
    HAL::GPIOPort _select;
    Digit _digit[4];
    uint8_t _refreshIterator;
};

}

#endif