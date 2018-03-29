#include "CQuaternion.h"

CQuaternion::CQuaternion(float angleDegree, CVecteur* pVecteur)
{
    this->reinit(angleDegree,pVecteur);
}

void CQuaternion::reinit(float angleDegree, CVecteur* pVecteur)
{
    float degRadian;
    degRadian =  angleDegree * 3.14159265359 / 180.0;

    m_dX = cos(degRadian/2);
    m_dY = pVecteur->getX() * sin(degRadian/2);
    m_dZ = pVecteur->getY() * sin(degRadian/2);
    m_dW = pVecteur->getZ() * sin(degRadian/2);
}


CQuaternion::~CQuaternion()
{

}

// D�but accesseurs

float CQuaternion::GetX()
{
    return m_dX;
}

float CQuaternion::GetY()
{
    return m_dY;
}

float CQuaternion::GetZ()
{
    return m_dZ;
}

float CQuaternion::GetW()
{
    return m_dW;
}

void CQuaternion::SetX(float i)
{
    m_dX = i;
}

void CQuaternion::SetY(float i)
{
    m_dY = i;
}

void CQuaternion::SetZ(float i)
{
    m_dZ = i;
}

void CQuaternion::SetW(float i)
{
    m_dW = i;
}

// Fin accesseurs

SquareMatrice* CQuaternion::QuaternionToMatrice()
{

    /*
        1 - 2*qy2 - 2*qz2	2*qx*qy - 2*qz*qw	2*qx*qz + 2*qy*qw
        2*qx*qy + 2*qz*qw	1 - 2*qx2 - 2*qz2	2*qy*qz - 2*qx*qw
        2*qx*qz - 2*qy*qw	2*qy*qz + 2*qx*qw	1 - 2*qx2 - 2*qy2
    */

    SquareMatrice* matriceResultat = new SquareMatrice(4,true);
    float *matrice = matriceResultat->get_ptr();
    matrice[0]=(1 - 2 * (this->m_dY * this->m_dY)) - (2 * (this->m_dZ * this->m_dZ));
    matrice[1]= (2 * this->m_dY * this->m_dX) - (2 * this->m_dZ * this->m_dW);
    matrice[2]= (2 * this->m_dZ * this->m_dX) + (2 * this->m_dY * this->m_dW);

    matrice[4]= (2 * this->m_dY * this->m_dX) + (2 * this->m_dZ * this->m_dW);
    matrice[5]= (1 - 2 * (this->m_dX * this->m_dX)) - (2 * (this->m_dZ * this->m_dZ));
    matrice[6]= (2 * this->m_dY * this->m_dZ) - (2 * this->m_dX * this->m_dW);

    matrice[8]= (2 * this->m_dZ * this->m_dX) - (2 * this->m_dY * this->m_dW);
    matrice[9]= (2 * this->m_dY * this->m_dZ) + (2 * this->m_dX * this->m_dW);
    matrice[10]= (1 - 2 * (this->m_dX * this->m_dX)) - (2 * (this->m_dY * this->m_dY));

    return matriceResultat;
    //return new SquareMatrice(4,true);
}



CQuaternion* CQuaternion::QuaternionMultiplication(CQuaternion* q1, CQuaternion* q2)
{
    // Q*Q�=aa�-bb�-cc�-dd�+(ab�+ba�+cd�-dc�)i+(ac�+ca�+db�-bd�)j+(ad�+da�+bc�-cb�)k

    if(q1 == NULL || q2 == NULL)
        return NULL;

    CQuaternion* qResultat = new CQuaternion(0, new CVecteur(0,0,0));

    qResultat->SetX((q1->GetX() * q2->GetX()) - (q1->GetY() * q2->GetY()) - (q1->GetZ() * q2->GetZ()) - (q1->GetW() * q2->GetW()));
    qResultat->SetY((q1->GetX() * q2->GetY()) + (q1->GetY() * q2->GetX()) + (q1->GetZ() * q2->GetW()) - (q1->GetW() * q2->GetZ()));
    qResultat->SetZ((q1->GetX() * q2->GetZ()) + (q1->GetZ() * q2->GetZ()) + (q1->GetW() * q2->GetY()) - (q1->GetY() * q2->GetW()));
    qResultat->SetW((q1->GetX() * q2->GetW()) + (q1->GetW() * q2->GetX()) + (q1->GetY() * q2->GetZ()) - (q1->GetZ() * q2->GetY()));

    return qResultat;
}

CQuaternion* CQuaternion::ConjugueQuaternion()
{
    CQuaternion* pConjugue = new CQuaternion(m_dX, new CVecteur(-m_dY, -m_dZ, -m_dW));

    return pConjugue;
}

float CQuaternion::NormeQuaternion()
{
    return sqrt( m_dW * m_dW + m_dX * m_dX + m_dY * m_dY + m_dZ * m_dZ);
}

CQuaternion* CQuaternion::AdditionQuaternion(CQuaternion* q1, CQuaternion* q2)
{
    if(q1 == NULL || q2 == NULL)
        return NULL;

    CVecteur* pV1 = new CVecteur(q1->GetY(), q1->GetZ(), q1->GetW());
    CVecteur* pV2 = new CVecteur(q2->GetY(), q2->GetZ(), q2->GetW());

    CVecteur* pV3 = pV1->AdditionVecteur(pV1, pV2);

    CQuaternion* qResultat = new CQuaternion(q1->GetX() + q2->GetX(), new CVecteur(pV3->getX(), pV3->getY(), pV3->getZ()));

    return qResultat;
}
