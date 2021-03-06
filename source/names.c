/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                                 names.c                                  **
 **--------------------------------------------------------------------------**
 **                        First version: 18/04/2011                         **
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <osl/macros.h>
#include <osl/names.h>
#include <osl/strings.h>

/******************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/

/**
 * osl_names_idump function:
 * this function displays an osl_names structure (*names) into a
 * file (file, possibly stdout) in a way that trends to be understandable. It
 * includes an indentation level (level) in order to work with others
 * idump functions.
 * \param[in] file  The file where the information has to be printed.
 * \param[in] names The names structure whose information has to be printed.
 * \param[in] level Number of spaces before printing, for each line.
 */
void osl_names_idump(FILE* const file, const osl_names* const names,
                     int level) {
  // Go to the right level.
  osl_util_idump_indent(file, level);

  if (names) {
    fprintf(file, "+-- osl_names\n");

    // A blank line.
    osl_util_idump_blank_line(file, level + 2);

    // Print the various names.
    osl_strings_idump(file, names->parameters, level + 1);
    osl_strings_idump(file, names->iterators, level + 1);
    osl_strings_idump(file, names->scatt_dims, level + 1);
    osl_strings_idump(file, names->local_dims, level + 1);
    osl_strings_idump(file, names->arrays, level + 1);
  } else {
    fprintf(file, "+-- NULL names\n");
  }

  // The last line.
  osl_util_idump_blank_line(file, level + 1);
}

/**
 * osl_names_dump function:
 * this function prints the content of an osl_names structure
 * (*names) into a file (file, possibly stdout).
 * \param[in] file  The file where the information has to be printed.
 * \param[in] names The names structure whose information has to be printed.
 */
void osl_names_dump(FILE* const file, const osl_names* const names) {
  osl_names_idump(file, names, 0);
}

/******************************************************************************
 *                               Reading function                             *
 ******************************************************************************/

/******************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/

/**
 * osl_names_malloc function:
 * this function allocates the memory space for an osl_names
 * structure and sets its fields with default values. Then it returns a
 * pointer to the allocated space.
 * \return A pointer to an empty names structure with fields set to
 *         default values.
 */
osl_names* osl_names_malloc(void) {
  osl_names* names;

  OSL_malloc(names, osl_names*, sizeof(osl_names));
  names->parameters = NULL;
  names->iterators = NULL;
  names->scatt_dims = NULL;
  names->local_dims = NULL;
  names->arrays = NULL;

  return names;
}

/**
 * osl_names_free function:
 * This function frees the allocated memory for an osl_names
 * structure. If the names are not character strings, it is the
 * responsibility of the user to free each array of elements (including
 * the array itself), this function will only free the osl_names shell.
 * \param[in,out] names The pointer to the names structure we want to free.
 */
void osl_names_free(osl_names* const names) {
  if (names) {
    osl_strings_free(names->parameters);
    osl_strings_free(names->iterators);
    osl_strings_free(names->scatt_dims);
    osl_strings_free(names->local_dims);
    osl_strings_free(names->arrays);

    free(names);
  }
}

/******************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/

/**
 * osl_names_generate function:
 * this function generates some names. For each kind of name it will generate
 * a given number of names with a given prefix followed by a number.
 * \param[in] parameter_prefix Prefix for parameter names.
 * \param[in] nb_parameters    Number of parameters names to generate.
 * \param[in] iterator_prefix  Prefix for iterator names.
 * \param[in] nb_iterators     Number of iterators names to generate.
 * \param[in] scatt_dim_prefix Prefix for scattering dimension names.
 * \param[in] nb_scatt_dims    Number of scattering dim names to generate.
 * \param[in] local_dim_prefix Prefix for local dimension names.
 * \param[in] nb_local_dims    Number of local dimension names to generate.
 * \param[in] array_prefix     Prefix for array names.
 * \param[in] nb_arrays        Number of array names to generate.
 * \return A new names structure containing generated names.
 */
osl_names* osl_names_generate(
    const char* const parameter_prefix, int nb_parameters,
    const char* const iterator_prefix, int nb_iterators,
    const char* const scatt_dim_prefix, int nb_scatt_dims,
    const char* const local_dim_prefix, int nb_local_dims,
    const char* const array_prefix, int nb_arrays) {
  osl_names* const names = osl_names_malloc();

  names->parameters = osl_strings_generate(parameter_prefix, nb_parameters);
  names->iterators = osl_strings_generate(iterator_prefix, nb_iterators);
  names->scatt_dims = osl_strings_generate(scatt_dim_prefix, nb_scatt_dims);
  names->local_dims = osl_strings_generate(local_dim_prefix, nb_local_dims);
  names->arrays = osl_strings_generate(array_prefix, nb_arrays);

  return names;
}

/**
 * osl_names_clone function:
 * this function builds and returns a "hard copy" (not a pointer copy) of an
 * osl_names data structure provided as parameter.
 * \param[in] names The pointer to the names structure we want to clone.
 * \return A pointer to the clone of the names structure provided as parameter.
 */
osl_names* osl_names_clone(const osl_names* const names) {
  if (!names)
    return NULL;

  osl_names* const clone = osl_names_malloc();
  clone->parameters = osl_strings_clone(names->parameters);
  clone->iterators = osl_strings_clone(names->iterators);
  clone->scatt_dims = osl_strings_clone(names->scatt_dims);
  clone->local_dims = osl_strings_clone(names->local_dims);
  clone->arrays = osl_strings_clone(names->arrays);

  return clone;
}
