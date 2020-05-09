#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade)
{
    fadeFactor = fade;
}

HSLAPixel fadeColorPicker::operator()(point p)
{
    HSLAPixel ret = p.c.color;
    int distSq = pow(((int) p.x) - ((int) p.c.x), 2) + pow(((int) p.y) - ((int) p.c.y), 2);
    ret.h = ret.h;
    ret.s = ret.s;
    ret.l = ret.l * pow(fadeFactor, sqrt(distSq));
    ret.a = ret.a;
    return ret;
}
