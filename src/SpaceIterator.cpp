// -----------------------------------------------------------------------------
// Copyright (c) 2011-2012 Matt Pillar. All rights reserved.
// -----------------------------------------------------------------------------

#include <iostream>
#include <exception>
#include <vector>

#include <assert.h>

#include "Exception.h"
#include "Whiteout.h"
#include "SpaceIterator.h"

namespace cs
{

SpaceIterator::SpaceIterator(Space space) :
    m_space(space),
    m_done(false),
    m_current(0,0),
    m_starting(0,0)
{
    // Nothing to do.
}

void SpaceIterator::operator++(int)
{
    m_current.IncrX();

    if (m_current.GetX() >= m_space.GetWidth())
    {
        m_current.SetX(0);

        m_current.IncrY();
        m_current.SetY(m_current.GetY() % m_space.GetHeight());
    }

    if (m_current == m_starting)
    {
        m_done = true;
    }
}

}
