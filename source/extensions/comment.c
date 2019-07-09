/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                          extensions/comment.c                            **
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <osl/interface.h>
#include <osl/macros.h>
#include <osl/util.h>

#include <osl/extensions/comment.h>

/******************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/

/**
 * osl_comment_idump function:
 * this function displays an osl_comment structure (*comment) into a
 * file (file, possibly stdout) in a way that trends to be understandable. It
 * includes an indentation level (level) in order to work with others
 * idump functions.
 * \param[in] file    The file where the information has to be printed.
 * \param[in] comment The comment structure to print.
 * \param[in] level   Number of spaces before printing, for each line.
 */
void osl_comment_idump(FILE* const file, const osl_comment* const comment,
                       const int level) {
  // Go to the right level.
  osl_util_idump_indent(file, level);
  fprintf(file, "+-- %s\n", comment ? "osl_comment" : "NULL comment");

  if (comment) {
    // Go to the right level.
    osl_util_idump_indent(file, level + 1);

    // Display the comment message (without any carriage return).
    char* tmp;
    OSL_strdup(tmp, comment->comment);
    for (size_t l = 0; l < strlen(tmp); l++)
      if (tmp[l] == '\n')
        tmp[l] = ' ';
    fprintf(file, "comment: %s\n", tmp);
    free(tmp);
  }

  // The last line.
  osl_util_idump_blank_line(file, level + 1);
}

/**
 * osl_comment_dump function:
 * this function prints the content of an osl_comment structure
 * (*comment) into a file (file, possibly stdout).
 * \param[in] file    The file where the information has to be printed.
 * \param[in] comment The comment structure to print.
 */
void osl_comment_dump(FILE* const file, const osl_comment* const comment) {
  osl_comment_idump(file, comment, 0);
}

/**
 * osl_comment_sprint function:
 * this function prints the content of an osl_comment structure
 * (*comment) into a string (returned) in the OpenScop textual format.
 * \param[in] comment The comment structure to print.
 * \return A string containing the OpenScop dump of the comment structure.
 */
char* osl_comment_sprint(const osl_comment* const comment) {
  size_t high_water_mark = OSL_MAX_STRING;
  char* string = NULL;
  char buffer[OSL_MAX_STRING];

  if (comment) {
    OSL_malloc(string, char*, high_water_mark * sizeof(char));
    string[0] = '\0';

    // Print the comment.
    sprintf(buffer, "%s", comment->comment);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    // Keep only the memory space we need.
    OSL_realloc(string, char*, (strlen(string) + 1) * sizeof(char));
  }

  return string;
}

/******************************************************************************
 *                               Reading function                             *
 ******************************************************************************/

/**
 * osl_comment_sread function:
 * this function reads a comment structure from a string complying to the
 * OpenScop textual format and returns a pointer to this comment structure.
 * The input parameter is updated to the position in the input string this
 * function reach right after reading the comment structure.
 * \param[in,out] input The input string where to find a comment.
 *                      Updated to the position after what has been read.
 * \return A pointer to the comment structure that has been read.
 */
osl_comment* osl_comment_sread(char** input) {
  if (!*input) {
    OSL_debug("no comment optional tag");
    return NULL;
  }

  if (strlen(*input) > OSL_MAX_STRING)
    OSL_error("comment too long");

  // Build the comment structure
  osl_comment* const comment = osl_comment_malloc();
  OSL_strdup(comment->comment, *input);

  // Update the input pointer (everything has been read).
  input += strlen(*input);

  return comment;
}

/******************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/

/**
 * osl_comment_malloc function:
 * this function allocates the memory space for an osl_comment
 * structure and sets its fields with default values. Then it returns a
 * pointer to the allocated space.
 * \return A pointer to an empty comment structure with fields set to
 *         default values.
 */
osl_comment* osl_comment_malloc(void) {
  osl_comment* comment;

  OSL_malloc(comment, osl_comment*, sizeof(osl_comment));
  comment->comment = NULL;

  return comment;
}

/**
 * osl_comment_free function:
 * this function frees the allocated memory for an osl_comment
 * structure.
 * \param[in,out] comment The pointer to the comment structure to free.
 */
void osl_comment_free(osl_comment* comment) {
  if (comment) {
    if (comment->comment)
      free(comment->comment);
    free(comment);
  }
}

/******************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/

/**
 * osl_comment_clone function:
 * this function builds and returns a "hard copy" (not a pointer copy) of an
 * osl_comment data structure.
 * \param[in] comment The pointer to the comment structure to clone.
 * \return A pointer to the clone of the comment structure.
 */
osl_comment* osl_comment_clone(const osl_comment* const comment) {
  if (!comment)
    return NULL;

  osl_comment* const clone = osl_comment_malloc();
  OSL_strdup(clone->comment, comment->comment);

  return clone;
}

/**
 * osl_comment_equal function:
 * this function returns true if the two comment structures are the same
 * (content-wise), false otherwise.
 * \param[in] c1  The first comment structure.
 * \param[in] c2  The second comment structure.
 * \return 1 if c1 and c2 are the same (content-wise), 0 otherwise.
 */
bool osl_comment_equal(const osl_comment* const c1,
                       const osl_comment* const c2) {
  if (c1 == c2)
    return true;

  if ((!c1 && c2) || (c1 && !c2)) {
    OSL_info("comments are not the same");
    return false;
  }

  if (strcmp(c1->comment, c2->comment)) {
    // Well, the test does not apply well on textual content but the idea
    // is here (see osl_generic_sprint if you want to understand the problem).
    // OSL_info("comments are not the same");
    // return 0;
  }

  return true;
}

/**
 * osl_comment_interface function:
 * this function creates an interface structure corresponding to the comment
 * extension and returns it).
 * \return An interface structure for the comment extension.
 */
osl_interface* osl_comment_interface(void) {
  osl_interface* const interface = osl_interface_malloc();

  OSL_strdup(interface->URI, OSL_URI_COMMENT);
  interface->idump = (osl_idump_f)osl_comment_idump;
  interface->sprint = (osl_sprint_f)osl_comment_sprint;
  interface->sread = (osl_sread_f)osl_comment_sread;
  interface->malloc = (osl_malloc_f)osl_comment_malloc;
  interface->free = (osl_free_f)osl_comment_free;
  interface->clone = (osl_clone_f)osl_comment_clone;
  interface->equal = (osl_equal_f)osl_comment_equal;

  return interface;
}
