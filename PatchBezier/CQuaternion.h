#ifndef CQUATERNION_H
#define CQUATERNION_H

#include "SquareMatrice.h"

class CQuaternion
{
    public:
        CQuaternion(float angleDegree, CVecteur* pVecteur);
        virtual ~CQuaternion();

        CQuaternion* QuaternionMultiplication(CQuaternion* q1, CQuaternion* q2);
        CQuaternion* ConjugueQuaternion();
        CQuaternion* AdditionQuaternion(CQuaternion* q1, CQuaternion* q2);
        SquareMatrice* QuaternionToMatrice();
        float NormeQuaternion();
        void reinit(float angleDegree, CVecteur* pVecteur);

        float GetX();
        float GetY();
        float GetZ();
        float GetW();

        void SetX(float i);
        void SetY(float i);
        void SetZ(float i);
        void SetW(float i);

    protected:
    private:
        float m_dX;
        float m_dY;
        float m_dZ;
        float m_dW;
};

#endif // CQUATERNION_H