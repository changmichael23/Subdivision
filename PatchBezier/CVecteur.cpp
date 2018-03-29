#include "CVecteur.h"

CVecteur::CVecteur(float x, float y, float z )
{
    m_dX = x;
    m_dY = y;
    m_dZ = z;
}

CVecteur::CVecteur(float x, float y, float z ,bool normalize)
{
    m_dX = x;
    m_dY = y;
    m_dZ = z;
    if(normalize)
    {
        this->normalizeData();
    }
}

CVecteur::~CVecteur()
{

}

// D�but accesseurs
float CVecteur::getX()
{
    return m_dX;
}

float CVecteur::getY()
{
    return m_dY;
}

float CVecteur::getZ()
{
    return m_dZ;
}

void CVecteur::setX(float x)
{
    m_dX=x;
}

void CVecteur::setY(float y)
{
    m_dY=y;
}

void CVecteur::setZ(float z)
{
    m_dZ=z;
}

CVecteur* CVecteur::AdditionVecteur(CVecteur* v1, CVecteur* v2)
{
    CVecteur* vecteurResultat = new CVecteur(v1->getX() + v2->getX(), v1->getY() + v2->getY(), v1->getZ() + v2->getZ());

    return vecteurResultat;
}

float CVecteur::DotProduct(CVecteur* v1, CVecteur* v2)
{
	return (v1->getX() * v2->getX()) + (v1->getY() * v2->getY()) + (v1->getX() * v2->getZ());
}

CVecteur* CVecteur::CrossProduct(CVecteur* v1, CVecteur* v2)
{
	float a = v1->getY() * v2->getZ() - v1->getZ() * v2->getY();
	float b = v1->getZ() * v2->getX() - v1->getX() * v2->getZ();
	float c = v1->getX() * v2->getY() - v1->getY() * v2->getX();

	return new CVecteur(a, b, c);
}

CVecteur* CVecteur::Scalar(CVecteur* v1, float f)
{
	return new CVecteur(v1->getX() * f, v1->getY() * f, v1->getZ() * f);
}

void CVecteur::normalizeData()
{
    float norme = this->getNorme();
    if(norme!=1)
    {
        m_dX/=norme;
        m_dY/=norme;
        m_dZ/=norme;
    }
}

float CVecteur::getNorme()
{
    return (sqrt(m_dX*m_dX+m_dY*m_dY+m_dZ*m_dZ));
}





