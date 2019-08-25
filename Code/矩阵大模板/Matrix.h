#pragma once
#include <iostream>
#include <cstring>
using namespace std;
template <typename T>
class Matrix
{
#define uint unsigned int
    template <typename Ty>
    friend ostream &operator<<(ostream &os, const Matrix<Ty> &Mat);
    template <typename Ty>
    friend istream &operator>>(istream &is, const Matrix<Ty> &Mat);

  private:
    T *element;
    uint _row, _column, _size;
    void Build(const uint Row, const uint Column)
    {
        _row = Row, _column = Column;
        element = new T[_size = Row * Column];
        memset(element,0,sizeof(T)*_size);
    }

  public:
    Matrix();
    Matrix(const uint Row, const uint Column) { Build(Row, Column); }
    Matrix(const uint, const uint, T *const);
    Matrix(const Matrix &);
    // ~Matrix() { delete[] element; }      //太费时间了
    uint row() const { return _row; }
    uint column() const { return _column; }
    uint size() const { return _size; }
    Matrix GetTransposition() const;
    Matrix QuickPowAndMod(unsigned long long, const uint) const;
    Matrix QuickPowAndModWithQuickMul(unsigned long long, const uint) const;
    Matrix &operator=(const Matrix &);
    Matrix operator+(const Matrix &) const;
    Matrix operator-(const Matrix &) const;
    Matrix operator*(const uint) const;
    Matrix operator*(const Matrix &)const;
    Matrix operator%(const uint) const;
    Matrix operator^(unsigned long long) const;
    Matrix &operator+=(const Matrix &Mat) { return *this = *this + Mat; }
    Matrix &operator-=(const Matrix &Mat) { return *this = *this - Mat; }
    Matrix &operator*=(const uint Num) { return *this = *this * Num; }
    Matrix &operator*=(const Matrix &Mat) { return *this = *this * Mat; }
    Matrix &operator%=(const uint Mod) { return *this = *this % Mod; }
    Matrix &operator^=(unsigned long long Index) { return *this = *this ^ Index; }
    T &operator()(const uint Row, const uint Column) const { return element[Row * _column + Column]; }
};
template <typename T>
Matrix<T>::Matrix()
{
    Build(0, 0);
    element = NULL;
}
template <typename T>
Matrix<T>::Matrix(const uint Row, const uint Column, T *const Add)
{
    Build(Row, Column);
    memcpy(element, Add, sizeof(T) * _size);
}
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &MO)
{
    Build(MO._row, MO._column);
    memcpy(element, MO.element, _size * sizeof(T));
}
template <typename T>
inline Matrix<T> Matrix<T>::GetTransposition() const
{
    Matrix<T> Mat_ret(_column, _row);
    for (uint i = 0; i < _column; i++)
        for (uint j = 0; j < _row; j++)
            Mat_ret(i, j) = operator()(j, i);
    return Mat_ret;
}
template <typename T>
inline Matrix<T> Matrix<T>::QuickPowAndMod(unsigned long long Index, const uint Mod) const
{
    Matrix<T> mat = *this, ans = *this;
    struct MultiAndMod
    {
        inline void operator()(Matrix &a, const Matrix &b, const uint Mod)
        {
            if (a._column != b._row)
                throw "Multiplication Error";
            Matrix<T> tmp(a._row, b._column);
            for (uint i = 0; i < a._row; i++)
                for (uint k = 0; k < b._column; k++)
                    for (uint j = 0; j < a._column; j++)
                        tmp(i, j) = (tmp(i, j) % Mod + a(i, k) % Mod * b(k, j) % Mod) % Mod;
            a = tmp;
        }
    } mam;
    for (Index--; Index; mam(mat, mat, Mod), Index >>= 1)
        if (Index & 1)
            mam(ans, mat, Mod);
    return ans % Mod;
}
template <typename T>
inline Matrix<T> Matrix<T>::QuickPowAndModWithQuickMul(unsigned long long Index, const uint Mod) const
{
    Matrix<T> mat = *this, ans = *this;
    struct MultiAndMod
    {
        struct QuickMul
        {
            inline long long operator()(long long x, long long y, long long mod)
            {
                long long ans = 0;
                while (y != 0)
                {
                    if (y & 1 == 1)
                        ans += x, ans %= mod;
                    x = x + x, x %= mod;
                    y >>= 1;
                }
                return ans;
            }
        } qm;
        inline void operator()(Matrix &a, const Matrix &b, const uint Mod)
        {
            if (a._column != b._row)
                throw "Multiplication Error";
            Matrix<T> tmp(a._row, b._column);
            for (uint i = 0; i < a._row; i++)
                for (uint k = 0; k < b._column; k++)
                    for (uint j = 0; j < a._column; j++)
                        tmp(i, j) = (tmp(i, j) % Mod + qm(a(i, k), b(k, j), Mod)) % Mod;
            a = tmp;
        }
    } mam;
    for (Index--; Index; mam(mat, mat, Mod), Index >>= 1)
        if (Index & 1)
            mam(ans, mat, Mod);
    return ans % Mod;
}
template <typename T>
inline Matrix<T> &Matrix<T>::operator=(const Matrix<T> &Mat)
{
    if (_row != Mat._row || _column != Mat._column)
    {
        delete[] element;
        _row = Mat._row, _column = Mat._column;
        element = new T[_size = _row * _column];
    }
    memcpy(element, Mat.element, sizeof(T) * _row * _column);
    return *this;
}
template <typename T>
inline Matrix<T> Matrix<T>::operator+(const Matrix<T> &Mat) const
{
    if (_row != Mat._row || _column != Mat._column)
        throw "Addition Error";
    Matrix<T> Mat_ret(_row, _column);
    for (uint i = 0; i < _size; i++)
        Mat_ret.element[i] = element[i] + Mat.element[i];
    return Mat_ret;
}
template <typename T>
inline Matrix<T> Matrix<T>::operator-(const Matrix<T> &Mat) const
{
    if (_row != Mat._row || _column != Mat._column)
        throw "Subtraction Error";
    Matrix<T> Mat_ret(_row, _column);
    for (uint i = 0; i < _size; i++)
        Mat_ret.element[i] = element[i] - Mat.element[i];
    return Mat_ret;
}
template <typename T>
inline Matrix<T> Matrix<T>::operator*(const uint Num) const
{
    Matrix<T> Mat_ret(_row, _column);
    for (uint i = 0; i < _size; i++)
        Mat_ret.element[i] = element[i] * Num;
    return Mat_ret;
}
template <typename T>
inline Matrix<T> Matrix<T>::operator*(const Matrix<T> &Mat) const
{
    if (_column != Mat._row)
        throw "Multiplication Error";
    Matrix<T> Mat_ret(_row, Mat._column);
    memset(Mat_ret.element, 0, sizeof(T) * Mat._size);
    for (uint i = 0; i < _row; i++)
        for (uint j = 0; j < Mat._column; j++)
            for (uint k = 0; k < _column; k++)
                Mat_ret(i, j) += operator()(i, k) * Mat(k, j);
    return Mat_ret;
}
template <typename T>
inline Matrix<T> Matrix<T>::operator%(const uint Mod) const
{
    Matrix<T> Mat_ret(*this);
    for (uint i = 0; i < _row * _column; i++)
        Mat_ret.element[i] %= Mod;
    return Mat_ret;
}
template <typename T>
inline Matrix<T> Matrix<T>::operator^(unsigned long long Index) const
{
    Matrix<T> mat = *this, ans = *this;
    for (Index--; Index; mat *= mat, Index >>= 1)
        if (Index & 1)
            ans *= mat;
    return ans;
}
template <typename T>
ostream &operator<<(ostream &os, const Matrix<T> &Mat)
{
    for (uint i = 0; i < Mat._row; i++)
    {
        for (uint j = 0; j < Mat._column; j++)
            os << Mat(i, j) << ' ';
        os << endl;
    }
    return os;
}
template <typename T>
istream &operator>>(istream &is, const Matrix<T> &Mat)
{
    for (uint i = 0; i < Mat._row; i++)
        for (uint j = 0; j < Mat._column; j++)
            is >> Mat(i, j);
    return is;
}
#undef uint
//======================================