/*
 *     sudoku.c
 *     by Jia Wen Goh (jgoh01) and Winston Tan (wtan02), 09/19/2017
 *
 *     Sudoku takes in a single portable graymap file, and verifies if it is a 
 *     solved sudoku puzzle. A solved sudoku puzzle has the following 
 *     properties:
 *      - Denominator == 9
 *      - No pixel == 0
 *      - In each ROW, no 2 pixels have same intensity
 *      - In each COL, no 2 pixels have same intensity
 *      - in each SQR, no 2 pixels have same intensity
 *     The programme will return 1 if it is not a solved sudoku puzzle, and 0 
 *     if it is.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uarray2.h>
#include <pnmrdr.h>

/* SUCCESS & FAILURE constants */
const int SUCCESS = 0;
const int FAILURE = 1;

/* --- File Processing Functions --- */
int process_file         (FILE* fp);
int verify_specifications(Pnmrdr_mapdata pix_data);
UArray2_T store_sudoku   (FILE* fp);
int store_sudoku_pixels  (UArray2_T sudoku_uarray, Pnmrdr_T sudoku_file);

/* --- Sudoku-Check Functions --- */
int check_sudoku         (UArray2_T sudoku, int index);
int check_row            (UArray2_T sudoku, int row);
int check_col            (UArray2_T sudoku, int col);
int check_square         (UArray2_T sudoku, int col, int row);
int check_pixel_intensity(int sudoku_pixel);
int check_count          (int *int_freq);

/*
 * [Name]:              main
 * [Purpose]:           main call; processes the file given on the cmnd line/
 *                      stdin (i.e. check if it is a valid sudoku graymap)
 * [Parameters]:        1 int (argc), 1 char* (argv[])
 * [Returns]:           0/SUCCESS if valid sudoku, else 1/FAILURE if not
 */
int main(int argc, char** argv) {
        assert(argc < 3);
        if (argc != 1) {
                FILE* fp;
                fp = fopen(argv[1], "r");
                assert(fp != NULL);

                int exit_code = process_file(fp);

                fclose(fp);
                exit(exit_code);        
        } else {
                int exit_code = process_file(stdin);
                exit(exit_code);
        }
}

/*----------------------------------------------------------------*
 |                  Processing Functions                          |
 *----------------------------------------------------------------*/
/*
 * [Name]:              main
 * [Purpose]:           main call; processes the file given on the cmnd line/
 *                      stdin (i.e. check if it is a valid sudoku graymap)
 * [Parameters]:        1 int (argc), 1 char* (argv[])
 * [Returns]:           0/SUCCESS if valid sudoku, else 1/FAILURE if not
 */
int process_file(FILE *fp) 
{
        UArray2_T sudoku = store_sudoku(fp);
        if (sudoku == NULL) {
                return FAILURE;
        }

        for (int i = 0; i < 9; i++) {
                if (check_sudoku(sudoku, i)) {
                        UArray2_free(&sudoku);
                        return FAILURE;
                }
        }
        
        UArray2_free(&sudoku);              
        return SUCCESS;
}

/*
 * [Name]:              store_sudoku
 * [Purpose]:           Given the FILE* (from argv/stdin), store the numbers in
 *                      a UArray2; give a CRE if:
 *                      - not a proper graymap
 *                      - not 9x9
 *                      Additionally, exit if denominator != 9
 * [Parameters]:        1 int (argc), 1 char* (argv[])
 * [Returns]:           0/SUCCESS if valid sudoku, else 1/FAILURE if not
 */
 UArray2_T store_sudoku(FILE *fp)
 {
        Pnmrdr_T sudoku_file    = Pnmrdr_new(fp);
        Pnmrdr_mapdata pix_data = Pnmrdr_data(sudoku_file);
 
        if (verify_specifications(pix_data) == 1) {
                Pnmrdr_free(&sudoku_file);                              
                return NULL;
        }                
 
        UArray2_T sudoku_uarray = UArray2_new(9, 9, 4);
        int pix_invalid = store_sudoku_pixels(sudoku_uarray, sudoku_file);
 
        Pnmrdr_free(&sudoku_file);
        if (pix_invalid) {
                UArray2_free(&sudoku_uarray);
                return NULL;
        }
        return sudoku_uarray;
 }

/*
 * [Name]:              verify_specifications
 * [Purpose]:           Verifies if the given file is a valid graymap. Then
 *                      checks if a valid sudoku puzzle (i.e. 9x9), or if 
 *                      denominator != 9 
 * [Parameters]:        1 Pnmrdr_mapdata (file to be read)
 * [Returns]:           0/SUCCESS if valid sudoku, else 1/FAILURE if not
 */
int verify_specifications(Pnmrdr_mapdata pix_data)
{
        assert(pix_data.type == Pnmrdr_gray);   

        if ((pix_data.width != 9) || (pix_data.height != 9) || 
            (pix_data.denominator != 9)) {
                return FAILURE;
        }

        return SUCCESS;
}

 /*
 * [Name]:              store_sudoku_pixels
 * [Purpose]:           store pixels in UArray2 and checks if pixels are valid
 * [Parameters]:        UArray2_T (array to store)
 *                      Pnmrdr_T (file to take pixels from)
 * [Returns]:           0/SUCCESS if valid sudoku, else 1/FAILURE if not
 */
int store_sudoku_pixels(UArray2_T sudoku_uarray, Pnmrdr_T sudoku_file)
{
        int pix_invalid = 0;
        int *sudoku_pixel = NULL;
        for (int row = 0; row < 9; row++) {
                for (int col = 0; col < 9; col++) {
                        sudoku_pixel  = UArray2_at(sudoku_uarray, col, row);
                        *sudoku_pixel = Pnmrdr_get(sudoku_file);
                        if (check_pixel_intensity(*sudoku_pixel)) {
                                pix_invalid = 1;
                        }
                }
        }

        return pix_invalid;
}

 /*----------------------------------------------------------------*
 |                  Sudoku-Check Functions                         |
 *----------------------------------------------------------------*/
/*
 * [Name]:              check_sudoku
 * [Purpose]:           Checks if the given sudoku UArray2 is a solved sudoku 
 *                      puzzle (i.e. rows, cols and squares have no repeated
 *                      pixels)
 * [Parameters]:        1 UArray_T (sudoku), 1 int (x-th index of row/col/sqr)
 * [Returns]:           0/SUCCESS if solved sudoku, else 1/FAILURE if not
 */
int check_sudoku(UArray2_T sudoku, int index)
{
        if (check_row(sudoku, index) || check_col(sudoku, index) 
            || check_square(sudoku, index % 3 * 3, index / 3 * 3)) {
            return FAILURE;
        }
        return SUCCESS;
}

/*
 * [Name]:              check_pixel_intensity
 * [Purpose]:           Verifies if given pixel has a value <1 or >9. 
 * [Parameters]:        1 int (sudoku pixel)
 * [Returns]:           0/SUCCESS if within bounds, else 1/FAILURE if not
 */
int check_pixel_intensity(int sudoku_pixel)
{
        if ((sudoku_pixel > 9) || (sudoku_pixel < 1)) {
                return FAILURE; 
        }

        return SUCCESS; 
}

/*
 * [Name]:              check_row
 * [Purpose]:           Verifies if the given sudoku UArray2 has valid rows
 *                      (i.e. no repeated pixels)
 * [Parameters]:        1 UArray_T (sudoku), 1 int (x-th row index)
 * [Returns]:           0/SUCCESS if solved sudoku, else 1/FAILURE if not
 *                      (based on check_count)
 */
int check_row(UArray2_T sudoku, int row)
{
        int *temp = NULL;
        int int_freq[9] = {0};

        for (int col = 0; col < 9; col++) {
                temp = UArray2_at(sudoku, col, row);
                int_freq[*temp - 1]++;
        }

        return check_count(int_freq);
}

/*
 * [Name]:              check_col
 * [Purpose]:           Verifies if the given sudoku UArray2 has valid cols
 *                      (i.e. no repeated pixels)
 * [Parameters]:        1 UArray_T (sudoku), 1 int (x-th col index)
 * [Returns]:           0/SUCCESS if solved sudoku, else 1/FAILURE if not
 *                      (based on check_count)
 */
int check_col(UArray2_T sudoku, int col)
{
        int *temp = NULL;
        int int_freq[9] = {0};

        for (int row = 0; row < 9; row++) {
                temp = UArray2_at(sudoku, col, row);
                int_freq[*temp - 1]++;
        }

        return check_count(int_freq);
}

/*
 * [Name]:              check_square
 * [Purpose]:           Verifies if the given sudoku UArray2 has valid squares
 *                      (i.e. no repeated pixels)
 * [Parameters]:        1 UArray_T (sudoku), 2 int (col/row index of xth-sqr)
 * [Returns]:           0/SUCCESS if solved sudoku, else 1/FAILURE if not
 *                      (based on check_count)
 */
int check_square(UArray2_T sudoku, int col, int row)
{
        int *temp = NULL;
        int int_freq[9] = {0};

        for (int i = 0; i < 9; i++) {
                temp = UArray2_at(sudoku, col + i / 3, row + i % 3);
                int_freq[*temp - 1]++;
        }

        return check_count(int_freq);
}

/*
 * [Name]:              check_count
 * [Purpose]:           Checks if there are any repeated integers in the given 
 *                      int array; frequency array is used by row, col and 
 *                      square checks.
 * [Parameters]:        1 int* (array; contains the frequencies of integers 1-9
 *                      in the row/col/square)
 * [Returns]:           0/SUCCESS if no repeats, else 1/FAILURE if there are
 */
int check_count(int *int_freq)
{
        for (int i = 0; i < 9; i++) {
                if (int_freq[i] != 1) {
                        return FAILURE;
                }
        }

        return SUCCESS;
}