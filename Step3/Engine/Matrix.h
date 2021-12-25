#pragma once

template<size_t m, size_t n>
class Matrix final
{
    friend class Matrix;

public:
    template<typename... Type>
    Matrix(Type const &... element) : element { static_cast<float>(element)... }
    {
        static_assert(not (m == 1 and n == 1), "Matrix<1, 1> cannot be created.");
    }

    auto operator +(void) const
    {
        return *this;
    }

    auto operator +(Matrix<m, n> const & other) const
    {
        Matrix<m, n> product = *this;

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

    auto & operator [](size_t const & index)
    {
             if constexpr (m != 1 and n == 1) return (*( element)[index]);
        else if constexpr (m == 1 and n != 1) return ( (*element)[index]);
        else if constexpr (m != 1 and n != 1) return ( ( element)[index]);
    }

    auto & operator [](size_t const & index) const
    {
        return (*const_cast<Matrix<m, n> *>(this))[index];
    }

private:
    float element[m][n];
};

template<size_t m, size_t n>
inline Matrix<m, n> const transpose(Matrix<m, n> const& other)
{
    if constexpr (m == 1 or n == 1)
    {
        return reinterpret_cast<Matrix<n, m> const&>(other);
    }
    else
    {
        Matrix<n, m> result = Matrix<n, m>();
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                result[j][i] = other[i][j];

        return result;
    }
}