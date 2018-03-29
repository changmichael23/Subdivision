#include "SquareMatrice.h"
#include "CQuaternion.h"
#include <cmath>

using namespace std;

SquareMatrice::SquareMatrice(unsigned int normeMatrice, bool isIdentity): m_norme((int)normeMatrice)
{
    allocMatrice();
    this->reinit(isIdentity);

}

void SquareMatrice::reinit(bool isIdentity)
{
    for(int i=0;i<m_norme;i++)
    {
        for(int j=0;j<m_norme;j++)
        {
            if(isIdentity && i==j)
                m_matrice[i*m_norme+j]=1;
            else
                m_matrice[i*m_norme+j]=0;
        }
    }
}


SquareMatrice::SquareMatrice(float x, float y, float z, Transformation transformation)
{
    m_norme=4;
    allocMatrice();
    this->reinit(x,y,z,transformation);
}

void SquareMatrice::reinit(float x, float y, float z, Transformation transformation)
{
    if(m_norme!=4)
    {
        cout<<"Error size matrice while reinnit";
        return;
    }

    if(transformation == SquareMatrice::Translation)
    {
        for(int i=0;i<m_norme;i++)
        {
            for(int j=0;j<m_norme;j++)
            {
                if(i==j)
                    m_matrice[i*m_norme+j]=1;
                else
                    m_matrice[i*m_norme+j]=0;
            }
        }
        m_matrice[12]=x;
        m_matrice[13]=y;
        m_matrice[14]=z;
    }
    else if(transformation == SquareMatrice::Scale)
    {
        for(int i=0;i<m_norme;i++)
        {
            for(int j=0;j<m_norme;j++)
            {
                m_matrice[i*m_norme+j]=0;
            }
        }
        m_matrice[0*m_norme+0]=x;
        m_matrice[1*m_norme+1]=y;
        m_matrice[2*m_norme+2]=z;
        m_matrice[3*m_norme+3]=1;
    }
}

SquareMatrice::SquareMatrice(float angle,float ratioScreen, float near, float far)
{
    m_norme=4;
    allocMatrice();
    this->reinit(angle,ratioScreen,near,far);
}

void SquareMatrice::reinit(float angle,float ratioScreen, float near, float far)
{
    if(m_norme!=4)
    {
        cout<<"Error size matrice while reinnit";
        return;
    }

    for(int i=0;i<m_norme;i++)
    {
        for(int j=0;j<m_norme;j++)
        {
            m_matrice[i*m_norme+j]=0;
        }
    }

    float rad = ( angle * 3.1416 ) / 180 ;
    float range = tan(rad / 2) * near;
    float left = -range * ratioScreen;
    float right = range * ratioScreen;
    float bottom = -range;
    float top = range;

    m_matrice[0*m_norme+0] = (2 * near) / (right - left);
    m_matrice[1*m_norme+1] = (2 * near) / (top - bottom);
    m_matrice[2*m_norme+2] = - (far + near) / (far - near);
    m_matrice[2*m_norme+3] = - 1;
    m_matrice[3*m_norme+2] = - (2 * far * near) / (far - near);
}

SquareMatrice::SquareMatrice(float angle,CVecteur axe)
{
    m_norme=4;
    allocMatrice();
    this->reinit(angle,axe);
}

void SquareMatrice::reinit(float angle,CVecteur axe)
{
    int x=axe.getX(),y=axe.getY(),z=axe.getZ();
    if(m_norme!=4)
    {
        cout<<"Error size matrice while reinnit";
        return;
    }
//    cout<<"reinit rotation"<<endl;
    angle = ( angle * 3.1416 ) / 180 ;
//    cout<<"in fonction";
//    if(axe == SquareMatrice::AxeX)
//        cout<<"test1";
//    else if(axe == SquareMatrice::AxeY)
//        cout<<"test2";
//    else if(axe == SquareMatrice::AxeZ)
//        cout<<"test3";

    m_matrice[0]=x*x*(1-cos(angle))+cos(angle);
    m_matrice[1]=x*y*(1-cos(angle))-z*sin(angle);
    m_matrice[2]=x*z*(1-cos(angle))+y*sin(angle);
    m_matrice[3]=0;

    m_matrice[4]=x*y*(1-cos(angle))+z*sin(angle);
    m_matrice[5]=y*y*(1-cos(angle))+cos(angle);
    m_matrice[6]=y*z*(1-cos(angle))-x*sin(angle);
    m_matrice[7]=0;

    m_matrice[8]=x*z*(1-cos(angle))-y*sin(angle);
    m_matrice[9]=y*z*(1-cos(angle))+x*sin(angle);
    m_matrice[10]=z*z*(1-cos(angle))+cos(angle);
    m_matrice[11]=0;

    m_matrice[12]=0;
    m_matrice[13]=0;
    m_matrice[14]=0;
    m_matrice[15]=1;
}

void SquareMatrice::allocMatrice()
{
    m_matrice = (float*) malloc(sizeof(float)*m_norme*m_norme);
}

SquareMatrice::~SquareMatrice()
{
    for(int i=0;i<m_norme;i++)
    {
        free(m_matrice);
    }
}


float* SquareMatrice::get_ptr()
{
    return m_matrice;
}

int const SquareMatrice::getNorme()
{
    return m_norme;
}

void SquareMatrice::mult(SquareMatrice const *matrice2)
{
    if(m_norme != matrice2->m_norme)
    {
        cout<<"Multiplication entre matrice de norme différentes"<<endl;
        return;
    }

    float* matrix = (float*) malloc(sizeof(float)*m_norme*m_norme);

    for(int i=0;i<m_norme;i++)
    {
        for(int j=0;j<m_norme;j++)
        {
            float total = 0;
            for(int k=0;k<m_norme;k++)
            {
                total+=m_matrice[i*m_norme+k]*matrice2->m_matrice[k*m_norme+j];
            }
            matrix[i*m_norme+j]=total;
        }
    }
//    free(m_matrice);
    m_matrice=matrix;
}

void SquareMatrice::mult(const float* ma1, const float* m2,int norme,float* result)
{
    for(int i=0;i<norme;i++)
    {
        for(int j=0;j<norme;j++)
        {
            float total = 0;
            for(int k=0;k<norme;k++)
            {
                total+=ma1[i*norme+k]*m2[k*norme+j];
            }
            result[i*norme+j]=total;
        }
    }
}

int SquareMatrice::fillMatrice(float *tab, int sizeTab)
{
    if(sizeTab!=m_norme*m_norme)
    {
        cout<<"Erreur de taille entre tableau donné et matrice, remplissage impossible";
        return -1;
    }
    for(int i=0;i<m_norme;i++)
        for(int j=0;j<m_norme;j++)
            m_matrice[i*m_norme+j]=tab[i*m_norme+j];
    return 0;
}

void SquareMatrice::print()
{
    SquareMatrice::print(m_matrice, m_norme);
}

void SquareMatrice::print(const float* matrix, int norme)
{
    for(int i=0;i<norme;i++)
    {
        cout<<"|";
        for(int j=0;j<norme;j++)
        {
            cout<<" "<<matrix[i*norme+j];
        }
        cout<<" |"<<endl;
    }
}


SquareMatrice::SquareMatrice(CVecteur *pos, float angleX, float angleY)
{
    m_norme=4;
    allocMatrice();
    this->reinit(pos,angleX,angleY);

}

void SquareMatrice::reinit(CVecteur *pos, float angleX, float angleY)
{
    if(m_norme!=4)
    {
        cout<<"Error size matrice while reinnit";
        return;
    }

    this->reinit(true);

    CQuaternion *quatRotation = new CQuaternion(-angleX,new CVecteur(0,1,0));

//    SquareMatrice *matricePos = new SquareMatrice(-pos->getX(),-pos->getY(),-pos->getZ(),SquareMatrice::Translation);
//    SquareMatrice *matriceRotX = quatRotation->QuaternionToMatrice();
//    angleX =  ( angleX * 3.1416 ) / 180;
//    quatRotation->reinit(-angleY,new CVecteur(0,cos(angleX),sin(angleY)));
//    SquareMatrice *matriceRotY = quatRotation->QuaternionToMatrice();

    SquareMatrice *matricePos = new SquareMatrice(-pos->getX(),-pos->getY(),-pos->getZ(),SquareMatrice::Translation);
    SquareMatrice *matriceRotX = new SquareMatrice(-angleX,CVecteur(1,0,0));
    angleX =  ( angleX * 3.1416 ) / 180;
    SquareMatrice *matriceRotY = new SquareMatrice(-angleY,CVecteur(0,cos(angleX),sin(angleY)));

    this->mult(matriceRotX);
    this->mult(matriceRotY);
    this->mult(matricePos);

    delete matricePos;
    delete matriceRotX;
    delete matriceRotY;
}

CQuaternion* SquareMatrice::MatriceToQuaternion()
{
    /*
        qw= √(1 + m00 + m11 + m22) /2
        qx = (m21 - m12)/( 4 *qw)
        qy = (m02 - m20)/( 4 *qw)
        qz = (m10 - m01)/( 4 *qw)
    */

    if(m_norme!=4)
    {
        cout<<"Error size matrice cant convert to quaternion";
        return NULL;
    }


    CQuaternion* qResultat = new CQuaternion(0, new CVecteur(1,1,0));

    qResultat->SetW(sqrt(1 + m_matrice[0] + m_matrice[5] + m_matrice[10])/2);
    qResultat->SetX((m_matrice[9] - m_matrice[6]) / (4*qResultat->GetW()));
    qResultat->SetX((m_matrice[2] - m_matrice[8]) / (4*qResultat->GetW()));
    qResultat->SetX((m_matrice[4] - m_matrice[1]) / (4*qResultat->GetW()));

    return qResultat;
}


