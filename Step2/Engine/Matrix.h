// ��������� �� ������ �аڴٴ� Ű����
#pragma once

// template : Ŭ�������� template�� ������ �� ����κ��� ��������� �ۼ��ؾ� �۵� ����
template<size_t m, size_t n>
// final : Matrix�� ���� �Ļ� Ŭ���� ������ ����
class Matrix final
{
    // Matrix Class�� ���ؼ��� private�� ����
    friend class Matrix;

public:
    // ... : �����ڸ� ȣ���ϴ� �������� ������ ������ ����
    //  :  : �޼ҵ带 �����ϱ� ���� ����
#pragma region Static �ΰ� ���� ***
/*
static : ���� �����μ� �����Ǹ� ���α׷� ���� �������� ����
static_cast : �����ϴ� ���� �Ͻ������� ������ �� ��ȯ ����

int num = 10;
static_cast<float>(num);
// - cast�ϴ� �������� num�� float���� ����ǰ� �� �ķ� int�� ���
*/
#pragma endregion

#pragma region Static_assert ***
/*
static_assert(constant-expression, string-literal);
// constant-expression : bool������ return
// string-literal : ��ȯ�� bool���� false�� ��, �����Ϸ��� ǥ���ϴ� ����
*/
#pragma endregion

    template<typename... Type>
    Matrix(Type const &... element) : element { static_cast<float>(element)... }
    {
        // assert : run time ���߿� ������ ����� ���α׷� ���� ����
        // static_assert : compile time ���߿� ������ ����� ���α׷� ���� ����

        // m�� n�� �� �� 1�� �ƴ� �� ���� ����
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
        // constexpression : compile time�� ���ǹ��� �̸� ������ �� �ִ� Ű����
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

    // �迭�� Index��ȣ ��ȯ
    auto & operator [](size_t const & index)
    {
             if constexpr (m != 1 and n == 1) return (*( element)[index]);
        else if constexpr (m == 1 and n != 1) return ( (*element)[index]);
        else if constexpr (m != 1 and n != 1) return ( ( element)[index]);
    }

    auto & operator [](size_t const & index) const
    {
        // const_cast : const(������)�� �Ͻ������� ����
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