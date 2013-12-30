#ifndef MATRIX_H
#define MATRIX_H

#include "vecmath.h"
class Matrix {
public:
    Matrix() : dx(2),   dy(2)
    {
        initializeMatrix();

        for(int i = 0; i < dx; ++i)
        {
            for(int j = 0; j < dy; ++j)
            {
                p[i][j] = 0;
            }
        }
    }
    Matrix(const Matrix& m)
    {
        dx = m.dx; dy = m.dy;

        initializeMatrix();

        for(int i = 0; i < dx; ++i)
        {
            for(int j = 0; j < dy; ++j)
            {
                p[i][j] = m.p[i][j];
            }
        }
    }
    Matrix(int rows, int cols) : dx(rows), dy(cols)
    {
        initializeMatrix();

        for(int i = 0; i < dx; ++i)
        {
            for(int j = 0; j < dy; ++j)
            {
                p[i][j] = 0;
            }
        }
    }
    ~Matrix()
    {
        for(int i = 0; i < dx; ++i)
        {
            delete [] p[i];
        }

        delete [] p;
    }

    Matrix& operator=(const Matrix& a)
    {
        if(this->dx != a.dx || this->dy != a.dy)
        {
            return *this;
        }
        for(int i = 0; i < a.dx; ++i)
        {
            for(int j = 0; j < a.dy; ++j)
            {
                p[i][j] = a.p.[i][j];
            }
        }
        r0 = a.r0; r1 = a.r1; r2 = a.r2, r3 = a.r3;
        return *this;
    }

  //double operator[](int n) const { return ((double *) this)[n]; }

    Matrix& operator+=(const Matrix& a)
    {
        if(this->dx != a.dx || this->dy != a.dy)
        {
            return *this;
        }

        for(int i = 0; i < dx; ++i)
        {
            for(int j = 0; j < dy; ++j)
            {
                p[i][j] += a.p[i][j];
            }
        }

        retrn *this;
    }

    Matrix& operator-=(const Matrix& a)
    {
        if(this->dx != a.dx || this->dy != a.dy)
        {
            return *this;
        }

        for(int i = 0; i < dx; ++i)
        {
            for(int j = 0; j < dy; ++j)
            {
                p[i][j] -= a.p[i][j];
            }
        }

        retrn *this;
    }

    Matrix& operator*=(double s)
    {
        for(int i = 0; i < dx; ++i)
        {
            for(int j = 0; j < dy; ++j)
            {
                p[i][j] *= s;
            }
        }

        retrn *this;
    }

    Matrix operator-() const
    {
        Matrix temp(*this);
        for(int i = 0; i < dx; ++i)
        {
            for(int j = 0; j < dy; ++j)
            {
                temp[i][j] = -p[i][j];
            }
        }

        return temp;
    }

    Matrix operator+() const
    {
        return *this;
    }

    double &operator()(int x, int y) const
    {
        return p[x][y];
    }

    void initializeArray()
    {
        p = new double*[dx];
        for(int i = 0; i < dx; ++i)
        {
            p[i] = new double[dy];
        }
    }

    double det() const
    {
        if(dx != dy)
            return DM;

        if(dx == 1)
            return p[0][0];

        if(dx == 2)
            return (p[0][0] * p[1][1] - p[1][0] * p[0][1]);

        else
            return 0.0;
    }

    void transpose() const
    {
        if(dx != dy) /*ERROR*/
            return;

        double temp;

        for (int i = 1; i < n; ++i)
        {
           for (int j = 0; j < i; j++)
           {
              temp = p[i][j];
              p[i][j] = p[j][i];
              p[j][i] = temp;
           }
        }
    }

    int dx, dy;
    double **p;
};

// **** Matrix operators ****

inline Matrix operator+(const Matrix& a, const Matrix& b)
{
    Matrix temp(a);
    return (temp += b);
}

inline Matrix operator-(const Matrix& a, const Matrix& b)
{
    Matrix temp(a);
    return (temp -= b);
}

inline Matrix operator*(double s, const Matrix& a)
{
    Matrix temp(a);
    return (temp *= s);
}

inline Matrix operator*(const Matrix& a, double s)
{
    Matrix temp(a);
    return (temp *= s);
}

// Matrix Multiplication
inline Matrix operator*(const Matrix& a, const Matrix& b)
{
    Matrix result(a.dx, b.dy);

    for(int i = 0; i < result.dx; ++i)
    {
        for(int j = 0; j < result.dy; ++j)
        {
            for(int k = 0; k < a.dy; ++k)
            {
                result[i][j] += a.p[i][k] * b.p[k][j];
            }
        }
    }

    return result;
}

// cross product
inline Matrix operator^(const Matrix& a, const Matrix& b)
{
  return Matrix(a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}

inline bool operator==(const Matrix& a, const Matrix& b)
{
    if(this->dx != a.dx || this->dy != a.dy)
        return false;

    for(int i = 0; i < dx; ++i)
    {
        for(int j = 0; j < dy; ++j)
        {
            if(p[i][j] != a.p[i][j])
                return false;
        }
    }

    return true;
}


inline bool operator!=(const Matrix& a, const Matrix& b)
{
    if(this->dx != a.dx || this->dy != a.dy)
        return true;

    for(int i = 0; i < dx; ++i)
    {
        for(int j = 0; j < dy; ++j)
        {
            if(p[i][j] != a.p[i][j])
                return true;
        }
    }

    return false;
}

inline Matrix operator/(const Matrix& a, double s)
{
  double is = 1 / s;
  Matrix temp(a);
  return (temp *= is);
}

inline ostream& operator<<(ostream& os, const Matrix& m)
{
    for(int i = 0; i < dx; ++i)
    {
        for(int j = 0; j < dy; ++j)
            os << m.p[i][j] << " ";
        os << endl;
    }
    return os;
}


#endif /* _VECMATH_H_ */
