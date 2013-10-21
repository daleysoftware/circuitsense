// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef API_TESS_RESULTENGLISH_H
#define API_TESSS_RESULTENGLISH_H

namespace cs
{

class Tesseract;

// -----------------------------------------------------------------------------
// class Tesseract
// -----------------------------------------------------------------------------

class ResultEnglish
{
private:
    // Members
    std::string m_chars;
    int m_confidence;
    bool m_error;

public:
    // Constructor
    ResultEnglish() :
        m_confidence(0),
        m_error(false)
    {
        // no-op
    }

    // Accessors
    bool GetError() const {return m_error;}
    int GetConfidence() const {return m_confidence;}
    const std::string& GetChars() const {return m_chars;}

    // Friends
    friend class Tesseract;
};

} // end namespace cs

#endif
