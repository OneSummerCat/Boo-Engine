#pragma once

namespace Boo
{

    class Quat
    {
    private:
        float _x;
        float _y;
        float _z;
        float _w;

    public:
        Quat();
        Quat(float x, float y, float z, float w);
        void set(float x, float y, float z, float w);
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        void setW(float w);
        float getX() const;
        float getY() const;
        float getZ() const;
        float getW() const;
        ~Quat();
    };

} // namespace Boo
