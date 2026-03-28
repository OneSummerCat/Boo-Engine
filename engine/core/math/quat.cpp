#include "quat.h"

namespace Boo
{

    Quat::Quat() : _x(0), _y(0), _z(0), _w(1)
    {
    }
    Quat::Quat(float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w)
    {
    }
    void Quat::set(float x, float y, float z, float w)
    {
        this->_x = x;
        this->_y = y;
        this->_z = z;
        this->_w = w;
    }
    void Quat::setX(float x)
    {
        this->_x = x;
    }
    void Quat::setY(float y)
    {
        this->_y = y;
    }
    void Quat::setZ(float z)
    {
        this->_z = z;
    }
    void Quat::setW(float w)
    {
        this->_w = w;
    }
    float Quat::getX() const
    {
        return this->_x;
    }
    float Quat::getY() const
    {
        return this->_y;
    }
    float Quat::getZ() const
    {
        return this->_z;
    }
    float Quat::getW() const
    {
        return this->_w;
    }

    Quat::~Quat()
    {
    }

} // namespace Boo
