/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                          extensions/textual.h                            **
 **--------------------------------------------------------------------------**
 **                        First version: 15/07/2011                         **
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

#ifndef OSL_TEXTUAL_H
#define OSL_TEXTUAL_H

#include <stdio.h>

#include <osl/attributes.h>
#include <osl/interface.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define OSL_URI_TEXTUAL "textual"

/**
 * The osl_textual structure stores the complete textual
 * representation of the scop extension field. It is a special case of
 * extension since it does not require start and end tag.
 */
struct osl_textual {
  char* textual; /**< Full extension string as a 0-terminated string. */
};
typedef struct osl_textual osl_textual;

/* The following typedefs are deprecated. Use osl_textual. */
typedef struct osl_textual osl_textual_t OSL_DEPRECATED;
typedef struct osl_textual* osl_textual_p OSL_DEPRECATED;

/******************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/

void osl_textual_idump(FILE*, const osl_textual*, int) OSL_NONNULL_ARGS(1);
void osl_textual_dump(FILE*, const osl_textual*) OSL_NONNULL_ARGS(1);
char* osl_textual_sprint(const osl_textual*) OSL_WARN_UNUSED_RESULT;

/******************************************************************************
 *                               Reading function                             *
 ******************************************************************************/

osl_textual* osl_textual_sread(char**) OSL_WARN_UNUSED_RESULT;

/******************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/

osl_textual* osl_textual_malloc(void) OSL_WARN_UNUSED_RESULT;
void osl_textual_free(osl_textual*);

/******************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/

osl_textual* osl_textual_clone(const osl_textual*) OSL_WARN_UNUSED_RESULT;
bool osl_textual_equal(const osl_textual*, const osl_textual*);
osl_interface* osl_textual_interface(void) OSL_WARN_UNUSED_RESULT;

#if defined(__cplusplus)
}
#endif

#endif /* define OSL_TEXTUAL_H */
