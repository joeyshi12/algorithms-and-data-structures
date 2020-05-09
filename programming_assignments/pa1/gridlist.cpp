// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"

// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const
{
  GridNode* curr = northwest;
  int dimension = curr->data.Dimension();
  PNG image(dimx * dimension, dimy * dimension);
  for (int y = 0; y < dimy; y++){
    for (int x = 0; x < dimx; x++){
      curr->data.Render(image, y * dimension, x * dimension);
      curr = curr->next;
    }
  }
  return image;
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata)
{
  GridNode * gridNode = new GridNode(bdata);
  if (this->IsEmpty()){
    northwest = gridNode;
    southeast = gridNode;
  } else{
    GridNode * temp = southeast;
    southeast = gridNode;
    southeast->prev = temp;
    temp->next = southeast;
  }
}


// if this list has an odd number of column blocks, then the right side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList& inner)
{
  if (dimy == inner.dimy){
    int moves = 0;
    GridNode* temp;
    GridNode* currInner = inner.northwest;

    for (GridNode* curr = northwest; curr != NULL; curr = curr->next){
      if (moves == dimx / 2 - 1){
        temp = curr->next;
        curr->next = currInner;
        currInner->prev = curr;
        curr = temp;
        moves++;

        for (int i = 0; i < inner.dimx - 1; i++){
          currInner = currInner->next;
        }
        temp = currInner->next;

        curr->prev = currInner;
        currInner->next = curr;
        for (int i = moves; i < dimx-1; i++){
          curr = curr->next;
        }
        currInner = temp;
        moves = 0;
        if (currInner == NULL){
          dimx += inner.dimx;
          inner.northwest = NULL;
          inner.southeast = NULL;
          return;
        }
        continue;
      }
      moves++;
    }
 }
}

// inner list must have the same horizontal resolution, horizontal block dimension, and block size
// if this list has an odd number of row blocks, then the bottom side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList& inner)
{
  if (dimx == inner.dimx){

    // Accumulators
    int moves = 0;

    for (GridNode* curr = northwest; curr != NULL; curr = curr->next){
      if (moves == dimx * (dimy / 2) - 1){
        GridNode* temp = curr->next;
        curr->next = inner.northwest;
        inner.northwest->prev = curr;

        curr = temp;
        inner.southeast->next = curr;
        curr->prev = inner.southeast;

        dimy += inner.dimy;
        inner.northwest = NULL;
        inner.southeast = NULL;
        return;
      }
      moves++;
    }
  }
}


// PRE:  this list and otherlist have the same pixel dimensions, block dimensions, and block size
// POST: this list and otherlist are checkered with each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList& otherlist)
{
  if (dimx == otherlist.dimx) {
    GridNode* currOther = otherlist.northwest->next;
    //GridNode* temp = NULL;
    bool switcharoo = true;
    if (northwest != NULL){
      if (dimx % 2 == 0){
        int count = 1;
        for (GridNode* curr = northwest->next; curr != NULL; curr = curr->next){
          if (count % dimx == 0){
            switcharoo = !switcharoo;
          }
          if (switcharoo){
            if(curr->prev) {
              curr->prev->next = currOther;
            }
            if(currOther->prev) {
              currOther->prev->next = curr;
            }
            if(curr->next) {
              curr->next->prev= currOther;
            }
            if(currOther->next) {
              currOther->next->prev= curr;
            }
            GridNode* temp;
            temp = curr->prev;
            curr->prev = currOther->prev;
            currOther->prev = temp;
            temp = curr->next;
            curr->next= currOther->next;
            currOther->next= temp;
          }
          switcharoo = !switcharoo;
          currOther = currOther->next;
          count++;
          if (curr == NULL || currOther == NULL) {
            break;
          }
        }
      } else {
        for (GridNode* curr = northwest->next; curr != NULL; curr = curr->next){
          if (switcharoo){
            if(curr->prev) {
              curr->prev->next = currOther;
            }
            if(currOther->prev) {
              currOther->prev->next = curr;
            }
            if(curr->next) {
              curr->next->prev= currOther;
            }
            if(currOther->next) {
              currOther->next->prev= curr;
            }
            GridNode* temp;
            temp = curr->prev;
            curr->prev = currOther->prev;
            currOther->prev = temp;
            temp = curr->next;
            curr->next= currOther->next;
            currOther->next= temp;
          }
          switcharoo = !switcharoo;
          currOther = currOther->next;
          if (curr == NULL || currOther == NULL) {
            break;
          }
        }
      }
    }
  }
}


// POST: this list has the negative effect applied selectively to GridNodes to form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect applied).
// Ensure that the checkering looks correct for both odd and even horizontal block dimensions
void GridList::CheckerN()
{
  bool makeNegative = true;
  if (northwest != NULL){
    if (dimx % 2 == 0){
      int count = 1;
      for (GridNode* curr = northwest->next; curr != NULL; curr = curr->next){
        if (count % dimx == 0){
          makeNegative = !makeNegative;
        }
        if (makeNegative){
          curr->data.Negative();
        }
        makeNegative = !makeNegative;
        count++;
      }
    } else{
      for (GridNode* curr = northwest->next; curr != NULL; curr = curr->next){
        if (makeNegative){
          curr->data.Negative();
        }
        makeNegative = !makeNegative;
      }
    }
  }
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  if (!IsEmpty()){
    GridNode* prev = northwest;
    for (GridNode* curr = northwest->next; curr != NULL; curr = curr->next){
      delete prev;
      prev = curr;
    }
    delete prev;
    dimx = 0;
    dimy = 0;
  }
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{
  if (otherlist.northwest != NULL){
    GridNode* gridNode = new GridNode();
    gridNode->data = otherlist.northwest->data;
    northwest = gridNode;
    GridNode* curr = northwest;
    for (GridNode* currOther = otherlist.northwest->next; currOther != NULL; currOther = currOther->next){
      // Making node copy
      GridNode* gridNode = new GridNode();
      gridNode->data = currOther->data;

      // Putting copy in this list
      curr->next = gridNode;
      curr = curr->next;
    }
    dimx = otherlist.dimx;
    dimy = otherlist.dimy;
  }
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//
