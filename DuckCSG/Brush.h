#pragma once

#include <string>

namespace MapTools {

    struct PlanePoint
    {
        float x;
        float y;
        float z;
    };

    // TODO:
    struct UV
    {
        float x;
        float y;
    };

    class Brush
    {
    public:
        Brush();

        void setPoint1(PlanePoint point);
        void setPoint2(PlanePoint point);
        void setPoint3(PlanePoint point);

        void setTextureString(std::string texture);

        void setUV(UV uv);

        void setScaleU(float scaleU);
        void setScaleV(float scaleV);
    private:
    };

}