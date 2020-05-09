#include <vector>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"

#include "block.h"

// set values in data attribute, based on a dimension x dimension square region
//   with upper-left corner at (left, upper) in the input PNG image
// The orientation of the pixels in the data vector must match the orientation
//   of the pixels in the PNG.
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Build(PNG& im, int upper, int left, int dimension){
  for (int y = upper; y < upper + dimension; y++){
    std::vector<HSLAPixel> row;
    for (int x = left; x < left + dimension; x++){
      row.push_back(*(im.getPixel(x, y)));
    }
    data.push_back(row);
 }
}

// write the pixel colour data fom data attribute into im,
//   with upper-left corner at (left, upper)
// PRE: upper and left (and upper + dimension, left + dimension) are valid
//        vector indices
void Block::Render(PNG& im, int upper, int left) const{
    int dimension = data.size();
    for (int y = upper; y < dimension + upper; y++){
      for (int x = left; x < dimension + left; x++){
        HSLAPixel * imPixel = im.getPixel(x, y);
        HSLAPixel dataPixel = data[y - upper][x - left];
        *imPixel = dataPixel;
      }
    }
}

// "Reverse" the Hue and Luminance channels for each pixel in the data attribute
//   to simulate a photo-negative effect.
// Refer to the HSLAPixel documentation to determine an appropriate transformation
//   for "reversing" hue and luminance.
void Block::Negative(){
  int length = data.size();
  for (int y = 0; y < length; y++){
    for (int x = 0; x < length; x++){
      HSLAPixel * dataPixel = &data[x][y];
      dataPixel->h = (((int) dataPixel->h) + 180) % 360;
      dataPixel->l = 1 - dataPixel->l;
    }
  }
}

// Return the horizontal (or vertical) size of the data block's image region
int Block::Dimension() const{
  return data.size();
}
