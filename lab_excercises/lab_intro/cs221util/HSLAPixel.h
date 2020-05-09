#pragma once

class HSLAPixel
{
public:
    double h;
    double s;
    double l;
    double a;

    HSLAPixel();

    HSLAPixel(double h, double s, double l);

    HSLAPixel(double h, double s, double l, double a);
};