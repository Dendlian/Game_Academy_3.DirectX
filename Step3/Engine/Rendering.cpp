#include <Windows.h>

#define _USE_MATH_DEFINES

#include "PipeLine.h"
#include "FreeImage.h"
#include "Rendering.h"

#include <map>
#include <string>

#include "Time.h"
#include "Resource.h"

namespace Rendering
{
    namespace Pipeline
    {
        void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
    }

    namespace
    {
        inline Matrix<4, 4> Scale(Vector<2> const& length)
        {
            return Matrix<4, 4>
            {
                length[0], 0        , 0, 0,
                0        , length[1], 0, 0,
                0        , 0        , 1, 0,
                0        , 0        , 0, 1
            };
        }
        inline Matrix<4, 4> Rotation(float const& angle)
        {
            float const radian = static_cast<float>(M_PI) / 180.0f * angle;

            return Matrix<4, 4>
            {
                +cos(radian), +sin(radian), 0, 0,
                -sin(radian), +cos(radian), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }
        
        inline Matrix<4, 4> Translation(Vector<2> const& location)
        {
            return Matrix<4, 4>
            {
                1, 0, 0, location[0],
                0, 1, 0, location[1],
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }
    }

    // World, View(Camera), Projection

    void Camera::Set() const
    {
        using namespace Pipeline;
      
        Matrix<4, 4> const projection = Scale(Vector<2>(2 / Length[0], 2 / Length[1])); 
        Matrix<4, 4> const view = Rotation(-Angle) * Translation(-Location);
        Matrix<4, 4> const latter = projection * view;

        Transform::Update<Transform::Type::Latter>(reinterpret_cast<Transform::Matrix const&>(latter));
    }

    namespace Text
    {
        void Import(std::string const& file)
        {
            // API(GDI) �Լ� ���
            AddFontResourceEx(file.data(), FR_PRIVATE | FR_NOT_ENUM, nullptr);
        }

        void Component::Draw()
        {
            LOGFONT descriptor = LOGFONT();

            descriptor.lfHeight     = Font.Size;
            descriptor.lfWeight     = Font.Bold ? FW_BOLD : FW_NORMAL;
            descriptor.lfItalic     = Font.Italic;
            descriptor.lfUnderline  = Font.Underlined;
            descriptor.lfStrikeOut  = Font.StrikeThrough;

            // ��Ʈ ����ü�� ����� �� ���� ������ ��� ����
            // DEFAULT_CHARSET : �⺻���� ����, �ѱ��� �۲� ����
            descriptor.lfCharSet    = DEFAULT_CHARSET;

            // LF_FACESIZE : ��ü�� �Ҵ�Ǿ� �ִ� ���� ũ�� ���� (FaceName�� �ִ� 32Byte)
            // lfFaceName�� Font.Name�� ��� ���ڿ��� �����Ͽ� ����
            // Font.Name : ������ ���� ��ü �̸�
            strcpy_s(descriptor.lfFaceName, LF_FACESIZE, Font.Name);

            // descriptor�� �������� font ����
            HFONT const font = CreateFontIndirect(&descriptor);

            SIZE const area    = { static_cast<LONG>(  Length[0]), static_cast<LONG>(  Length[1]) };
            POINT const center = { static_cast<LONG>(Location[0]), static_cast<LONG>(Location[1]) };

            Pipeline::String::Render(font, Content, RGB(Color.Red, Color.Green, Color.Blue), area, center);

            DeleteObject(font);
        }
    }
    

    namespace Image
    {
        // Resource Shader�� �ش� �̹����� ������ ����
        struct Descriptor final
        {
        public:
            Pipeline::Texture::Handle* Handle = nullptr;

        public:
            SIZE Size = SIZE();
        };

        std::map<std::string, Descriptor> Storage;

#pragma region Image::Import ***
/*
1. if��
    - Bitmap���� ���� �����Ͱ� 32bit�� �ƴ϶�� �ٽ� 32bit�� ��ȯ

2. size_t const x = file.find_first_of('/') + sizeof(char);
   size_t const y = file.find_last_of('.');
    - x : ���ڿ��� ù��° ������ ��ġ
    - y : ���ڿ��� ������ ������ ��ġ

3. Image::Storage.try_emplace(file.substr(x, y - x), descriptor);
    - try_emplace(Image�� �̸�, Container) : Container(map)�� Image�� ����
    - file.substr(size_type pos = 0, size_type count = npos) const;
        - pos   : ù��° ������ ��ġ (x)
        - count : �κ� ���ڿ��� ���� (y-x)
*/
#pragma endregion

        void Import(std::string const& file)
        {
            {
                FIBITMAP* Bitmap = FreeImage_Load(FreeImage_GetFileType(file.data()), file.data());

                FreeImage_FlipVertical(Bitmap);

                // Bitmap�� 32bit�� �ƴ϶�� �ٽ� 32bit�� ��ȯ
                if (FreeImage_GetBPP(Bitmap) != 32)
                {
                    FIBITMAP* const previous = Bitmap;

                    Bitmap = FreeImage_ConvertTo32Bits(Bitmap);

                    FreeImage_Unload(previous);
                }

                Image::Descriptor descriptor = Image::Descriptor();
                {
                    descriptor.Size.cx = FreeImage_GetWidth(Bitmap);
                    descriptor.Size.cy = FreeImage_GetHeight(Bitmap);

                    Pipeline::Texture::Create(descriptor.Handle, descriptor.Size, FreeImage_GetBits(Bitmap));
                }
                {
                    size_t const x = file.find_first_of('/') + sizeof(char);
                    size_t const y = file.find_last_of('.');

                    // Container�� Image�� ���� ��Ҹ� �߰�
                    Image::Storage.try_emplace(file.substr(x, y - x), descriptor);
                }
                FreeImage_Unload(Bitmap);
            }
        }

        void Component::Draw()
        {
            using namespace Pipeline;
            {
                Matrix<4, 4> const world = Translation(Location) * Rotation(Angle) * Scale(Length);
                Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::Matrix const&>(world));
            }
            {
                // at(Content) : Content�� Ű���� ��ȯ
                Descriptor const& image = Storage.at(Content);

                Texture::Render(image.Handle, { 0, 0, image.Size.cx, image.Size.cy });
            }
        }
    }

    namespace Animation
    {
        void Component::Draw()
        {
            using namespace Pipeline;
            {
                // Animation ���� ��� �̵�, ȸ���� �� �� �ֱ� ������ ���� �׿� ���� ����� ���� 
                Matrix<4, 4> const world = Translation(Location) * Rotation(Angle) * Scale(Length);

                Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::Matrix const&>)(world);
            }
        }
    }

    void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
    {
        switch (uMessage)
        {
            case WM_CREATE:
            {  
                Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

                FreeImage_Initialise();
                {
                    Resource::Import("Image", Image::Import);
                }
                FreeImage_DeInitialise();

                return;
            }
            case WM_APP:
            {
                Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

                return;
            }
            case WM_DESTROY:
            {
#pragma region �迭 ��� for�� ***
/*
    int Array[5] = {1, 2, 3, 4, 5};
    for(int i = 0; i < 5, i++)
    {
        Array[i] = i;
    }
    for(int i : Array) // Array�� �� ��ҿ� i�� ���������� ����
    {
        i << Array[i];
    }
*/
#pragma endregion

                using namespace std;
                using namespace Image;

                // pair(Key, Value) : map�� ��Ҹ� �ϳ��� �˻��Ͽ� ����
                for (pair<string, Descriptor> const& pair : Storage)
                    Pipeline::Texture::Delete(pair.second.Handle);

                Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);
                return;
            }
            case WM_SIZE:
            {
                Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

                return;
            }
        }
    }
}