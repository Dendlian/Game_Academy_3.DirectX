#pragma once

#include <cmath>

#include "Matrix.h"

template<size_t n>
using Vector = Matrix<1, n>;

template<size_t n>
inline float Dot(Vector<n> const & one, Vector<n> const & other)
{
    return one * transpose(other);
}

template<size_t n>
inline float Length(Vector<n> const & one)
{
    // Root 연산을 수행 후 float 형태로 변환
    return sqrt(Dot(one, one));
}

template<size_t n>
inline Vector<n> Normalize(Vector<n> const & one)
{
    return one / Length(one);
}

inline Vector<3> Cross(Vector<3> const & one, Vector<3> const & other)
{
    Vector<3> product = Vector<3>();

    for(size_t u = 0; u < 3; ++u)
    {
        size_t v = (u + 1) % 3;
        size_t w = (v + 1) % 3;

        product[u] = one[v] * other[w] - one[w] * other[v];
    }

    return product;
}

#pragma region Cast & Inline ***
/*
// Cast
 - static_cast      : 정적 변수에 대해 형변환을 실행
 - dynamic_cast     : class에서 상속자와 파생자 관계라면 형변환을 허용
 - const_cast       : const를 일시적으로 해제하여 형변환을 허용
 - reinterpret_cast : 임의로 포인터끼리의 형변환을 허용

// Inline
 - funtion : code area에 저장되고 함수를 호출 시 직접 찾아가서 실행
 - inline : code area에서 함수의 내용을 배송하여 바로 실행
*/
#pragma endregion