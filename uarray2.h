/*
 *     uarray2.h
 *     by Jia Wen Goh (jgoh01) and Winston Tan (wtan02), 09/17/2017
 *     COMP40: HW2 Part A
 *
 *     UArray2 is a 2D representation of a UArray that stores values in
 *     rows & columns.
 */

#ifndef UARRAY2_H_INCLUDED
#define UARRAY2_H_INCLUDED

#include <stdio.h>

#define T UArray2_T
typedef struct T *T;

/*
 *               uarray2.h
 *
 *   The unboxed 2D array (UArray2) is a 2D representation of a UArray
 *   that can store data values in rows & columns.
 *
 *   ---= Functions =---
 *      [Constructor/Destructor]
 *      - UArray2_new  - Creates UArray2 based on given dimensions
 *      - UArray2_free - Frees allocated memory of UArray2
 *
 *      [Properties]
 *      - UArray2_width  - Returns the width of the given UArray2
 *      - UArray2_height - Returns the height of the given UArray2
 *      - UArray2_size   - Returns the size of the stored contents
 *                         of the given UArray2
 *      - *UArray2_at    - Returns a ptr to the element at the given row/col
 *                         of the given UArray2
 *
 *      [Mapping]
 *      - UArray2_map_row_major
 *      - UArray2_map_col_major
 *                     - These functions calls the given apply function for
 *                       each element in the UArray2. The function maps column
 *                       by column for UArray2_map_row_major, and row by row
 *                       for UArray2_map_col_major.
 *
 *   Errors:
 *     A Hanson exception must be raised if memory cannot be allocated for
 *     UArray2.
 */

extern T    UArray2_new   (int width, int height, int size);
extern void UArray2_free  (T *uarray2);

extern int  UArray2_width (T uarray2);
extern int  UArray2_height(T uarray2);
extern int  UArray2_size  (T uarray2);
extern void *UArray2_at   (T uarray2, int row, int col);

extern void UArray2_map_row_major(T uarray2, void apply(int w_index,
                                  int h_index, T uarray2, void* index_p,
                                  void* cl), void* cl);
extern void UArray2_map_col_major(T uarray2, void apply(int w_index,
                                  int h_index, T uarray2, void* index_p,
                                  void* cl), void* cl);

#undef T
#endif