#include <Windows.h>
#include "PipeLine.h"
#include "FreeImage.h"
#include "Rendering.h"

namespace Rendering
{
    namespace Pipeline
    {
        void Procedure(HWND const, UINT const, WPARAM const, LPARAM const);
    }

    void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
    {
        static struct
        {
            Pipeline::Texture::Handle* handle = nullptr;
            SIZE size = SIZE();
        }Image;


        switch (uMessage)
        {
            case WM_CREATE:
            {  
                Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);
                using namespace Pipeline;
                {
                    char const* const File = "FreeImage.png";

                    FreeImage_Initialise();
                    {
                        FIBITMAP* Bitmap = FreeImage_Load(FreeImage_GetFileType(File), File);
                        
                        // ��½� �̹����� ������ ��µǴ� ���� �ٽ� �������ִ� �۾�
                        FreeImage_FlipVertical(Bitmap);

                        Image.size.cx = FreeImage_GetWidth(Bitmap);
                        Image.size.cy = FreeImage_GetHeight(Bitmap);

                        Texture::Create(Image.handle, Image.size, FreeImage_GetBits(Bitmap));
                        
                        FreeImage_Unload(Bitmap);
                    }
                    FreeImage_DeInitialise();

                    float x = 1;
                    float y = 0;

#pragma region Matrix ���� ***
                    /*
                        a1, a2, a3, a4
                        b1, b2, b3, b4
                        c1, c2, c3, c4
                        d1, d2, d3, d4

                        Transform : Scale. Translation, Rotation
                        Scale       : (a1, b2, c3)
                        Translation : (d1, d2, d3)
                        ConstantBuffer -> Vertex Shader

                        DirectX���� ����� Shader ����� �Ѿ �� �� �������� �� ������ ��ȯ�Ǿ� ó��
                        ��ġ��� : �밢������ ������ ���
                        �� �켱 Packing : ���԰� ��� ���������� ������ ����

                        World
                          // scale.x, 0,       0,     location.x,
                          // 0,       scale.y, 0,     location.y,
                          // 0,       0,       1,     0,
                          // 0,       0,       0,     1

                     // Matrix ���� ���� (��,��,��,��,��)
                     - Scale
                     - Rotation
                     - Translation
                     - Revolutuion
                     - Parent object

                        // Scale
                           (x, y) * (a, 0) = (x * a, y * b)
                                    (0, b)

                        // Translation
                                       (1, 0, 0)
                          (x, y, 1) *  (0, 1, 0) = (x + 2, y + 2, 1)
                                       (2, 2, 1)

                        // Rotation

                           (x, y) * (cos , sin)
                                    (-sin, cos)
                    */
#pragma endregion
                    Matrix<4, 4> world
                    {
                        1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1
                    };

                    float radian = static_cast<float>(3.14) / 180.0f;

                    Matrix<4, 4> rotation
                    {
                        cos(60*radian),sin(60 * radian),0,0,
                        -sin(60 * radian),cos(60 * radian),0,0,
                        0,0,1,0,
                        0,0,0,1
                    };

                    Matrix<4, 4> identity
                    {
                        1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,0,1
                    };

                    Transform::Update<Transform::Type::Former>(reinterpret_cast<Transform::Matrix const&>(world *= rotation));
                    Transform::Update<Transform::Type::Latter>(reinterpret_cast<Transform::Matrix const&>(identity));
                }
                return;
            }
            case WM_APP:
            {
                Pipeline::Procedure(hWindow, uMessage, wParameter, lParameter);

                Pipeline::Texture::Render(Image.handle, {0, 0, Image.size.cx, Image.size.cy});

                return;
            }
            case WM_DESTROY:
            {
                Pipeline::Texture::Delete(Image.handle);
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