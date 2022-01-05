#pragma once
#include "Vector.h"

namespace Collision
{
    struct Point final
    {
        Vector<2> Location;
    };

    struct Circle final
    {
        float Diameter;
        Vector<2> Location;
    };

    struct RectAngle final
    {
        Vector<2> Length;
        float Angle;
        Vector<2> Location;
    };

    bool Collide(Point      const& LHS, Circle      const& RHS);
    bool Collide(Point      const& LHS, RectAngle   const& RHS);
    bool Collide(Circle     const& LHS, Point       const& RHS);
    bool Collide(Circle     const& LHS, Circle      const& RHS);
    bool Collide(Circle     const& LHS, RectAngle   const& RHS);
    bool Collide(RectAngle  const& LHS, Point       const& RHS);
    bool Collide(RectAngle  const& LHS, Circle      const& RHS);
    bool Collide(RectAngle  const& LHS, RectAngle   const& RHS);
}
