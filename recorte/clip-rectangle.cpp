#include "clip-rectangle.h"
#include <iostream>
#include <cmath>

#define INSIDE 0b0000
#define LEFT   0b0001
#define RIGHT  0b0010
#define BOTTOM 0b0100
#define TOP    0b1000

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

    if      (x > mXmax) bin |= RIGHT;
    else if (x < mXmin) bin |= LEFT;
    if      (y > mYmax) bin |= TOP;
    else if (y < mYmin) bin |= BOTTOM;

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
        x0 = l.mP0.mX, 
        x1 = l.mP1.mX, 
        y0 = l.mP0.mY, 
        y1 = l.mP1.mY;
    
    unsigned int outcode0 = encode(x0, y0);
    unsigned int outcode1 = encode(x1, y1);

	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			accept = true;
			break;
		} else if (outcode0 & outcode1) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			break;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			unsigned int outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y1 - y0) / (x1 - x0)
			//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
			//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// outcode bit being tested guarantees the denominator is non-zero
			if (outcodeOut & TOP) {           // point is above the clip window
				x = x0 + (x1 - x0) * (mYmax - y0) / (y1 - y0);
				y = mYmax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip window
				x = x0 + (x1 - x0) * (mYmin - y0) / (y1 - y0);
				y = mYmin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip window
				y = y0 + (y1 - y0) * (mXmax - x0) / (x1 - x0);
				x = mXmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip window
				y = y0 + (y1 - y0) * (mXmin - x0) / (x1 - x0);
				x = mXmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = encode(x0, y0);
			} else {
				x1 = x;
				y1 = y;
				outcode1 = encode(x1, y1);
			}
		}
	}
	if (accept) {
		// Following functions are left for implementation by user based on
		// their platform (OpenGL/graphics.h etc.)
        Point P0(x0, y0), P1(x1, y1);
        Line* clippedLine = new Line(P0, P1);
        return clippedLine;
	}
    else {
        return NULL;
    }
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


