/*
 *     uarray2.c
 *     by Jia Wen Goh (jgoh01) and Winston Tan (wtan02), 09/17/2017
 *     COMP40: HW2 Part A
 *
 *     UArray2 is a 2D representation of a UArray that stores values in
 *     rows & columns.
 */

#include <stdio.h>
#include <stdlib.h>
#include <uarray.h>
#include <uarray2.h>
#include <assert.h>

/*
 * UArray2_T type that stores relevant information (i.e. dimensions, size
 * and contents)
 */
#define T UArray2_T
struct T {
        int      width;
        int      height;
        UArray_T array;
        int      size;
};

/*----------------------------------------------------------------*
 |                  Constructor/Destructor                        |
 *----------------------------------------------------------------*/
/*
 * [Name]:              UArray2_new
 * [Purpose]:           Creates UArray2 from given dimensions
 * [Parameters]:        3 int (dimension & byte-size of contents)
 * [Returns]:           T (created UArray2)
 */
T UArray2_new(int width, int height, int size)
{
        T uarray2 = malloc(20);
        assert(uarray2 != NULL);

        uarray2->width  = width;
        uarray2->height = height;
        uarray2->size   = size;
        uarray2->array  = UArray_new(width, sizeof(UArray_T));

         UArray_T *temp;
        for (int col = 0; col < width; col++) {
                temp = (UArray_T*)UArray_at(uarray2->array, col);
                *temp = UArray_new(height, size);
        }

        return uarray2;
}

/*
 * [Name]:              UArray2_free
 * [Purpose]:           Frees allocated memory of UArray2
 * [Parameters]:        1 T* (pointer to the UArray2 to be freed)
 * [Returns]:           void
 */
void UArray2_free(T* uarray2)
{
        for (int col = 0; col < (*uarray2)->width; col++) {
                UArray_free((UArray_T*)(UArray_at((*uarray2)->array, col)));
        }

        UArray_free(&(*uarray2)->array);
        free(*uarray2);
}

/*----------------------------------------------------------------*
 |                          Properties                            |
 *----------------------------------------------------------------*/
/*
 * [Name]:              UArray2_width
 * [Purpose]:           Returns the width of the given UArray2
 * [Parameters]:        1 T (the width of this UArray2 will be returned)
 * [Returns]:           int (width)
 */
int UArray2_width(T uarray2)
{
        return uarray2->width;
}

/*
 * [Name]:              UArray2_height
 * [Purpose]:           Returns the height of the given UArray2
 * [Parameters]:        1 T (the height of this UArray2 will be returned)
 * [Returns]:           int (height)
 */
int UArray2_height(T uarray2)
{
        return uarray2->height;
}

/*
 * [Name]:              UArray2_size
 * [Purpose]:           Returns the byte-size of the stored contents of the
 *                      given UArray2
 * [Parameters]:        1 T (the byte-size of this UArray2 will be returned)
 * [Returns]:           int (byte-size of contents)
 */
int UArray2_size(T uarray2)
{
        return uarray2->size;
}

/*
 * [Name]:              UArray2_at
 * [Purpose]:           Returns a ptr to the element at the given col/row of
 *                      the given UArray2
 * [Parameters]:        1 T (referenced UArray2), 2 int (col/row index)
 * [Returns]:           int (byte-size of contents)
 */
void *UArray2_at(T uarray2, int col, int row)
{
        UArray_T *temp = UArray_at(uarray2->array, col);
        temp = UArray_at(*temp, row);
        return temp;
}

/*----------------------------------------------------------------*
 |                          Mapping                               |
 *----------------------------------------------------------------*/
/*
 * [Name]:              UArray2_map_row_major
 * [Purpose]:           Calls the given apply function on each element in the
 *                      UArray2; the function will map row by row
 * [Parameters]:        1 T (referenced UArray2), apply function pointer,
 *                      1 void* (cl)
 * [Returns]:           void
 */
void UArray2_map_row_major(T uarray2, void apply(int w_index, int h_index,
                           T uarray2, void* index_p, void* cl), void* cl)
{
        for (int row = 0; row < uarray2->height; row++) {
                for (int col = 0; col < uarray2->width; col++) {
                        apply(col, row, uarray2, UArray2_at(uarray2, col, row),
                              cl);
                }
        }
}

/*
 * [Name]:              UArray2_map_col_major
 * [Purpose]:           Calls the given apply function on each element in the
 *                      UArray2; the function will map column by column
 * [Parameters]:        1 T (referenced UArray2), apply function pointer,
 *                      1 void* (cl)
 * [Returns]:           void
 */
void UArray2_map_col_major(T uarray2, void apply(int w_index, int h_index,
                           T uarray2, void* index_p, void* cl), void* cl)
{
        for (int col = 0; col < uarray2->width; col++) {
                for (int row = 0; row < uarray2->height; row++) {
                        apply(col, row, uarray2, UArray2_at(uarray2, col, row),
                              cl);
                }
        }
}