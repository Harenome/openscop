/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                            extensions/loop.c                             **
 **--------------------------------------------------------------------------**
 **                        First version: 03/06/2013                         **
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

#include <osl/interface.h>
#include <osl/macros.h>
#include <osl/strings.h>
#include <osl/util.h>

#include <osl/extensions/loop.h>

/******************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/

/**
 * osl_loop_idump function:
 * this function displays an osl_loop structure (loop) into a file
 * (file, possibly stdout) in a way that trends to be understandable. It
 * includes an indentation level (level) in order to work with others
 * idump functions.
 *
 * \param[in] file   The file where the information has to be printed.
 * \param[in] loop   The loop structure to print.
 * \param[in] level  Number of spaces before printing, for each line.
 */
void osl_loop_idump(FILE* const file, const osl_loop* loop, const int level) {
  // Go to the right level.
  for (int j = 0; j < level; j++)
    fprintf(file, "|\t");

  if (loop)
    fprintf(file, "+-- osl_loop\n");
  else
    fprintf(file, "+-- NULL loop\n");

  bool first = true;
  size_t number = 1;
  while (loop) {
    // Go to the right level.
    if (!first) {
      // Go to the right level.
      for (int j = 0; j < level; j++)
        fprintf(file, "|\t");

      fprintf(file, "|   osl_loop (node %zu)\n", number);
    } else {
      first = false;
    }

    // A blank line.
    for (int j = 0; j <= level + 1; j++)
      fprintf(file, "|\t");
    fprintf(file, "\n");

    // Display the number of names.
    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+--iterator: %s\n", loop->iter);

    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+--nb_stmts: %zu\n", loop->nb_stmts);

    // Display the id/name.
    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+--stmt_ids:");
    for (size_t i = 0; i < loop->nb_stmts; i++) {
      // Go to the right level.
      fprintf(file, "%2d, ", loop->stmt_ids[i]);
    }
    fprintf(file, "\n");

    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+--private_vars: %s\n", loop->private_vars);

    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+--directive: %d\n", loop->directive);

    for (int j = 0; j <= level; j++)
      fprintf(file, "|\t");
    fprintf(file, "+--user: %s\n", loop->user);

    loop = loop->next;
    number++;

    // Next line.
    if (loop) {
      for (int j = 0; j <= level; j++)
        fprintf(file, "|\t");
      fprintf(file, "V\n");
    }
  }

  // The last line.
  for (int j = 0; j <= level; j++)
    fprintf(file, "|\t");
  fprintf(file, "\n");
}

/**
 * osl_loop_dump function:
 * this function prints the content of an osl_loop structure
 * (*loop) into a file (file, possibly stdout).
 *
 * \param[in] file   The file where the information has to be printed.
 * \param[in] loop The loop structure to print.
 */
void osl_loop_dump(FILE* const file, const osl_loop* const loop) {
  osl_loop_idump(file, loop, 0);
}

/**
 * osl_loop_sprint function:
 * this function prints the content of an osl_loop structure
 * (*loop) into a string (returned) in the OpenScop textual format.
 * \param[in] loop The loop structure to print.
 * \return         A string containing the OpenScop dump of the loop structure.
 */
char* osl_loop_sprint(const osl_loop* loop) {
  size_t high_water_mark = OSL_MAX_STRING;
  char* string = NULL;
  char buffer[OSL_MAX_STRING];

  OSL_malloc(string, char*, high_water_mark * sizeof(char));
  string[0] = '\0';

  sprintf(buffer, "# Number of loops\n%d\n", osl_loop_count(loop));
  osl_util_safe_strcat(&string, buffer, &high_water_mark);

  size_t nloop = 0;
  while (loop) {
    sprintf(buffer, "# ===========================================\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    sprintf(buffer, "# Loop number %zu \n", ++nloop);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    sprintf(buffer, "# Iterator name\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);
    sprintf(buffer, "%s\n", loop->iter);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    sprintf(buffer, "# Number of stmts\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);
    sprintf(buffer, "%zu\n", loop->nb_stmts);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    if (loop->nb_stmts) {
      sprintf(buffer, "# Statement identifiers\n");
      osl_util_safe_strcat(&string, buffer, &high_water_mark);
    }
    for (size_t i = 0; i < loop->nb_stmts; i++) {
      sprintf(buffer, "%d\n", loop->stmt_ids[i]);
      osl_util_safe_strcat(&string, buffer, &high_water_mark);
    }

    sprintf(buffer, "# Private variables\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);
    sprintf(buffer, "%s\n", loop->private_vars);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    sprintf(buffer, "# Directive\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);
    sprintf(buffer, "%d", loop->directive);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    // special case for OSL_LOOP_DIRECTIVE_USER
    if (loop->directive & OSL_LOOP_DIRECTIVE_USER) {
      sprintf(buffer, " %s", loop->user);
      osl_util_safe_strcat(&string, buffer, &high_water_mark);
    }
    sprintf(buffer, "\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    loop = loop->next;
  }

  OSL_realloc(string, char*, (strlen(string) + 1) * sizeof(char));
  return string;
}

/******************************************************************************
 *                               Reading function                             *
 ******************************************************************************/

/**
 * osl_loop_sread function:
 * this function reads a loop structure from a string complying to the
 * OpenScop textual format and returns a pointer to this loop structure.
 * The input parameter is updated to the position in the input string this
 * function reaches right after reading the comment structure.
 *
 * \param[in,out] input   The input string where to find an loop structure.
 *                        Updated to the position after what has been read.
 * \return                A pointer to the loop structure that has been read.
 */
osl_loop* osl_loop_sread(char** input) {
  if (!input) {
    OSL_debug("no loop optional tag");
    return NULL;
  }

  // Find the number of names provided.
  int nb_loops = osl_util_read_int(NULL, input);
  if (!nb_loops)
    return NULL;

  // Allocate the array of id and names.
  osl_loop* const head = osl_loop_malloc();
  osl_loop* loop = head;

  while (nb_loops != 0) {
    loop->iter = osl_util_read_string(NULL, input);
    loop->nb_stmts = (size_t)osl_util_read_int(NULL, input);

    OSL_malloc(loop->stmt_ids, int*, loop->nb_stmts * sizeof(int));
    for (size_t i = 0; i < loop->nb_stmts; i++)
      loop->stmt_ids[i] = osl_util_read_int(NULL, input);

    loop->private_vars = osl_util_read_line(NULL, input);
    if (!strcmp(loop->private_vars, "(null)")) {
      free(loop->private_vars);
      loop->private_vars = NULL;
    }

    loop->directive = osl_util_read_int(NULL, input);

    // special case for OSL_LOOP_DIRECTIVE_USER
    if (loop->directive & OSL_LOOP_DIRECTIVE_USER) {
      loop->user = osl_util_read_line(NULL, input);
      if (!strcmp(loop->user, "(null)")) {
        free(loop->user);
        loop->user = NULL;
      }
    }

    nb_loops--;
    if (nb_loops != 0) {
      loop->next = osl_loop_malloc();
      loop = loop->next;
    }
  }

  return head;
}

/******************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/

/**
 * osl_loop_malloc function:
 * this function allocates the memory space for an osl_loop
 * structure and sets its fields with default values. Then it returns a
 * pointer to the allocated space.
 *
 * \return  A pointer to an empty loop structure with fields set to
 *          default values.
 */
osl_loop* osl_loop_malloc(void) {
  osl_loop* loop;

  OSL_malloc(loop, osl_loop*, sizeof(osl_loop));
  loop->iter = NULL;
  loop->nb_stmts = 0;
  loop->stmt_ids = NULL;
  loop->private_vars = NULL;
  loop->directive = 0;
  loop->user = NULL;
  loop->next = NULL;

  return loop;
}

/**
 * osl_loop_free function:
 * this function frees the allocated memory for an loop structure.
 *
 * \param[in,out] loop The pointer to the loop structure we want to free.
 */
void osl_loop_free(osl_loop* loop) {
  while (loop) {
    osl_loop* const tmp = loop;

    if (loop->iter)
      free(loop->iter);
    if (loop->stmt_ids)
      free(loop->stmt_ids);
    if (loop->private_vars)
      free(loop->private_vars);
    if (loop->user)
      free(loop->user);

    loop = loop->next;

    free(tmp);
  }
}

/******************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/

/**
 * osl_loop_clone_one function:
 * this function builds and returns a "hard copy" (not a pointer copy) of
 * "one" (and not the whole list) osl_loop data structure.
 *
 * \param[in] loop  The pointer to the loop structure to clone.
 * \return          A pointer to the clone of the loop structure.
 */
osl_loop* osl_loop_clone_one(const osl_loop* loop) {
  if (!loop)
    return NULL;

  osl_loop* const clone = osl_loop_malloc();
  OSL_strdup(clone->iter, loop->iter);
  clone->nb_stmts = loop->nb_stmts;
  OSL_malloc(clone->stmt_ids, int*, loop->nb_stmts * sizeof(int));

  for (size_t i = 0; i < loop->nb_stmts; i++) {
    clone->stmt_ids[i] = loop->stmt_ids[i];
  }

  clone->directive = loop->directive;

  if (loop->private_vars)
    OSL_strdup(clone->private_vars, loop->private_vars);

  if (loop->user)
    OSL_strdup(clone->user, loop->user);

  return clone;
}

/**
 * osl_loop_clone function:
 * this function builds and returns a "hard copy" (not a pointer copy) of a
 * list of osl_loop data structures.
 *
 * \param[in] loop  The pointer to the list of loop structure to clone.
 * \return          A pointer to the clone of list of the loop structure.
 */
osl_loop* osl_loop_clone(const osl_loop* loop) {
  if (!loop)
    return NULL;

  osl_loop* clone = NULL;
  osl_loop* head = NULL;

  while (loop) {
    if (!clone) {
      head = clone = osl_loop_clone_one(loop);
    } else {
      clone->next = osl_loop_clone_one(loop);
      clone = clone->next;
    }

    loop = loop->next;
  }

  return head;
}

/**
 * osl_loop_equal_one function:
 * this function returns true if the two loop structures are the same
 * (content-wise), false otherwise. This functions considers two loop
 * structures as equal if the order of the array names differ, however
 * the identifiers and names must be the same.
 *
 * \param[in] a1 The first loop structure.
 * \param[in] a2 The second loop structure.
 * \return       1 if a1 and a2 are the same (content-wise), 0 otherwise.
 */
bool osl_loop_equal_one(const osl_loop* const a1, const osl_loop* const a2) {
  if (a1 == a2)
    return true;

  if ((!a1 && a2) || (a1 && !a2)) {
    // OSL_info("loops are not the same (compare with NULL)");
    return false;
  }

  // Check whether the number of names is the same.
  if (a1->nb_stmts != a2->nb_stmts) {
    // OSL_info("loops are not the same (nb_stmts)");
    return false;
  }

  if (strcmp(a1->iter, a2->iter)) {
    // OSL_info("loops are not the same (iter name)");
    return false;
  }
  // We accept a different order of the names, as long as the identifiers
  // are the same.
  for (size_t i = 0; i < a1->nb_stmts; i++) {
    bool found = false;
    for (size_t j = 0; j < a2->nb_stmts; j++) {
      if (a1->stmt_ids[i] == a2->stmt_ids[j]) {
        found = true;
        break;
      }
    }
    if (!found) {
      // OSL_info("loop are not the same (stmt ids)");
      return false;
    }
  }

  // TODO: necessarily same ???
  if (a1->private_vars != a2->private_vars) {  // NULL check
    if (strcmp(a1->private_vars, a2->private_vars)) {
      // OSL_info("loops are not the same (private vars)");
      return false;
    }
  }

  // TODO: necessarily same ???
  if (a1->directive != a2->directive) {
    // OSL_info("loops are not the same (directive)");
    return false;
  }

  if (a1->user != a2->user) {  // NULL check
    if (strcmp(a1->user, a2->user)) {
      return false;
    }
  }

  return true;
}

/**
 * osl_loop_equal function:
 * this function returns true if the two loop lists are the same
 * (content-wise), false otherwise. Two lists are equal if one contains
 * all the elements of the other and vice versa. The exact order of the
 * nodes is not taken into account by this function.
 *
 * \param[in] a1  The first loop list.
 * \param[in] a2  The second loop list.
 * \return        1 if a1 and a2 are the same (content-wise), 0 otherwise.
 */
bool osl_loop_equal(const osl_loop* a1, const osl_loop* a2) {
  if (a1 == a2)
    return true;

  if ((!a1 && a2) || (a1 && !a2)) {
    OSL_info("lists of loops are not the same (compare with NULL)");
    return false;
  }

  if (osl_loop_count(a1) != osl_loop_count(a2)) {
    OSL_info("list of loops are not the same");
    return false;
  }

  while (a1) {
    bool found = false;
    const osl_loop* temp = a2;

    while (temp) {
      if (osl_loop_equal_one(a1, temp) == 1) {
        found = true;
        break;
      }
      temp = temp->next;
    }

    if (!found) {
      OSL_info("list of loops are not the same");
      return false;
    }
    a1 = a1->next;
  }

  return true;
}

/**
 * osl_loop_interface function:
 * this function creates an interface structure corresponding to the loop
 * extension and returns it.
 *
 * \return  An interface structure for the loop extension.
 */
osl_interface* osl_loop_interface(void) {
  osl_interface* const interface = osl_interface_malloc();

  OSL_strdup(interface->URI, OSL_URI_LOOP);
  interface->idump = (osl_idump_f)osl_loop_idump;
  interface->sprint = (osl_sprint_f)osl_loop_sprint;
  interface->sread = (osl_sread_f)osl_loop_sread;
  interface->malloc = (osl_malloc_f)osl_loop_malloc;
  interface->free = (osl_free_f)osl_loop_free;
  interface->clone = (osl_clone_f)osl_loop_clone;
  interface->equal = (osl_equal_f)osl_loop_equal;

  return interface;
}

/**
 * osl_loop_add function:
 * this function adds a loop structure at the end of the list
 *
 * \param[in,out] ll  Pointer to a list of loops.
 * \param[in] loop    Pointer to the loop structure to be added.
 */
void osl_loop_add(osl_loop* loop, osl_loop** ll) {
  while (*ll)
    ll = &(*ll)->next;

  *ll = loop;
}

/**
 * osl_loop_count:
 * this function returns the number of elements in the list
 *
 * \param[in] ll  Pointer to a list of loops.
 * \return        Number of elements in the list
 */
int osl_loop_count(const osl_loop* ll) {
  int count = 0;
  while (ll) {
    count++;
    ll = ll->next;
  }

  return count;
}
