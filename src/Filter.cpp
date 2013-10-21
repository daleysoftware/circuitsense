// -----------------------------------------------------------------------------
// Implementation of the filter library. Contains the opencv equivalent
// of the image magick filters that were originally used for the demo.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>

#include "Filter.h"

namespace cs
{

// -----------------------------------------------------------------------------
// Static configuration
//
// Note the component blob smooth parameter is dynamic and specified by the
// user.
// -----------------------------------------------------------------------------

// First param (threshold) to canny edge detector in the connection blob
// function.
static unsigned int g_connCannyParam1 = 50;

// Second param to canny edge detector in the connection blob function.
static unsigned int g_connCannyParam2 = 100;

// Threshold param to cv threshold the connection blob function.
static unsigned int g_connThreshold = 200;

// Threshold param to cv threshold in the component blob function.
static unsigned int g_compThreshold = 70;

// -----------------------------------------------------------------------------
//  filter implementation
// -----------------------------------------------------------------------------

void Filter::Initialize()
{
    // Resize the image. Store the small gray image in m_sgray.
    m_sgray = ResizeGray(&m_src, m_maxdim);

    // Initialize the temps but don't put anything in them yet.
    m_connblob = cvCreateImage(cvGetSize(m_sgray), IPL_DEPTH_8U, 1);
    m_stmp1 = cvCreateImage(cvGetSize(m_sgray), IPL_DEPTH_8U, 1);
    m_stmp2 = cvCreateImage(cvGetSize(m_sgray), IPL_DEPTH_8U, 1);

    // Find the connection blob.
    ConnectionBlobInternal();
}

Filter::~Filter()
{
    // Note: release on a 0 pointer is a no-op. Do NOT destroy src.
    cvReleaseImage(&m_sgray);
    cvReleaseImage(&m_connblob);
    cvReleaseImage(&m_stmp1);
    cvReleaseImage(&m_stmp2);
}

void Filter::ConnectionBlobInternal()
{
    // Edge detection and inversion.
    cvCanny(m_sgray, m_stmp1, g_connCannyParam1, g_connCannyParam2, 3);
    Invert(m_stmp1);

    // Repeating smoothing and thresholding to accent lines.
    for (unsigned int i = 0; i < 5; i++)
    {
        cvSmooth(m_stmp1, m_stmp2);
        cvThreshold(m_stmp2, m_stmp1, g_connThreshold, 255, CV_THRESH_BINARY);
    }

    cvCopy(m_stmp1, m_connblob);
}

// 21 is a good param, so 10 is a good threshold. Probably a good idea to vary
// from 0 to 20.
const IplImage* Filter::ComponentBlob(unsigned int threshold)
{
    unsigned int param = 2*threshold+1;

    cvSmooth(ConnectionBlob(), m_stmp1, CV_BLUR, param, param);
    cvThreshold(m_stmp1, m_stmp2, g_compThreshold, 255, CV_THRESH_BINARY);

    return m_stmp2;
}

bool Filter::Invert(IplImage* img)
{
    uint8_t* data;
    int i, j, k;

    if (img == 0)
    {
        return false;
    }

    data = (uint8_t*) img->imageData;
    for (i = 0; i < img->height; i++)
    {
        for (j = 0; j < img->width; j++)
        {
            for (k = 0; k < img->nChannels; k++)
            {
                // Could have used  point here, but the function
                // call would just slow us down.
                int index = i*img->widthStep + j*img->nChannels + k;
                data[index] = 0xff - data[index];
            }
        }
    }

    return true;
}

IplImage* Filter::ResizeGray(IplImage* src, int maxdim)
{
    int newHeight = src->height, newWidth = src->width;
    bool resizeRequired = false;

    // If we have 2000x1000 (col x row i.e. width x height) then we will end up
    // with maxdim x (maxdim/2) after the resize.
    //
    // Similarly, if we have 1000x2000 we will have (maxdim/2) x maxdim.
    if (src->height > src->width && src->height > maxdim)
    {
        resizeRequired = true;

        newHeight = maxdim;
        newWidth = (int) (src->width * ((double) maxdim / src->height));
    }
    else if (src->width > src->height && src->width > maxdim)
    {
        resizeRequired = true;

        newWidth = maxdim;
        newHeight = (int) (src->height * ((double) maxdim / src->width));
    }

    // Convert the RGB input image to a grayscale image.
    IplImage* gray = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    cvCvtColor(src, gray, CV_BGR2GRAY);

    // Perform the resize, if applicable, now that we're finished the math.
    CvSize newSize;
    newSize.height = newHeight;
    newSize.width = newWidth;

    IplImage* sgray;
    if (resizeRequired)
    {
        sgray = cvCreateImage(newSize, IPL_DEPTH_8U, 1);
        cvResize(gray, sgray);
    }
    else
    {
        sgray = (IplImage*) cvClone(gray);
    }

    cvReleaseImage(&gray);
    return sgray;
}

} // end namespace cs
