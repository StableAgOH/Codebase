#include <iostream>
#include <cstring>
using namespace std;
template <int row, int col>
struct Matrix
{
    int r, c;
    long long ele[row][col];
    Matrix() { throw "Please set row and column!"; }
    Matrix(int a, int b) : r(a), c(b) { memset(ele, 0, sizeof(ele)); }
    long long &operator()(int a, int b) { return ele[a][b]; }
    Matrix &operator+=(Matrix oth) { return *this = *this + oth; }
    Matrix &operator-=(Matrix oth) { return *this = *this - oth; }
    Matrix &operator*=(Matrix oth) { return *this = *this * oth; }
};
template <int m, int n>
Matrix<m, n> operator+(Matrix<m, n> m1, Matrix<m, n> m2)
{
    Matrix<m, n> ret;
    for (int i = 0; i < m1.r; i++)
        for (int j = 0; j < m1.c; j++)
            ret(i, j) = m1(i, j) + m2(i, j);
    return ret;
}
template <int m, int n>
Matrix<m, n> operator-(Matrix<m, n> m1, Matrix<m, n> m2)
{
    Matrix<m, n> ret;
    for (int i = 0; i < m1.r; i++)
        for (int j = 0; j < m1.c; j++)
            ret(i, j) = m1(i, j) + m2(i, j);
    return ret;
}
template <int m, int n, int p>
Matrix<m, p> operator*(Matrix<m, n> m1, Matrix<n, p> m2) //普通乘法
{
    Matrix<m, p> ret(m1.r, m2.c);
    for (int i = 0; i < m1.r; i++)
        for (int k = 0; k < m1.c; k++)
            for (int j = 0; j < m2.c; j++)
                ret(i, j) += m1(i, k) * m2(k, j);
    return ret;
}
template <int m, int n>
Matrix<m, n> operator^(Matrix<m, n> mat, long long k) //快速幂
{
    Matrix<m, n> ans = mat;
    for (k--; k; mat *= mat, k >>= 1)
        if (k & 1)
            ans *= mat;
    return ans;
}
template <int m, int n, int p>
Matrix<m, p> mulmod(Matrix<m, n> m1, Matrix<n, p> m2, int mod) //乘加取模
{
    Matrix<m, p> ret(m1.r, m2.c);
    for (int i = 0; i < m1.r; i++)
        for (int k = 0; k < m1.c; k++)
            for (int j = 0; j < m2.c; j++)
                ret(i, j) = (ret(i, j) + m1(i, k) * m2(k, j) % mod) % mod;
    return ret;
}
template <int m, int n>
Matrix<m, n> qpowmod(Matrix<m, n> mat, long long k, int mod) //快速幂取模
{
    Matrix<m, n> ans = mat;
    for (k--; k; mat = mulmod(mat, mat, mod), k >>= 1)
        if (k & 1)
            ans = mulmod(ans, mat, mod);
    return ans;
}
template <int m, int n>
ostream &operator<<(ostream &os, Matrix<m, n> &mat)
{
    for (int i = 0; i < mat.r; i++)
    {
        for (int j = 0; j < mat.c; j++)
            os << mat(i, j) << ' ';
        os << endl;
    }
    return os;
}
template <int m, int n>
istream &operator>>(istream &is, Matrix<m, n> &mat)
{
    for (int i = 0; i < mat.r; i++)
        for (int j = 0; j < mat.c; j++)
            is >> mat(i, j);
    return is;
}
const int maxn = 105;
int main()
{
    int n;
    long long k;
    const int p = 1e9 + 7;
    cin >> n >> k;
    Matrix<maxn, maxn> mat(n, n);
    cin >> mat;
    auto tmp = qpowmod(mat, k, p);
    cout << tmp;
    return 0;
}