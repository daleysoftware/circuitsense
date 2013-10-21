// -----------------------------------------------------------------------------
// This file contains the core implementation of the tesseract library.
// This library is intented to replace the old tesseract library that used the
// dirty old c-style api. This library also uses opencv instead of manipulating
// files on disk.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <tesseract/baseapi.h>
#include <opencv/highgui.h>

#include "Tesseract.h"
#include "ResultEnglish.h"
#include "ResultCircuits.h"

namespace cs
{

static int iswhitespace(char c)
{
    return (c == ' ' || c == '\r' || c == '\n');
}

Tesseract::Tesseract(IplImage& src) : m_src(src)
{
    m_gray = cvCreateImage(cvGetSize(&m_src), IPL_DEPTH_8U, 1);
    cvCvtColor(&m_src, m_gray, CV_BGR2GRAY);
}

Tesseract::~Tesseract()
{
    cvReleaseImage(&m_gray);
}

ResultEnglish Tesseract::TesseractEnglish(Rectangle rect)
{
    ResultEnglish result;

    // Set the region of interest and copy the image in memory.
    cvSetImageROI(m_gray, cvRect(rect.GetX(), rect.GetY(),
                rect.GetWidth(),rect.GetHeight()));

    IplImage* roi = cvCreateImage(cvGetSize(m_gray), m_gray->depth, 1);
    cvCopy(m_gray, roi);

    // Reset the region of interest.
    cvResetImageROI(m_gray);

    // Now, process the copied image, roi.
    tesseract::TessBaseAPI api;
    api.Init(".", "eng");
    api.SetVariable("save_best_choices", "T");
    api.SetVariable("tessedit_char_whitelist", "pnumkMG0123456789.VHF");
    api.SetPageSegMode(tesseract::PSM_SINGLE_WORD);

    api.SetImage((uint8_t*) roi->imageData, roi->width, roi->height, 1,
            roi->widthStep);

    result.m_confidence = api.MeanTextConf();

    char* utf8 = api.GetUTF8Text();
    result.m_chars = std::string(utf8);
    delete[] utf8;

    result.m_chars.erase(std::remove_if(result.m_chars.begin(),
                result.m_chars.end(), iswhitespace), result.m_chars.end());

    // Cleanup and return.
    cvReleaseImage(&roi);
    return result;
}

ResultCircuits Tesseract::TesseractCircuits(Rectangle rect)
{
    ResultCircuits result;

    // Set the region of interest and copy the image in memory.
    cvSetImageROI(m_gray, cvRect(rect.GetX(), rect.GetY(),
                rect.GetWidth(),rect.GetHeight()));

    IplImage* roi = cvCreateImage(cvGetSize(m_gray), m_gray->depth, 1);
    cvCopy(m_gray, roi);

    // Reset the region of interest.
    cvResetImageROI(m_gray);

    // Now, process the copied image, roi.
    tesseract::TessBaseAPI api;
    api.Init(".", "cir");
    api.SetVariable("save_best_choices", "T");
    api.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

    api.SetImage((uint8_t*) roi->imageData, roi->width, roi->height, 1,
            roi->widthStep);

    result.m_confidence = api.MeanTextConf();

    char* utf8 = api.GetUTF8Text();
    std::string chars = std::string(utf8);
    delete[] utf8;

    chars.erase(std::remove_if(chars.begin(), chars.end(), iswhitespace),
            chars.end());

    for (unsigned int i = 0; i < chars.length(); i++)
    {
        result.m_vector.push_back(ComponentID(chars[i]));
    }

    // Cleanup and return.
    cvReleaseImage(&roi);
    return result;
}

} // end namespace cs
