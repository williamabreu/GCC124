#include "clip-rectangle.h"
#include <iostream>
#include <cmath>

using namespace std;

ClipRectangle::ClipRectangle (double xmin, double xmax, double ymin, double ymax)
    : mXmin(xmin), mXmax(xmax), mYmin(ymin), mYmax(ymax), mInitialized(true)
{}

void ClipRectangle::Set(double xmin, double xmax, double ymin, double ymax)
{
    mXmin = xmin;
    mXmax = xmax;
    mYmin = ymin;
    mYmax = ymax;
    mInitialized = true;
}

unsigned int ClipRectangle::encode(double x, double y)
{
    unsigned int bin = 0b0000;

    if      (x > mXmax) bin |= 0b0010;
    else if (x < mXmin) bin |= 0b0001;
    if      (y > mYmax) bin |= 0b1000;
    else if (y < mYmin) bin |= 0b0100;

    // Para testar a região:
    // Testado e funcionando.
    // switch (bin)
    // {
    //     case 0b1001:
    //         cout << "1001" << endl;
    //         break;
        
    //     case 0b1000:
    //         cout << "1000" << endl;
    //         break;
        
    //     case 0b1010:
    //         cout << "1010" << endl;
    //         break;
        
    //     case 0b0001:
    //         cout << "0001" << endl;
    //         break;
        
    //     case 0b0010:
    //         cout << "0010" << endl;
    //         break;
        
    //     case 0b0101:
    //         cout << "0101" << endl;
    //         break;
        
    //     case 0b0100:
    //         cout << "0100" << endl;
    //         break;

    //     case 0b0110:
    //         cout << "0110" << endl;
    //         break;

    //     case 0b0000:
    //         cout << "0000" << endl;
    //         break;
    // }
    
    return bin;
}

// Retorna um ponteiro para uma linha recem alocada inteiramente dentro da area de recorte
// ou NULL se l for completamente descartada.
// A linha alocada aqui será desalocada na função CleanUp().
Line* ClipRectangle::Clip(const Line& l)
{
    // NOT IMPLEMENTED YET!
    // cout << *this << endl;
    double 
        x1 = l.mP0.mX, 
        x2 = l.mP1.mX, 
        y1 = l.mP0.mY, 
        y2 = l.mP1.mY;
    
    unsigned int encodeP1 = encode(x1, y1);
    unsigned int encodeP2 = encode(x2, y2);

    unsigned int bitwiseAnd = encodeP1 & encodeP2;

    if (encodeP1 != encodeP2 and bitwiseAnd == 0) 
    {
        // calcula inteseções, se for diferente de zero é pq está completamente fora do recorte.
    }

    // l.mP0.mX, l.mP1
    // mXmin
    // mXmax
    // mYmin
    // mYmax
    Point P0(x1, y1), P1(x2, y2);
    Line* clippedLine = new Line(P0, P1);
    return clippedLine;
}

void ClipRectangle::Read(const string& prompt)
{
    cout << prompt;
    cin >> mXmin >> mXmax >> mYmin >> mYmax;
    if (not mInitialized)
        mInitialized = true;
}

ostream& operator << (ostream& os, const ClipRectangle& r)
{
    return os << "(Clipping Rectangle: " << r.mXmin << ", " << r.mXmax << ", " 
              << r.mYmin << ", " << r.mYmax << ")";
}


