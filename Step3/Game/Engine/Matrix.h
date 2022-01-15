#pragma once

template<size_t m, size_t n>
class Matrix final
{
    friend class Matrix;
private:
    float element[m][n];

public:
    template<typename... Type>
    Matrix(Type const &... element) : element{ static_cast<float>(element)... }
    {
        static_assert(not (m == 1 and n == 1), "Matrix<1, 1> cannot be created.");
    }

    auto operator +(void) const
    {
        return *this;
    }

    auto operator +(Matrix<m, n> const& M) const
    {s
        Matrix<m, n> product = *this;

        for (size_t u = 0; u < m; ++u)
            for (size_t v = 0; v < n; ++v)
                product.element[u][v] += M.element[u][v];

        return product;
    }

    auto& operator +=(Matrix<m, n> const& M)
    {
        return *this = *this + M;
    }

    auto operator -(void) const
    {
        return *this * -1;
    }

    auto operator -(Matrix<m, n> const& M) const
    {
        return *this + (-M);
    }

    auto& operator -=(Matrix<m, n> const& M)
    {
        return *this = *this - M;
    }

    auto operator *(float const& M) const
    {
        Matrix<m, n> product = *this;

        for (size_t u = 0; u < m; ++u)
            for (size_t v = 0; v < n; ++v)
                product.element[u][v] *= M;

        return product;
    }

    auto& operator *=(float const& M)
    {
        return *this = *this * M;
    }

    auto operator /(float const& M) const
    {
        return *this * (1 / M);
    }

    auto& operator /=(float const& M)
    {
        return *this = *this / M;
    }

    template<size_t l>
    auto operator *(Matrix<n, l> const& M) const
    {
        if constexpr (m == 1 and l == 1)
        {
            float product = float();

            for (size_t u = 0; u < n; ++u)
                product += (*this).element[0][u] * M.element[u][0];

            return product;
        }
        else
        {
            Matrix<m, l> product = Matrix<m, l>();

            for (size_t u = 0; u < m; ++u)
                for (size_t v = 0; v < l; ++v)
                    for (size_t w = 0; w < n; ++w)
                        product.element[u][v] += (*this).element[u][w] * M.element[w][v];

            return product;
        }
    }

    auto& operator *=(Matrix<n, n> const& M)
    {
        return *this = *this * M;
    }

    auto& operator [](size_t const& index)
    {
        if constexpr (m != 1 and n == 1) return (*(element)[index]);
        else if constexpr (m == 1 and n != 1) return ((*element)[index]);
        else if constexpr (m != 1 and n != 1) return ((element)[index]);
    }

    auto& operator [](size_t const& index) const
    {
        return (*const_cast<Matrix<m, n> *>(this))[index];
    }
};

template<size_t m, size_t n>
inline Matrix<n, m> const transpose(Matrix<m, n> const& M)
{
    if constexpr (m == 1 or n == 1)
    {
        return reinterpret_cast<Matrix<n, m> const&>(M);
    }
    else
    {
        Matrix<n, m> product = Matrix<n, m>();
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < m; ++j)
                product[i][j] = M[j][i];

        return product;
    }
}
