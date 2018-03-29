#ifndef CVECTEUR_H
#define CVECTEUR_H

#include <iostream>
#include <math.h>

class CVecteur
{
    public:
        CVecteur(float x, float y, float z );
        CVecteur(float x, float y, float z ,bool normalize);
        ~CVecteur();
        float getX();
        float getY();
        float getZ();
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        float getNorme();
        void normalizeData();
        static CVecteur* AdditionVecteur(CVecteur* v1, CVecteur* v2);
		static float DotProduct(CVecteur* v1, CVecteur* v2);
		static CVecteur* CrossProduct(CVecteur* v1, CVecteur* v2);
		static CVecteur* Scalar(CVecteur* v1, float f);
    protected:
    private:
        float m_dX;
        float m_dY;
        float m_dZ;
};

#endif // CVECTEUR_H
