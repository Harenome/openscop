
    /*+-----------------------------------------------------------------**
     **                       OpenScop Library                          **
     **-----------------------------------------------------------------**
     **                             int.h                               **
     **-----------------------------------------------------------------**
     **                   First version: 18/07/2011                     **
     **-----------------------------------------------------------------**

 
 *****************************************************************************
 * OpenScop: Structures and formats for polyhedral tools to talk together    *
 *****************************************************************************
 *    ,___,,_,__,,__,,__,,__,,_,__,,_,__,,__,,___,_,__,,_,__,                *
 *    /   / /  //  //  //  // /   / /  //  //   / /  // /  /|,_,             *
 *   /   / /  //  //  //  // /   / /  //  //   / /  // /  / / /\             *
 *  |~~~|~|~~~|~~~|~~~|~~~|~|~~~|~|~~~|~~~|~~~|~|~~~|~|~~~|/_/  \            *
 *  | G |C| P | = | L | P |=| = |C| = | = | = |=| = |=| C |\  \ /\           *
 *  | R |l| o | = | e | l |=| = |a| = | = | = |=| = |=| L | \# \ /\          *
 *  | A |a| l | = | t | u |=| = |n| = | = | = |=| = |=| o | |\# \  \         *
 *  | P |n| l | = | s | t |=| = |d| = | = | = | |   |=| o | | \# \  \        *
 *  | H | | y |   | e | o | | = |l|   |   | = | |   | | G | |  \  \  \       *
 *  | I | |   |   | e |   | |   | |   |   |   | |   | |   | |   \  \  \      *
 *  | T | |   |   |   |   | |   | |   |   |   | |   | |   | |    \  \  \     *
 *  | E | |   |   |   |   | |   | |   |   |   | |   | |   | |     \  \  \    *
 *  | * |*| * | * | * | * |*| * |*| * | * | * |*| * |*| * | /      \* \  \   *
 *  | O |p| e | n | S | c |o| p |-| L | i | b |r| a |r| y |/        \  \ /   *
 *  '---'-'---'---'---'---'-'---'-'---'---'---'-'---'-'---'          '--'    *
 *                                                                           *
 * Copyright (C) 2008 University Paris-Sud 11 and INRIA                      *
 *                                                                           *
 * (3-clause BSD license)                                                    *
 * Redistribution and use in source  and binary forms, with or without       *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 * 3. The name of the author may not be used to endorse or promote products  *
 *    derived from this software without specific prior written permission.  *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR      *
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.   *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,          *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT  *
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF  *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.         *
 *                                                                           *
 * OpenScop Library, a library to manipulate OpenScop formats and data       *
 * structures. Written by:                                                   *
 * Cedric Bastoul     <Cedric.Bastoul@u-psud.fr> and                         *
 * Louis-Noel Pouchet <Louis-Noel.pouchet@inria.fr>                          *
 *                                                                           *
 *****************************************************************************/


#ifndef OPENSCOP_INT_H
# define OPENSCOP_INT_H

# include <openscop/macros.h>
# ifdef OPENSCOP_GMP_IS_HERE
#  include <gmp.h>
# endif


typedef void * openscop_int_p;


/*+***************************************************************************
 *                                Basic Functions                            *
 *****************************************************************************/


void   openscop_int_dump_precision(FILE *, int);
int    openscop_int_sizeof(int);
void * openscop_int_address(int, void *, int);
void   openscop_int_init(int, void *, int);
void * openscop_int_malloc(int);
void   openscop_int_assign(int, void *, int, void *, int);
void   openscop_int_set_si(int, void *, int, int);
int    openscop_int_get_si(int, void *, int);
void   openscop_int_init_set_si(int, void *, int, int);
void   openscop_int_clear(int, void *, int);
void   openscop_int_free(int, void *, int);
void   openscop_int_print(FILE *, int, void *, int);
void   openscop_int_sprint(char *, int, void *, int);
void   openscop_int_sread(char *, int, void *, int);


/*+***************************************************************************
 *                            Arithmetic Operations                          *
 *****************************************************************************/


void   openscop_int_increment(int, void *, int, void *, int);
void   openscop_int_decrement(int, void *, int, void *, int);
void   openscop_int_add(int, void *, int, void *, int, void *, int);
void   openscop_int_add_ui(int, void *, int, void *, int, int);
void   openscop_int_mul(int, void *, int, void *, int, void *, int);
void   openscop_int_mul_si(int, void *, int, void *, int, int);
void   openscop_int_sub(int, void *, int, void *, int, void *, int);
void   openscop_int_oppose(int, void *, int, void *, int);


/*+***************************************************************************
 *                            Conditional Operations                         *
 *****************************************************************************/


int    openscop_int_eq(int, void *, int, void *, int);
int    openscop_int_ne(int, void *, int, void *, int);
int    openscop_int_pos(int, void *, int);
int    openscop_int_neg(int, void *, int);
int    openscop_int_zero(int, void *, int);
int    openscop_int_notzero(int, void *, int);
int    openscop_int_one(int, void *, int);
int    openscop_int_mone(int, void *, int);
int    openscop_int_divisible(int, void *, int, void *, int);


#endif /* define OPENSCOP_INT_H */