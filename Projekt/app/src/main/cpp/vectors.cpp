
#include "vectors.h"
#include <algorithm>
Vector4 Vector4::operator+(Vector4 &a)
{
    Vector4 temp;
    temp.x = this->x + a.x;
    temp.x = this->y + a.y;
    temp.x = this->z + a.z;
    temp.w = 1;
    return temp;
}
Vector4 Vector4::operator/(int &a)
{
    Vector4 temp;
    temp.x = this->x / a;
    temp.x = this->y / a;
    temp.x = this->z / a;
    temp.w = 1;
    return temp;
}
Vector4& Vector4::operator=(const Vector4 &other)
{
    if (this != &other)
    {
        std::copy(other.val,other.val + 4, val);
    }
    return *this;
}

Vector4& Vector4::operator=(const Matrix<float,4,1> &other)
{

        this->val[0] = other(0);
        this->val[1] = other(1);
        this->val[2] = other(2);
        this->val[3] = other(3);

    return *this;
}
Vector4::Vector4(float x,float y,float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1;
}
Vector4::Vector4()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 1;
}
Matrice4 Matrice4::operator*(Matrice4 &b) {
    Matrice4 tmp;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {
            tmp.cell[i][j] = this->cell[i][0] * b.cell[0][j]
                             + this->cell[i][1] * b.cell[1][j]
                             + this->cell[i][2] * b.cell[2][j]
                             + this->cell[i][3] * b.cell[3][j];
        }
    }
    return tmp;
}
Vector4 Matrice4::operator*(Vector4 &b) {
    Vector4 tmp;
    tmp.w = 0;
    for(int i =0; i<4;i++){
        for(int j =0; j<4;j++) {
            tmp.val[i] += this->cell[i][j] * b.val[j];
        }
    }
    return tmp;
}
Matrice4 Matrice4::transpose() {
    Matrice4 tmp;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
        {
            tmp.cell[i][j] = this->cell[j][i];
        }
    }
    return tmp;

}
