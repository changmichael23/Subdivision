#ifndef DEF_SQUAREMATRICE
#define DEF_SQUAREMATRICE

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "CVecteur.h"

class CQuaternion;

class SquareMatrice
{
public:

    enum Transformation {
    Translation,
     Scale
    };

    enum Axe {
//    AxeX,
//    AxeY,
//    AxeZ,
    AxeCustom};
    SquareMatrice(unsigned int normeMatrice, bool isIdentity);
    SquareMatrice(CVecteur pos, float angleX, float angleY);
    SquareMatrice( float x, float y, float z, Transformation transformation);
    SquareMatrice(float angle,float ratioScreen, float near, float far);
    SquareMatrice(float angle,CVecteur axe);
    SquareMatrice(CVecteur *pos, float angleX, float angleY);
    ~SquareMatrice();
    float* get_ptr();
    int const getNorme();
    void mult(SquareMatrice const *matrice2);
    int fillMatrice(float *tab, int sizeTab);
    void print();
    void reinit(bool isIdentity);
    void reinit(float angle,float ratioScreen, float near, float far);
    void reinit(float x, float y, float z, Transformation transformation);
    void reinit(float angle,CVecteur axe);
    void reinit(CVecteur *pos, float angleX, float angleY);
    static void print(const float* matrix, int norme);
    static void mult(const float* ma1, const float* m2,int norme,float* result);
    CQuaternion* MatriceToQuaternion();
private:
    SquareMatrice::Transformation m_type;
    float *m_matrice;
    int m_norme;
    void allocMatrice();

};
#endif
