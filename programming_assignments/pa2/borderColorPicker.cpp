#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(unsigned int borderSize, HSLAPixel fillColor, PNG &img, double tolerance)
{
    this->borderSize = borderSize;
    this->fillColor = fillColor;
    this->img = img;
    this->tolerance = tolerance;
}

HSLAPixel borderColorPicker::operator()(point p)
{
  if ((int) p.x < (int) borderSize || (int) p.y < (int) borderSize || (int) img.width() - (int) borderSize <= (int) p.x || (int) img.height() - (int) borderSize <= (int) p.y) {
    return fillColor;
  }
  for (int y = (int) p.y - (int) borderSize; y <= (int) p.y + (int) borderSize; y++) {
    for (int x = (int) p.x - (int) borderSize; x <= (int) p.x + (int) borderSize; x++) {
      if ((x - (int) p.x) * (x - (int) p.x) + (y - (int) p.y) * (y - (int) p.y) <= (int) borderSize * (int) borderSize) {
        if (img.getPixel(x, y)->dist(p.c.color) > tolerance) {
          return fillColor;
        }
      }
    }
  }
  return *(img.getPixel(p.x, p.y));
}
