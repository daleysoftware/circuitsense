// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_TESS_TESSERACT_H
#define API_TESS_TESSERACT_H

#include <opencv/cv.h>
#include <string>

#include "Rectangle.h"

namespace cs
{

class ResultEnglish;
class ResultCircuits;

// -----------------------------------------------------------------------------
// class Tesseract
// -----------------------------------------------------------------------------

class Tesseract
{
private:
    // Source image.
    IplImage& m_src;
    IplImage* m_gray;

public:
    // Constructor.
    Tesseract(IplImage& src);

    // Destructor.
    ~Tesseract();

    // Mutators
    ResultEnglish TesseractEnglish(Rectangle rect);
    ResultCircuits TesseractCircuits(Rectangle rect);
};

} // end namespace cs

#endif
