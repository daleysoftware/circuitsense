// -----------------------------------------------------------------------------
// This header contains the public prototypes for the filter library. The
// filter library is used to create filters images and connection images of
// input circuit images using the opencv library.
//
// This has previously been done with the image magick library - we are now
// doing it with opencv to experiement with speed.
//
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_FILTER_FILTER_H
#define API_FILTER_FILTER_H

#include <opencv/cv.h>

namespace cs
{

// -----------------------------------------------------------------------------
// class Filter
// -----------------------------------------------------------------------------

class Filter
{
private:
    // Source image and resized image.
    IplImage& m_src;
    IplImage* m_sgray;
    IplImage* m_connblob;

    // Small temp images (grayscale).
    IplImage* m_stmp1;
    IplImage* m_stmp2;

    // The max dimension of the resized image.
    int m_maxdim;

    // Functions
    void Initialize();
    void ConnectionBlobInternal();

public:
    // Constructor. Ownership of src NOT passed to this object (we are NOT
    // responsible for destroying it).
    //
    // The input IplImage& src must be an RBG image.
    Filter(IplImage& src, unsigned int maxdim) :
        m_src(src),
        m_sgray(0),
        m_connblob(0),
        m_stmp1(0),
        m_stmp2(0),
        m_maxdim(maxdim)
    {
        // Resize so the mutators are ready to do their thing. Also initialize
        // m_[s]gray and m_stmpX. After this call, we will be ready to perform
        // ConnectionBlob() or ComponentBlob().
        Initialize();
    }

    // Destructor
    ~Filter();

    // Accessors
    const IplImage& GetSrc() const {return m_src;}

    // Mutators
    const IplImage* SmallGray() {return m_sgray;}
    const IplImage* ConnectionBlob() {return m_connblob;}
    const IplImage* ComponentBlob(unsigned int threshold);

    // Statics
    static bool Invert(IplImage* img);
    static IplImage* ResizeGray(IplImage* src, int maxdim);
};

} // end namespace cs

#endif
