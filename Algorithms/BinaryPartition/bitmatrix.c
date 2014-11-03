/*
    This file is part of DDMTestbed, a free Data Distribution Management benchmark
    Copyright (C) 2013 Francesco Bedini

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "bitmatrix.h"

_ERR_CODE init_bit_matrix (bitmatrix * matrix_pointer,const uint64_t rows, const uint64_t columns, const InitialValue initial_value){
    uint64_t line_width, i;
    bitvector vec;
    *matrix_pointer = (bitmatrix ) malloc(rows * sizeof(bitvector));

    line_width = BIT_VEC_WIDTH(columns);

    if(initial_value == zero)
        vec = (bitvector)calloc(line_width * rows, sizeof(BITVECTOR_TYPE));
    else
        vec = (bitvector)malloc(line_width * rows * sizeof(BITVECTOR_TYPE));
    if(vec == NULL || *matrix_pointer == NULL)
    {
         return set_error(err_alloc, __FILE__, __FUNCTION__, __LINE__);
    }

    for (i = 0; i < rows; i++)
        (*matrix_pointer)[i] = &vec[i * line_width];
    if(initial_value == one)
    {
        set_whole_bit_mat(*matrix_pointer, rows, columns);
    }

	return err_none;
}

void set_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column){
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(column);
    BIT_SET(mat[row][bit_pos],DBIT(BIT_POS_IN_VEC(column,bit_pos)));
}
void reset_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column){
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(column);
    BIT_CLEAR(mat[row][bit_pos],DBIT(BIT_POS_IN_VEC(column,bit_pos)));
}

void negate_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column){
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(column);
	BIT_FLIP(mat[row][bit_pos], DBIT(BIT_POS_IN_VEC(column,bit_pos)));
}

void reset_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
    memset(*mat, UINT8_C(0x00), rows* BIT_VEC_WIDTH(columns) * sizeof(BITVECTOR_TYPE));
}

void set_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
    memset(*mat, UINT8_C(0xFF), rows* BIT_VEC_WIDTH(columns) * sizeof(BITVECTOR_TYPE));
}

void negate_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
	uint64_t i, j;
	for(i=0; i<rows; i++)
	{
	    for(j=0; j < BIT_VEC_WIDTH(columns); j++)
        {
           BIT_FLIP(mat[i][j], FULL_MASK);
        }
	}
}

bool get_bit(const bitmatrix mat, const uint64_t row,const uint64_t col)
{
    return BIT_DCHECK(mat[row][BIT_TO_POS(col)], col) != 0;
}

_ERR_CODE read_bit_matrix (bitmatrix * mat, const char * filename, uint64_t * rows, uint64_t * columns)
{
	FILE * fp;
    uint64_t element_number = 0;
	BITVECTOR_TYPE i;
	fp = fopen(filename, "rb");
	if(fp != NULL)
	{
		if(!fread(rows,sizeof(uint64_t), 1, fp))
			return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
		if(!fread(columns,sizeof(uint64_t), 1, fp))
			return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
		element_number = BIT_VEC_WIDTH(*columns);
		if (init_bit_matrix(mat, *rows, *columns, uninitialized) != err_none)
			return print_error_string();
		for(i=0; i<*rows; i++)
		{
			if(fread((*mat)[i],sizeof(BITVECTOR_TYPE), element_number, fp) != element_number)
			{
            printf("DURANTE RIGA %"PRIu64" ", i);
		    fflush(stdout);
				perror("fread");
				if(feof(fp))
				return set_error ( err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
			}
		}
		fclose(fp);
	}
	else
	{
		return set_error(err_file, __FILE__, __FUNCTION__, __LINE__);
	}
	return err_none;
}

uint64_t count_ones(const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
    uint64_t i, j;
	uint_fast8_t count;
	BITVECTOR_TYPE res;
	uint64_t subtotal;
	uint32_t full_columns = BIT_VEC_WIDTH(columns)-1;
	subtotal = 0;
	BITVECTOR_TYPE mask = FULL_MASK << (BIT_LENGTH - (columns % BIT_LENGTH));
	//Compute hamming distance for each row except from the last one, which may not be completely occupied by meaningful data.
	for(i=0; i< rows; i++)
	{
	    for(j=0; j<full_columns; j++)
        {
            //Xor between the two values
            res =  mat[i][j];
            //Compute hamming distance for this row
            for (count = 0; res; count++)
                res &= res-1;
		subtotal += count;
        }
        //compute last cell to the right, masking the extra bits
        res = (mat[i][j]) & mask;
        for (count = 0; res; count++)
            res &= res-1;
		subtotal += count;
	}
	return subtotal;
}

void print_bit_matrix(const bitmatrix mat, const char * filename, const bool override_file, const uint64_t rows, const uint64_t columns)
{
	FILE * fp;
	BITVECTOR_TYPE i;
	uint64_t element_numb = BIT_VEC_WIDTH(columns);
	char confirm='1';
	if(!override_file){
		fp = fopen(filename,"rb");
		if(fp != NULL)
		{
			fclose(fp);
			printf("File %s already exists, would you like to overwrite it? (y|n)\n", filename);
			do{
				confirm = getchar();
			}while(confirm!='y' && confirm !='n');
			if(confirm == 'n')
				return;
		}
	}
	if((fp = fopen(filename, "wb")) != NULL)
	{
	    //Print the number of rows
		fwrite(&rows, sizeof(uint64_t), 1, fp);
		//Print the number of columns
		fwrite(&columns, sizeof(uint64_t), 1, fp);

		//Prints vector as bytes
		for(i=0; i< rows; i++)
			if(fwrite(mat[i],sizeof(BITVECTOR_TYPE),element_numb, fp) != element_numb)
			{
				set_error(err_generic,__FILE__,__FUNCTION__,__LINE__);
			}
		fclose(fp);
	}
	return;
}

bool checkSet (const bitmatrix mat1, const bitmatrix mat2, const int rows, const int columns)
{
	uint64_t i, j;
	for(i=0; i < rows; i++)
	{
	    for(j=0; j<BIT_VEC_WIDTH(columns); j++)
		if((mat1[i][j] & mat2[i][j]) != mat1[i][j])
			return false;
	}
	return true;
}

int compareBitmatix(const bitmatrix mat1, const bitmatrix mat2, const uint64_t rows, const uint64_t columns)
{
	uint64_t i, j;
	int count;
	BITVECTOR_TYPE res;
	int subtotal;
	uint32_t full_columns = BIT_VEC_WIDTH(columns)-1;
	subtotal=0;
	BITVECTOR_TYPE mask = FULL_MASK << (BIT_LENGTH - (columns % BIT_LENGTH));
	//Compute hamming distance for each row except from the last one, which may not be completely occupied by meaningful data.
	for(i=0; i< rows; i++)
	{
	    for(j=0; j<full_columns; j++)
        {
            //Xor between the two values
            res =  mat1[i][j] ^ mat2[i][j];
            //Compute hamming distance for this row
            for (count = 0; res; count++)
                res &= res-1;
		subtotal += count;
        }
        //compute last cell to the right, masking the extra bits
        res = (mat1[i][j] ^ mat2[i][j]) & mask;
        for (count = 0; res; count++)
            res &= res-1;
		subtotal += count;
	}
	return subtotal;
}

void free_bit_matrix(bitmatrix * mat){
	free(*mat);
}

void print_readable_bitmatrix(const bitmatrix in, int iteration, int rows, int columns)
{
    uint64_t i, j, bit; //TODO tipo giusto?
	uint8_t element[BIT_LENGTH + 1];
	BITVECTOR_TYPE val;
	BITVECTOR_TYPE mask;
    printf("\n________BITMATRIX %d _________\n", iteration);
	uint64_t line_width;

	// number of elements on each line of the bit matrix
	line_width = BIT_VEC_WIDTH(columns);

	// escape the string
	element[BIT_LENGTH] = '\0';

	// for each line (update extent)
	for (i = 0; i < rows; i++)
	{
		// for each element in the line
		for (j = 0; j < line_width; j++)
		{
			mask = BITVEC_ELEM_MAX_BIT;
			val = in[i][j];

			// for each bit in the element
			for (bit = 0; bit < BIT_LENGTH; bit++)
			{
				// if the actual bit is TRUE, set the char to '1', else set it to '0'
				element[bit] = (val & mask ? '1' : '0');
				// shift right
				mask >>= 1;
			}

			// print the string
			printf("%s ", element);
		}
		// new line
		printf("\n");
	}
}
