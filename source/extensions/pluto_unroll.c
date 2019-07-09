/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                        extensions/pluto_unroll.c                         **
 **--------------------------------------------------------------------------**
 **                        First version: 26/06/2014                         **
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

#include <stdlib.h>
#include <string.h>

#include <osl/macros.h>

#include <osl/extensions/pluto_unroll.h>

/******************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/

/**
 * osl_loop_idump function:
 * this function displays an osl_pluto_unroll structure (pluto_unroll) into a
 * file (file, possibly stdout) in a way that trends to be understandable. It
 * includes an indentation level (level) in order to work with others
 * idump functions.
 *
 * \param[in] file         The file where the information has to be printed.
 * \param[in] pluto_unroll The pluto_unroll structure to print.
 * \param[in] level        Number of spaces before printing, for each line.
 */
void osl_pluto_unroll_idump(FILE* const file,
                            const osl_pluto_unroll* pluto_unroll,
                            const int level) {
  // Go to the right level.
  osl_util_idump_indent(file, level);
  fprintf(file, "+-- %s\n", pluto_unroll ? "osl_pluto_unroll" : "NULL pluto_unroll");

  size_t number = 1;
  bool first = true;
  while (pluto_unroll) {
    // Go to the right level.
    if (!first) {
      // Go to the right level.
      osl_util_idump_indent(file, level);
      fprintf(file, "|   osl_pluto_unroll (node %zu)\n", number++);
    } else {
      first = false;
    }

    // A blank line.
    osl_util_idump_blank_line(file, level + 2);

    // Display the names of iterators.
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "+--iterator: %s\n", pluto_unroll->iter);

    // Display the names of iterators.
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "+--jam: %i\n", pluto_unroll->jam);

    // Display the names of iterators.
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "+--factor: %u\n", pluto_unroll->factor);

    pluto_unroll = pluto_unroll->next;

    // Next line.
    if (pluto_unroll)
      osl_util_idump_next_link(file, level);
  }

  // The last line.
  osl_util_idump_blank_line(file, level + 1);
}

/**
 * osl_pluto_unroll_dump function:
 * this function prints the content of an osl_pluto_unroll structure
 * (*pluto_unroll) into a file (file, possibly stdout).
 *
 * \param[in] file         The file where the information has to be printed.
 * \param[in] pluto_unroll The pluto_unroll structure to print.
 */
void osl_pluto_unroll_dump(FILE* const file,
                           const osl_pluto_unroll* const pluto_unroll) {
  osl_pluto_unroll_idump(file, pluto_unroll, 0);
}

/**
 * osl_pluto_unroll_sprint function:
 * this function prints the content of an osl_pluto_unroll structure
 * (*pluto_unroll) into a string (returned) in the OpenScop textual format.
 *
 * \param[in] pluto_unroll The loop structure to print.
 *
 * \return a string containing the OpenScop dump of the loop structure.
 */
char* osl_pluto_unroll_sprint(const osl_pluto_unroll* pluto_unroll) {
  char* string = NULL;
  char buffer[OSL_MAX_STRING] = {0};

  strcat(buffer, "# Pluto unroll\n");
  if (pluto_unroll) {
    strcat(buffer, "1\n");
  } else {
    strcat(buffer, "0\n");
  }

  while (pluto_unroll) {
    strcat(buffer, "# Iterator name\n");
    sprintf(buffer + strlen(buffer), "%s\n", pluto_unroll->iter);

    strcat(buffer, "# Jam\n");
    sprintf(buffer + strlen(buffer), "%i\n", pluto_unroll->jam);

    strcat(buffer, "# Factor\n");
    sprintf(buffer + strlen(buffer), "%u\n", pluto_unroll->factor);

    pluto_unroll = pluto_unroll->next;

    strcat(buffer, "# Next\n");
    if (pluto_unroll != NULL) {
      strcat(buffer, "1\n");
    } else {
      strcat(buffer, "0\n");
    }
  }

  OSL_realloc(string, char*, (strlen(buffer) + 1) * sizeof(char));
  strcpy(string, buffer);

  return string;
}

/******************************************************************************
 *                               Reading function                             *
 ******************************************************************************/

/**
 * osl_pluto_unroll_sread function:
 * this function reads a pluto_unroll structure from a string complying to the
 * OpenScop textual format and returns a pointer to this pluto_unroll structure.
 * The input parameter is updated to the position in the input string this
 * function reaches right after reading the comment structure.
 *
 * \param[in,out] input The input string where to find an pluto_unroll
 *                      structure.
 *                      Updated to the position after what has been read.
 *
 * \return a pointer to the pluto_unroll structure that has been read.
 */
osl_pluto_unroll* osl_pluto_unroll_sread(char** const input) {
  const int count = osl_util_read_int(NULL, input);
  osl_pluto_unroll* const res = count == 1 ? osl_pluto_unroll_malloc() : NULL;

  osl_pluto_unroll* current = res;
  while (current != NULL) {
    // iter
    current->iter = osl_util_read_line(NULL, input);
    // jam
    current->jam = osl_util_read_int(NULL, input);
    // factor
    current->factor = (unsigned int)osl_util_read_int(NULL, input);
    // Next
    if (osl_util_read_int(NULL, input) == 1) {
      current->next = osl_pluto_unroll_malloc();
    }
    current = current->next;
  }

  return res;
}

/******************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/

/**
 * osl_pluto_unroll_malloc function:
 * this function allocates the memory space for an osl_pluto_unroll
 * structure and sets its fields with default values. Then it returns a
 * pointer to the allocated space.
 *
 * \return a pointer to an empty pluto_unroll structure with fields set to
 *         default values.
 */
osl_pluto_unroll* osl_pluto_unroll_malloc(void) {
  osl_pluto_unroll* pluto_unroll = NULL;

  OSL_malloc(pluto_unroll, osl_pluto_unroll*, sizeof(osl_pluto_unroll));
  pluto_unroll->iter = NULL;
  pluto_unroll->jam = false;
  pluto_unroll->factor = 0;
  pluto_unroll->next = NULL;

  return pluto_unroll;
}

/**
 * osl_pluto_unroll_free function:
 * this function frees the allocated memory for an pluto_unroll structure.
 *
 * \param[in,out] pluto_unroll The pointer to the pluto_unroll structure
 *                             we want to free.
 */
void osl_pluto_unroll_free(osl_pluto_unroll* const pluto_unroll) {
  if (pluto_unroll) {
    osl_pluto_unroll_free(pluto_unroll->next);

    free(pluto_unroll->iter);
    free(pluto_unroll);
  }
}

/******************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/

/**
 * osl_pluto_unroll_fill function:
 * this function fill the osl_pluto_unroll with the iterator name,
 * if jam or not and the unroll factor
 *
 * \param[in] pluto_unroll The pointer to the osl_pluto_unroll
 * \param[in] jam          true if jam, falsee otherwise
 * \param[in] factor       Unroll factor
 */
void osl_pluto_unroll_fill(osl_pluto_unroll* const pluto_unroll,
                           char const* const iterator_name, bool jam,
                           const unsigned int factor) {
  if (pluto_unroll) {
    // iter
    if (iterator_name) {
      OSL_realloc(pluto_unroll->iter, char*,
                  (strlen(iterator_name) + 1) * sizeof(char));
      strcpy(pluto_unroll->iter, iterator_name);
    }
    // jam
    pluto_unroll->jam = jam;
    // factor
    pluto_unroll->factor = factor;
  } else {
    OSL_warning("osl_pluto_unroll_fill can not fill NULL osl_pluto_unroll_p");
  }
}

/**
 * osl_pluto_unroll_clone function:
 * this function builds and returns a "hard copy" (not a pointer copy) of a
 * list of osl_pluto_unroll data structures.
 *
 * \param[in] pluto_unroll The pointer to the list of pluto_unroll structure to
 *                         clone.
 *
 * \return a pointer to the clone of list of the pluto_unroll structure.
 */
osl_pluto_unroll* osl_pluto_unroll_clone(const osl_pluto_unroll* pluto_unroll) {
  osl_pluto_unroll* const res = pluto_unroll ? osl_pluto_unroll_malloc() : NULL;

  osl_pluto_unroll* current = res;
  while (pluto_unroll) {
    osl_pluto_unroll_fill(current, pluto_unroll->iter, pluto_unroll->jam,
                          pluto_unroll->factor);

    pluto_unroll = pluto_unroll->next;

    if (pluto_unroll) {
      current->next = osl_pluto_unroll_malloc();
    }
    current = current->next;
  }

  return res;
}

/**
 * osl_pluto_unroll_equal function:
 * this function returns true if the two pluto_unroll lists are the same
 * (content-wise), false otherwise. Two lists are equal if one contains
 * all the elements of the other and vice versa. The exact order of the
 * nodes is not taken into account by this function.
 *
 * \param[in] a The first pluto_unroll list.
 * \param[in] b The second pluto_unroll list.
 *
 * \return 1 if a and b are the same (content-wise), 0 otherwise.
 */
bool osl_pluto_unroll_equal(const osl_pluto_unroll* a,
                            const osl_pluto_unroll* b) {
  if (a == b)
    return true;

  if ((!a && b) || (a && !b))
    return false;

  while (a) {
    // Iter
    if ((!a->iter && b->iter) || (a->iter && !b->iter))
      return false;
    if (a->iter && strcmp(a->iter, b->iter) != 0)
      return false;
    // Jam
    if (a->jam != b->jam)
      return false;
    // Factor
    if (a->factor != b->factor)
      return false;
    // Next
    if (a->next == b->next)
      return true;
    if ((!a->next && b->next) || (a->next && !b->next))
      return false;
    a = a->next;
    b = b->next;
  }

  return true;
}

/**
 * osl_pluto_unroll_interface function:
 * this function creates an interface structure corresponding to the
 * pluto_unroll extension and returns it.
 *
 * \return an interface structure for the pluto_unroll extension.
 */
osl_interface* osl_pluto_unroll_interface(void) {
  osl_interface* const interface = osl_interface_malloc();

  OSL_strdup(interface->URI, OSL_URI_PLUTO_UNROLL);
  interface->idump = (osl_idump_f)osl_pluto_unroll_idump;
  interface->sprint = (osl_sprint_f)osl_pluto_unroll_sprint;
  interface->sread = (osl_sread_f)osl_pluto_unroll_sread;
  interface->malloc = (osl_malloc_f)osl_pluto_unroll_malloc;
  interface->free = (osl_free_f)osl_pluto_unroll_free;
  interface->clone = (osl_clone_f)osl_pluto_unroll_clone;
  interface->equal = (osl_equal_f)osl_pluto_unroll_equal;

  return interface;
}
