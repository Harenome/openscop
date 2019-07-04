/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                          extensions/extbody.c                            **
 **--------------------------------------------------------------------------**
 **                        First version: 07/12/2010                         **
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
#include <string.h>

#include <osl/body.h>
#include <osl/macros.h>
#include <osl/util.h>

#include <osl/extensions/extbody.h>

/******************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/

/**
 * osl_extbody_idump function:
 * this function displays an osl_extbody structure (*extbody) into a
 * file (file, possibly stdout) in a way that trends to be understandable. It
 * includes an indentation level (level) in order to work with others idump
 * functions.
 * \param[in] file   The file where the information has to be printed.
 * \param[in] ebody  The ebody structure to print.
 * \param[in] level  Number of spaces before printing, for each line.
 */
void osl_extbody_idump(FILE* const file, const osl_extbody* const ebody,
                       const int level) {
  // Go to the right level.
  for (int j = 0; j < level; j++)
    fprintf(file, "|\t");

  if (ebody)
    fprintf(file, "+-- osl_extbody\n");
  else
    fprintf(file, "+-- NULL extbody\n");

  if (ebody) {
    // Go to the right level.
    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");

    // Display the number of ebody.
    fprintf(file, "nb_access: %zu\n", ebody->nb_access);

    // Display the coordinates.
    for (size_t i = 0; i < ebody->nb_access; i++) {
      // Go to the right level.
      for (int j = 0; j <= level; j++)
        fprintf(file, "|\t");

      fprintf(file, "start: %d, length: %d\n", ebody->start[i],
              ebody->length[i]);
    }

    // Display the body
    osl_body_idump(file, ebody->body, level);
  }

  // The last line.
  for (int j = 0; j <= level; j++)
    fprintf(file, "|\t");
  fprintf(file, "\n");
}

/**
 * osl_extbody_dump function:
 * this function prints the content of an osl_extbody structure
 * (*ebody) into a file (file, possibly stdout).
 * \param[in] file   The file where the information has to be printed.
 * \param[in] ebody The ebody structure to print.
 */
void osl_extbody_dump(FILE* const file, const osl_extbody* const ebody) {
  osl_extbody_idump(file, ebody, 0);
}

/**
 * osl_extbody_sprint function:
 * this function prints the content of an osl_extbody structure
 * (*ebody) into a string (returned) in the OpenScop textual format.
 * \param[in] ebody The ebody structure to print.
 * \return A string containing the OpenScop dump of the ebodystructure.
 */
char* osl_extbody_sprint(const osl_extbody* const ebody) {
  size_t high_water_mark = OSL_MAX_STRING;
  char* string = NULL;
  char buffer[OSL_MAX_STRING];

  if (ebody != NULL) {
    OSL_malloc(string, char*, high_water_mark * sizeof(char));
    string[0] = '\0';

    sprintf(buffer, "# Number of accesses\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    sprintf(buffer, "%zu\n", ebody->nb_access);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    if (ebody->nb_access) {
      sprintf(buffer, "# Access coordinates (start/length)\n");
      osl_util_safe_strcat(&string, buffer, &high_water_mark);
    }
    for (size_t i = 0; i < ebody->nb_access; i++) {
      sprintf(buffer, "%d %d\n", ebody->start[i], ebody->length[i]);
      osl_util_safe_strcat(&string, buffer, &high_water_mark);
    }

    char* const body_string = osl_body_sprint(ebody->body);
    osl_util_safe_strcat(&string, body_string, &high_water_mark);
    free(body_string);
  }

  return string;
}

/******************************************************************************
 *                               Reading function                             *
 ******************************************************************************/

/**
 * osl_extbody_sread function:
 * this function reads an extbody structure from a string complying
 * to the OpenScop textual format and returns a pointer to this
 * extbody structure. The string should contain only one textual
 * format of an extbody structure. The input parameter is updated to
 * the position in the input string this function reach right after reading the
 * comment structure.
 * \param[in,out] input The input string where to find an extbody
 *                      structure.
 *                      Updated to the position after what has been read.
 * \return A pointer to the extbody structure that has been read.
 */
osl_extbody* osl_extbody_sread(char** const input) {
  if (!input) {
    OSL_debug("no extbody optional tag");
    return NULL;
  }

  // Find the number of ebody provided.
  const int nb_access = osl_util_read_int(NULL, input);
  if (nb_access < 0) {
    OSL_error("negative number of access");
  }
  const size_t nb_access_unsigned = (size_t)nb_access;

  // Allocate the array of start and length.
  osl_extbody* const ebody = osl_extbody_malloc();
  OSL_malloc(ebody->start, int*, nb_access_unsigned * sizeof(int));
  OSL_malloc(ebody->length, int*, nb_access_unsigned * sizeof(int));
  ebody->nb_access = nb_access_unsigned;

  // Get each array start/length.
  for (size_t k = 0; k < nb_access_unsigned; k++) {
    ebody->start[k] = osl_util_read_int(NULL, input);
    ebody->length[k] = osl_util_read_int(NULL, input);
  }

  // Read simple body.
  ebody->body = osl_body_sread(input);

  return ebody;
}

/******************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/

/**
 * osl_extbody_malloc function:
 * this function allocates the memory space for an osl_extbody
 * structure and sets its fields with default values. Then it returns a
 * pointer to the allocated space.
 * \return A pointer to an empty extbody structure with fields set to
 *         default values.
 */
osl_extbody* osl_extbody_malloc(void) {
  osl_extbody* ebody;
  OSL_malloc(ebody, osl_extbody*, sizeof(osl_extbody));

  ebody->nb_access = 0;
  ebody->start = NULL;
  ebody->length = NULL;
  ebody->body = NULL;

  return ebody;
}

/**
 * osl_extbody_free function:
 * this function frees the allocated memory for an ebody structure.
 * \param[in,out] ebody The pointer to the extbody structure we want to free.
 */
void osl_extbody_free(osl_extbody* const ebody) {
  if (ebody) {
    free(ebody->start);
    free(ebody->length);
    osl_body_free(ebody->body);
    free(ebody);
  }
}

/******************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/

/**
 * osl_extbody_clone function:
 * this function builds and returns a "hard copy" (not a pointer copy) of an
 * osl_extbody data structure.
 * \param[in] ebody The pointer to the extbody structure to clone.
 * \return A pointer to the clone of the extbody structure.
 */
osl_extbody* osl_extbody_clone(const osl_extbody* const ebody) {
  if (!ebody)
    return NULL;

  osl_extbody* const clone = osl_extbody_malloc();
  clone->nb_access = ebody->nb_access;
  OSL_malloc(clone->start, int*, ebody->nb_access * sizeof(int));
  OSL_malloc(clone->length, int*, ebody->nb_access * sizeof(int));

  for (size_t i = 0; i < ebody->nb_access; i++) {
    clone->start[i] = ebody->start[i];
    clone->length[i] = ebody->length[i];
  }

  clone->body = osl_body_clone(ebody->body);

  return clone;
}

/**
 * osl_extbody_equal function:
 * this function returns true if the two extbody structures are the
 * same (content-wise), false otherwise. This functions considers two
 * extbody structures as equal if the order of the array start/length
 * differ, however the identifiers, start and length must be the same.
 * \param[in] e1 The first extbody structure.
 * \param[in] e2 The second extbody structure.
 * \return 1 if e1 and e2 are the same (content-wise), 0 otherwise.
 */
bool osl_extbody_equal(const osl_extbody* const e1,
                       const osl_extbody* const e2) {
  if (e1 == e2)
    return true;

  if ((!e1 && e2) || (e1 && !e2)) {
    OSL_info("extbody are not the same");
    return false;
  }

  // Check whether the number of ebody is the same.
  if (e1->nb_access != e2->nb_access) {
    OSL_info("extbody are not the same");
    return false;
  }

  // We accept a different order of the start/length, as long as the
  // identifiers are the same.
  for (size_t i = 0; i < e1->nb_access; i++) {
    bool found = false;
    for (size_t j = 0; j < e2->nb_access; j++) {
      if ((e1->start[i] == e2->start[j]) && (e1->length[i] == e2->length[j])) {
        found = true;
        break;
      }
    }
    if (!found) {
      OSL_info("extbody are not the same");
      return false;
    }
  }

  return osl_body_equal(e1->body, e2->body);
}

/**
 * osl_extbody_interface function:
 * this function creates an interface structure corresponding to the
 * extbody extension and returns it.
 * \return An interface structure for the extbody extension.
 */
osl_interface* osl_extbody_interface(void) {
  osl_interface* const interface = osl_interface_malloc();

  OSL_strdup(interface->URI, OSL_URI_EXTBODY);
  interface->idump = (osl_idump_f)osl_extbody_idump;
  interface->sprint = (osl_sprint_f)osl_extbody_sprint;
  interface->sread = (osl_sread_f)osl_extbody_sread;
  interface->malloc = (osl_malloc_f)osl_extbody_malloc;
  interface->free = (osl_free_f)osl_extbody_free;
  interface->clone = (osl_clone_f)osl_extbody_clone;
  interface->equal = (osl_equal_f)osl_extbody_equal;

  return interface;
}

/**
 * osl_extbody_add function:
 * This function add an entry in the list of coordinates
 */
void osl_extbody_add(osl_extbody* const ebody, const int start,
                     const int length) {
  ebody->nb_access++;

  OSL_realloc(ebody->start, int*, sizeof(int) * ebody->nb_access);
  OSL_realloc(ebody->length, int*, sizeof(int) * ebody->nb_access);

  ebody->start[ebody->nb_access - 1] = start;
  ebody->length[ebody->nb_access - 1] = length;
}
