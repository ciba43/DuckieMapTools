#include "duckpch.h"
#include "Brush.h"

#include "Plane.h"

namespace MapTools {

    Brush::Brush()
    {
    }

    void Brush::pushPlane(Ref<Plane> plane)
    {
        m_planes.push_back(plane);
    }

    bool Brush::isValid()
    {
        // TODO:
        return false;
    }
}
