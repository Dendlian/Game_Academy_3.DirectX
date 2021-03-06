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

    namespace Text
    {
        class Component final
        {
        public:
            void Draw();

        public:
            // Content : 경로
            char const* Content = nullptr;

            struct
            {
                char const* Name            = nullptr;
                unsigned    Size            = 0;
                bool        Bold            = false;    // 굵기
                bool        Italic          = false;    // 기울기
                bool        Underlined      = false;   
                bool        StrikeThrough   = false;    // 삭제선
            }Font;

            struct
            {
                unsigned char Red   = 0;
                unsigned char Green = 0;
                unsigned char Blue  = 0;
            }Color;

        public:
            Vector<2> Length   = { 0.0f, 0.0f };
            Vector<2> Location = { 0.0f, 0.0f };
        };
    }

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

 namespace Animation
 {
     class Component final
     {
     public:
         void Door_Draw();
         void Player_Draw();
         void Magic_Draw(int currentline);
         void Zombie_Draw(Vector<2> PZ);

     public:
         char const* Content = nullptr;

         float Playback = 0;
         float Duration = 0;

         int   Line = 1;
         int   CurrentLine = 1;

         bool  Repeatable = false;
     
     public:
         Vector<2> Length   = { 0,0 };
         float     Angle    = { 0 };
         Vector<2> Location = { 0,0 };
     };
 }

}