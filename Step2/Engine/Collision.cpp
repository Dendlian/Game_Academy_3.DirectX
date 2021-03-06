#define _USE_MATH_DEFINES

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

    inline Matrix<2, 2> Rotation(float const& angle)
    {
        float constexpr radian = static_cast<float>(M_PI) / 180.0f;
        
        return Matrix<2, 2>
        {
            +cos(angle * radian), +sin(angle * radian),
            -sin(angle * radian), +cos(angle * radian)
        };
    }

#pragma region Point vs Point
    bool Collide(Point const& LHS, Point const& RHS)
    {
        return Length(LHS.Location - RHS.Location) == 0;
    }
#pragma endregion

#pragma region Circle vs Circle
    bool Collide(Circle const& LHS, Circle const& RHS)
    {
        return Length(LHS.Location - RHS.Location) <= (LHS.Diameter/2 + RHS.Diameter/2);
    }
#pragma endregion

#pragma region Rectangle vs Rectangle
    bool Collide(RectAngle const& LHS, RectAngle const& RHS)
    {
        if (LHS.Angle == 0 && RHS.Angle == 0)
        {
            Vector<2> L_min = LHS.Location - RHS.Length / 2;
            Vector<2> L_max = LHS.Location + RHS.Length / 2;

            Vector<2> R_min = RHS.Location - RHS.Length / 2;
            Vector<2> R_max = RHS.Location + RHS.Length / 2;

            float Min[2][2] =
            {
                {L_min[0], L_min[1]},
                {R_min[0], R_min[1]}
            };

            float Max[2][2] =
            {
                {L_max[0], L_max[1]},
                {R_max[0], R_max[1]}
            };

            return
                (
                 (Min[0][0] <= Max[1][0]) and (Min[0][1] <= Max[1][1]) and
                 (Min[1][0] <= Max[0][0]) and (Min[1][1] <= Max[0][1])
                );
        }
        else if (abs(LHS.Angle - RHS.Angle) < 0.1f)
        {
            return Collide
            (
                RectAngle{ LHS.Length, 0, LHS.Location * Rotation(-LHS.Angle) },
                RectAngle{ RHS.Length, 0, RHS.Location * Rotation(-RHS.Angle) }
            );
        }
        else
        {

        }
}
#pragma endregion

#pragma region Point vs Circle
    bool Collide(Point const& LHS, Circle const& RHS)
    {
        return Length(LHS.Location - RHS.Location) <= RHS.Diameter/2;
    }

    bool Collide(Circle const& LHS, Point const& RHS)
    {
        return Collide(RHS, LHS);
    }
#pragma endregion
    
#pragma region Point vs Rectangle
    bool Collide(Point const& LHS, RectAngle const& RHS)
    {
        if (RHS.Angle == 0.0f) 
        {
            Vector<2> const min = RHS.Location - RHS.Length / 2;
            Vector<2> const max = RHS.Location + RHS.Length / 2;

            return
                (
                    (min[0] <= LHS.Location[0] and LHS.Location[0] <= max[0]) and
                    (min[1] <= LHS.Location[1] and LHS.Location[1] <= max[1])
                );
        }
        else
        {
            return Collide
            (
                Point{ LHS.Location * Rotation(-RHS.Angle) },
                RectAngle{ RHS.Length, 0, RHS.Location * Rotation(-RHS.Angle) }
            );
        }
    }

    bool Collide(RectAngle const& LHS, Point const& RHS)
    {
        return Collide(RHS, LHS);
    }
#pragma endregion

#pragma region Circle vs Rectangle
    bool Collide(Circle const& LHS, RectAngle const& RHS)
    {
        if (RHS.Angle == 0.0f)
        {
            Vector<2> min = RHS.Location - RHS.Length / 2;
            Vector<2> max = RHS.Location + RHS.Length / 2;
 
            // ?? ????
            if (
                   (min[0] <= LHS.Location[0] and LHS.Location[0] <= max[0]) or
                   (min[1] <= LHS.Location[1] and LHS.Location[1] <= max[1])
               )
            {
                return Collide
                    (
                        Point     { LHS.Location },
                        RectAngle { RHS.Length + Vector<2>(1, 1) * LHS.Diameter, 0 , RHS.Location }
                    );
            }
            // ?????? ????
            else
            {
                Point Rectpoint = Point();
                
                if (LHS.Location[0] >= max[0] && LHS.Location[1] >= max[1])
                    Rectpoint.Location[0] = max[0], Rectpoint.Location[1] = max[1];
                
                if (LHS.Location[0] >= max[0] && LHS.Location[1] <= min[1])
                    Rectpoint.Location[0] = max[0], Rectpoint.Location[1] = min[1];

                if (LHS.Location[0] <= min[0] && LHS.Location[1] >= max[1])
                    Rectpoint.Location[0] = min[0], Rectpoint.Location[1] = max[1];

                if (LHS.Location[0] <= min[0] && LHS.Location[1] <= min[1])
                    Rectpoint.Location[0] = min[0], Rectpoint.Location[1] = min[1];

                return Collide(LHS, Rectpoint);
            }
        }
        else
        {
            return Collide
            (
                Circle    { LHS.Diameter, LHS.Location * Rotation(-RHS.Angle) },
                RectAngle { RHS.Length, 0, RHS.Location * Rotation(-RHS.Angle) }
            );
          
        }
    }

    bool Collide(RectAngle const& LHS, Circle const& RHS)
    {
        return Collide(RHS, LHS);
    }
#pragma endregion

}
