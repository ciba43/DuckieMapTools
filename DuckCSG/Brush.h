#pragma once

#include <string>
#include <vector>

namespace MapTools {

    class Plane;

    class Brush
    {
    public:
        Brush();
        void pushPlane(Ref<Plane> plane);
        bool isValid(); // A brush needs to be at least 4 planes to be considered valid (among other requirements)
    private:
        std::vector<Ref<Plane>> m_planes;
    };
}
