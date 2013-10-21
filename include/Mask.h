// -----------------------------------------------------------------------------
// This class is used to whiteout the edges of images for peak detection.
//
// Copyright (c) 2011 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_WHITEOUT_MASK_H
#define API_WHITEOUT_MASK_H

#include <stdint.h>
#include <opencv/cv.h>

namespace cs
{

// -----------------------------------------------------------------------------
// class Mask
// -----------------------------------------------------------------------------

class Mask
{
private:
    // Color components.
    uint8_t m_blue;
    uint8_t m_green;
    uint8_t m_red;

public:
    // Constructor.
    Mask(uint8_t blue, uint8_t green, uint8_t red)
    {
        // White color component is not allowed, because it will confuse the
        // whiteout class.
        m_blue  = blue  >= 0x7f ? blue  : 0x7f;
        m_green = green >= 0x7f ? green : 0x7f;
        m_red   = red   >= 0x7f ? red   : 0x7f;
    }

    // Accessors
    uint8_t GetBlue() const {return m_blue;}
    uint8_t GetGreen() const {return m_green;}
    uint8_t GetRed() const {return m_red;}

};

} // end namespace cs

#endif
