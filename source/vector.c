/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                                 vector.c                                 **
 **--------------------------------------------------------------------------**
 **                        First version: 30/04/2008                         **
 ******************************************************************************/

/******************************************************************************
 * OpenScop: Structures and formats for polyhedral tools to talk together     *
 ******************************************************************************
 *    ,___,,_,__,,__,,__,,__,,_,__,,_,__,,__,,___,_,__,,_,__,                 *
 *    /   / /  //  //  //  // /   / /  //  //   / /  // /  /|,_,              *
 *   /   / /  //  //  //  // /   / /  //  //   / /  // /  / / /\              *
 *  |~~~|~|~~~|~~~|~~~|~~~|~|~~~|~|~~~|~~~|~~~|~|~~~|~|~~~|/_/  \             *
 *  | G |C| P | = | L | P |=| = |C| = | = | = |=| = |=| C |\  \ /\            *
 *  | R |l| o | = | e | l |=| = |a| = | = | = |=| = |=| L | \# \ /\           *
 *  | A |a| l | = | t | u |=| = |n| = | = | = |=| = |=| o | |\# \  \          *
 *  | P |n| l | = | s | t |=| = |d| = | = | = | |   |=| o | | \# \  \         *
 *  | H | | y |   | e | o | | = |l|   |   | = | |   | | G | |  \  \  \        *
 *  | I | |   |   | e |   | |   | |   |   |   | |   | |   | |   \  \  \       *
 *  | T | |   |   |   |   | |   | |   |   |   | |   | |   | |    \  \  \      *
 *  | E | |   |   |   |   | |   | |   |   |   | |   | |   | |     \  \  \     *
 *  | * |*| * | * | * | * |*| * |*| * | * | * |*| * |*| * | /      \* \  \    *
 *  | O |p| e | n | S | c |o| p |-| L | i | b |r| a |r| y |/        \  \ /    *
 *  '---'-'---'---'---'---'-'---'-'---'---'---'-'---'-'---'          '--'     *
 *                                                                            *
 * Copyright (C) 2008 University Paris-Sud 11 and INRIA                       *
 *                                                                            *
 * (3-clause BSD license)                                                     *
 * Redistribution and use in source  and binary forms, with or without        *
 * modification, are permitted provided that the following conditions         *
 * are met:                                                                   *
 *                                                                            *
 * 1. Redistributions of source code must retain the above copyright notice,  *
 *    this list of conditions and the following disclaimer.                   *
 * 2. Redistributions in binary form must reproduce the above copyright       *
 *    notice, this list of conditions and the following disclaimer in the     *
 *    documentation and/or other materials provided with the distribution.    *
 * 3. The name of the author may not be used to endorse or promote products   *
 *    derived from this software without specific prior written permission.   *
 *                                                                            *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR       *
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES  *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.    *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,           *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT   *
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY      *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT        *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF   *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          *
 *                                                                            *
 * OpenScop Library, a library to manipulate OpenScop formats and data        *
 * structures. Written by:                                                    *
 * Cedric Bastoul     <Cedric.Bastoul@u-psud.fr> and                          *
 * Louis-Noel Pouchet <Louis-Noel.pouchet@inria.fr>                           *
 *                                                                            *
 ******************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <osl/int.h>
#include <osl/macros.h>
#include <osl/util.h>
#include <osl/vector.h>

/******************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/

/**
 * osl_vector_idump function:
 * Displays a osl_vector structure (*vector) into a file (file, possibly
 * stdout) in a way that trends to be understandable without falling in a deep
 * depression or, for the lucky ones, getting a headache... It includes an
 * indentation level (level) in order to work with others idump functions.
 * \param[in] file   File where informations are printed.
 * \param[in] vector The vector whose information have to be printed.
 * \param[in] level  Number of spaces before printing, for each line.
 */
void osl_vector_idump(FILE* const file, const osl_vector* const vector,
                      const int level) {
  if (vector) {
    // Go to the right level.
    for (int j = 0; j < level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+-- osl_vector (");
    osl_int_dump_precision(file, vector->precision);
    fprintf(file, ")\n");

    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");
    fprintf(file, "%d\n", vector->size);

    // Display the vector.
    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");

    fprintf(file, "[ ");

    for (int j = 0; j < vector->size; j++) {
      osl_int_print(file, vector->precision, vector->v[j]);
      fprintf(file, " ");
    }

    fprintf(file, "]\n");
  } else {
    // Go to the right level.
    for (int j = 0; j < level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+-- NULL vector\n");
  }

  // The last line.
  for (int j = 0; j <= level; j++)
    fprintf(file, "|\t");
  fprintf(file, "\n");
}

/**
 * osl_vector_dump function:
 * This function prints the content of a osl_vector structure
 * (*vector) into a file (file, possibly stdout).
 * \param[in] file   File where informations are printed.
 * \param[in] vector The vector whose information have to be printed.
 */
void osl_vector_dump(FILE* const file, const osl_vector* const vector) {
  osl_vector_idump(file, vector, 0);
}

/******************************************************************************
 *                   Memory allocation/deallocation function                  *
 ******************************************************************************/

/**
 * osl_vector_pmalloc function:
 * (precision malloc) this function allocates the memory space for an
 * osl_vector structure and sets its fields with default values. Then
 * it returns a pointer to the allocated space.
 * \param[in] precision The precision of the vector entries.
 * \param[in] size      The number of entries of the vector to allocate.
 * \return A pointer to the newly allocated osl_vector structure.
 */
osl_vector* osl_vector_pmalloc(const int precision, const int size) {
  osl_vector* vector;

  OSL_malloc(vector, osl_vector*, sizeof(osl_vector));
  vector->size = size;
  vector->precision = precision;
  if (!size) {
    vector->v = NULL;
  } else {
    OSL_malloc(vector->v, osl_int*, (size_t)size * sizeof(osl_int));
    for (int i = 0; i < size; i++)
      osl_int_init_set_si(precision, &vector->v[i], 0);
  }
  return vector;
}

/**
 * osl_vector_malloc function:
 * This function allocates the memory space for a osl_vector structure
 * and sets its fields with default values. Then it returns a pointer to the
 * allocated space. The precision of the vector elements corresponds to the
 * precision environment variable or to the highest available precision if it
 * is not defined.
 * \param[in] size      The number of entries of the vector to allocate.
 * \return A pointer to the newly allocated osl_vector structure.
 */
osl_vector* osl_vector_malloc(const int size) {
  const int precision = osl_util_get_precision();
  return osl_vector_pmalloc(precision, size);
}

/**
 * osl_vector_free function:
 * This function frees the allocated memory for a osl_vector structure.
 * \param[in] vector The pointer to the vector we want to free.
 */
void osl_vector_free(osl_vector* const vector) {
  if (vector) {
    if (vector->v) {
      for (int i = 0; i < vector->size; i++)
        osl_int_clear(vector->precision, &vector->v[i]);

      free(vector->v);
    }
    free(vector);
  }
}

/******************************************************************************
 *                           Processing functions                             *
 ******************************************************************************/

/**
 * osl_vector_add_scalar function:
 * This function adds a scalar to the vector representation of an affine
 * expression (this means we add the scalar only to the very last entry of the
 * vector). It returns a new vector resulting from this addition.
 * \param[in] vector The basis vector.
 * \param[in] scalar The scalar to add to the vector.
 * \return A pointer to a new vector, copy of the basis one plus the scalar.
 */
osl_vector* osl_vector_add_scalar(const osl_vector* const vector,
                                  const int scalar) {
  if (!vector || (vector->size < 2))
    OSL_error("incompatible vector for addition");

  const int precision = vector->precision;
  const int last = vector->size - 1;

  osl_vector* const result = osl_vector_pmalloc(precision, vector->size);
  for (int i = 0; i < vector->size; i++)
    osl_int_assign(precision, &result->v[i], vector->v[i]);
  osl_int_add_si(precision, &result->v[last], vector->v[last], scalar);

  return result;
}

/**
 * osl_vector_add function:
 * This function achieves the addition of two vectors and returns the
 * result as a new vector (the addition means the ith entry of the new vector
 * is equal to the ith entry of vector v1 plus the ith entry of vector v2).
 * \param v1 The first vector for the addition.
 * \param v2 The second vector for the addition.
 * \return A pointer to a new vector, corresponding to v1 + v2.
 */
osl_vector* osl_vector_add(const osl_vector* const v1,
                           const osl_vector* const v2) {
  if (!v1 || !v2 || (v1->size != v2->size) || (v1->precision != v2->precision))
    OSL_error("incompatible vectors for addition");

  osl_vector* const v3 = osl_vector_pmalloc(v1->precision, v1->size);
  for (int i = 0; i < v1->size; i++)
    osl_int_add(v1->precision, &v3->v[i], v1->v[i], v2->v[i]);

  return v3;
}

/**
 * osl_vector_sub function:
 * This function achieves the subtraction of two vectors and returns the
 * result as a new vector (the addition means the ith entry of the new vector
 * is equal to the ith entry of vector v1 minus the ith entry of vector v2).
 * \param v1 The first vector for the subtraction.
 * \param v2 The second vector for the subtraction (result is v1-v2).
 * \return A pointer to a new vector, corresponding to v1 - v2.
 */
osl_vector* osl_vector_sub(const osl_vector* const v1,
                           const osl_vector* const v2) {
  if (!v1 || !v2 || (v1->size != v2->size) || (v1->precision != v2->precision))
    OSL_error("incompatible vectors for subtraction");

  osl_vector* const v3 = osl_vector_pmalloc(v1->precision, v1->size);
  for (int i = 0; i < v1->size; i++)
    osl_int_sub(v1->precision, &v3->v[i], v1->v[i], v2->v[i]);

  return v3;
}

/**
 * osl_vector_tag_inequality function:
 * This function tags a vector representation of a contraint as being an
 * inequality >=0. This means in the PolyLib format, to set to 1 the very
 * first entry of the vector. It modifies directly the vector provided as
 * an argument.
 * \param vector The vector to be tagged.
 */
void osl_vector_tag_inequality(osl_vector* const vector) {
  if (!vector || (vector->size < 1))
    OSL_error("vector cannot be tagged");
  osl_int_set_si(vector->precision, &vector->v[0], 1);
}

/**
 * osl_vector_tag_equality function:
 * This function tags a vector representation of a contraint as being an
 * equality ==0. This means in the PolyLib format, to set to 0 the very
 * first entry of the vector. It modifies directly the vector provided as
 * an argument.
 * \param vector The vector to be tagged.
 */
void osl_vector_tag_equality(osl_vector* const vector) {
  if (!vector || (vector->size < 1))
    OSL_error("vector cannot be tagged");
  osl_int_set_si(vector->precision, &vector->v[0], 0);
}

/**
 * osl_vector_equal function:
 * this function returns true if the two vectors are the same, false
 * otherwise.
 * \param v1 The first vector.
 * \param v2 The second vector.
 * \return 1 if v1 and v2 are the same (content-wise), 0 otherwise.
 */
bool osl_vector_equal(const osl_vector* const v1, const osl_vector* const v2) {
  if (v1 == v2)
    return true;

  if ((v1->size != v2->size) || (v1->precision != v2->precision))
    return false;

  for (int i = 0; i < v1->size; i++)
    if (osl_int_ne(v1->precision, v1->v[i], v2->v[i]))
      return false;

  return true;
}

/**
 * osl_vector_mul_scalar function:
 * this function returns a new vector corresponding to the one provided
 * as parameter with each entry multiplied by a scalar.
 * \param v      The vector to multiply.
 * \param scalar The scalar coefficient.
 * \return A new vector corresponding to scalar * v.
 */
osl_vector* osl_vector_mul_scalar(const osl_vector* const v, const int scalar) {
  osl_vector* const result = osl_vector_pmalloc(v->precision, v->size);

  for (int i = 0; i < v->size; i++)
    osl_int_mul_si(v->precision, &result->v[i], v->v[i], scalar);

  return result;
}

/**
 * osl_vector_is_scalar function:
 * this function returns 1 if the vector represents a scalar value
 * (all but its last element is 0), 0 otherwise.
 * \param[in] vector The vector to check whether it is scalar or not.
 * \return 1 if the vector is scalar, 0 otherwise.
 */
bool osl_vector_is_scalar(const osl_vector* const vector) {
  if (!vector)
    return false;

  for (int i = 0; i < vector->size - 1; i++)
    if (!osl_int_zero(vector->precision, vector->v[i]))
      return false;
  return true;
}
