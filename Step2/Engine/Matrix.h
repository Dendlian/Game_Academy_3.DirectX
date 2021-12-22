// 헤더파일을 한 번씩만 읽겠다는 키워드
#pragma once

// template : 클래스에서 template를 선언할 때 선언부분인 헤더에서만 작성해야 작동 가능
template<size_t m, size_t n>
// final : Matrix에 대한 파생 클래스 생성을 금지
class Matrix final
{
    // Matrix Class에 대해서만 private을 공유
    friend class Matrix;

public:
    // ... : 생성자를 호출하는 시점에서 인자의 갯수를 설정
    //  :  : 메소드를 정의하기 전에 설정
#pragma region Static 부가 설명 ***
/*
static : 정적 변수로서 생성되면 프로그램 종료 시점까지 존재
static_cast : 설정하는 동안 일시적으로 변수의 형 변환 적용

int num = 10;
static_cast<float>(num);
// - cast하는 시점에서 num은 float으로 적용되고 그 후로 int로 사용
*/
#pragma endregion

#pragma region Static_assert ***
/*
static_assert(constant-expression, string-literal);
// constant-expression : bool값으로 return
// string-literal : 반환된 bool값이 false일 때, 컴파일러가 표현하는 구문
*/
#pragma endregion

    template<typename... Type>
    Matrix(Type const &... element) : element { static_cast<float>(element)... }
    {
        // assert : run time 도중에 문제가 생기면 프로그램 강제 종료
        // static_assert : compile time 도중에 문제가 생기면 프로그램 강제 종료

        // m과 n이 둘 다 1이 아닐 때 정상 실행
        static_assert(not (m == 1 and n == 1), "Matrix<1, 1> cannot be created.");
    }

    auto operator +(void) const
    {
        return *this;
    }

    auto operator +(Matrix<m, n> const & other) const
    {
        Matrix<m, n> product = *this;

        // size_t : UINT (32Bit), unsigned long long int (64Bit) (typedef)
        for(size_t u = 0; u < m; ++u)
            for(size_t v = 0; v < n; ++v)
                product.element[u][v] += other.element[u][v];

        return product;
    }

    auto & operator +=(Matrix<m, n> const & other)
    {
        return *this = *this + other;
    }

    auto operator *(float const & other) const
    {
        Matrix<m, n> product = *this;

        for(size_t u = 0; u < m; ++u)
            for(size_t v = 0; v < n; ++v)
                product.element[u][v] *= other;

        return product;
    }

    auto & operator *=(float const & other)
    {
        return *this = *this * other;
    }

    auto operator -(void) const
    {
        return *this * -1;
    }

    auto operator -(Matrix<m, n> const & other) const
    {
        return *this + (-other);
    }

    auto & operator -=(Matrix<m, n> const & other)
    {
        return *this = *this - other;
    }

    auto operator /(float const & other) const
    {
        return *this * (1 / other);
    }

    auto & operator /=(float const & other)
    {
        return *this = *this / other;
    }

    template<size_t l>
    auto operator *(Matrix<n, l> const & other) const
    {
        // constexpression : compile time에 조건문을 미리 실행할 수 있는 키워드
        if constexpr (m == 1 and l == 1)
        {
            float product = float();

            for(size_t u = 0; u < n; ++u)
                product += (*this).element[0][u] * other.element[u][0];

            return product;
        }
        else
        {
            Matrix<m, l> product = Matrix<m, l>();

            for(size_t u = 0; u < m; ++u)
                for(size_t v = 0; v < l; ++v)
                    for(size_t w = 0; w < n; ++w)
                        product.element[u][v] += (*this).element[u][w] * other.element[w][v];

            return product;
        }
    }

    auto & operator *=(Matrix<n, n> const & other)
    {
        return *this = *this * other;
    }

    // 배열의 Index번호 반환
    auto & operator [](size_t const & index)
    {
             if constexpr (m != 1 and n == 1) return (*( element)[index]);
        else if constexpr (m == 1 and n != 1) return ( (*element)[index]);
        else if constexpr (m != 1 and n != 1) return ( ( element)[index]);
    }

    auto & operator [](size_t const & index) const
    {
        // const_cast : const(한정자)를 일시적으로 해제
        return (*const_cast<Matrix<m, n> *>(this))[index];
    }

private:
    float element[m][n];
};

/*
template<unsigned m, unsigned n>
inline auto operator *(float const & one, Matrix<m, n> const & other)
{
    return other * one;
}

template<unsigned m, unsigned n>
inline auto operator /(float const & one, Matrix<m, n> const & other)
{
    return other / one;
}
*/