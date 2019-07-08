/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                           extensions/region.h                            **
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

#ifndef OSL_REGION_H
#define OSL_REGION_H

#include <stdbool.h>
#include <stdio.h>

#include <osl/attributes.h>
#include <osl/strings.h>
#include <osl/generic.h>
#include <osl/extensions/annotation.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* URI */
#define OSL_URI_REGION "region"

/* Location */
#define OSL_REGION_NONE 0
#define OSL_REGION_GLOBAL 1

/**
 * \brief Regions
 */
struct osl_region {
  /** Location of the region. */
  int location;

  /* Extensions. */
  osl_generic* extensions;

  struct osl_region* next;
};
typedef struct osl_region osl_region;

/******************************************************************************
 * Structure display functions                                                *
 ******************************************************************************/

void osl_region_idump(FILE* file, const osl_region* region, int level)
    OSL_NONNULL_ARGS(1);
void osl_region_dump(FILE* file, const osl_region* region)
    OSL_NONNULL_ARGS(1);
char* osl_region_sprint(const osl_region* region);

/******************************************************************************
 * Reading functions                                                          *
 ******************************************************************************/

osl_region* osl_region_sread(char** input) OSL_WARN_UNUSED_RESULT;

/******************************************************************************
 * Memory allocation/deallocation functions                                   *
 ******************************************************************************/

osl_region* osl_region_malloc(void) OSL_WARN_UNUSED_RESULT;
void osl_region_free(osl_region* region);

/******************************************************************************
 * Processing functions                                                       *
 ******************************************************************************/

osl_region* osl_region_clone_one(const osl_region* region)
    OSL_WARN_UNUSED_RESULT;
osl_region* osl_region_clone(const osl_region* region)
    OSL_WARN_UNUSED_RESULT;

bool osl_region_equal_one(const osl_region* a1, const osl_region* a2);
bool osl_region_equal(const osl_region* a1, const osl_region* a2);
size_t osl_region_count(const osl_region* ll);

osl_interface* osl_region_interface(void) OSL_WARN_UNUSED_RESULT;

void osl_region_append_prefix(osl_region* region, int prefix_type,
                              char* prefix);
void osl_region_append_suffix(osl_region* region, int suffix_type,
                              char* suffix);
void osl_region_append_prelude(osl_region* region, int prelude_type,
                               char* prelude);
void osl_region_append_postlude(osl_region* region, int postlude_type,
                                char* postlude);

#if defined(__cplusplus)
}
#endif

#endif
