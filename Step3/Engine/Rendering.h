#pragma once
#include "Vector.h"

namespace Rendering
{
    class Camera final
    {
    public:
        void Set() const;

    public:
        Vector<2> Length   = { 1280, 720 };
        float     Angle    = { 0 };
        Vector<2> Location = { 0, 0 };
    };

 namespace Image 
 { 
    class Component final
    {
    public:
        void Draw();

    public:
        char const* Content = nullptr;
    
    public:
        Vector<2> Length    = { 0, 0 };
        float     Angle     = { 0 };
        Vector<2> Location  = { 0, 0 };
    };
 }
}