
    /*+-----------------------------------------------------------------**
     **                       OpenScop Library                          **
     **-----------------------------------------------------------------**
     **                          interface.c                            **
     **-----------------------------------------------------------------**
     **                   First version: 15/07/2011                     **
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

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <openscop/interface.h>


/*+***************************************************************************
 *                          Structure display function                       *
 *****************************************************************************/


/**
 * openscop_interface_idump function:
 * this function displays an openscop_interface_t structure (*interface) into
 * a file (file, possibly stdout) in a way that trends to be understandable.
 * It includes an indentation level (level) in order to work with others
 * print_structure functions.
 * \param file      The file where the information has to be printed.
 * \param interface The interface structure which has to be printed.
 * \param level     Number of spaces before printing, for each line.
 */
void openscop_interface_idump(FILE * file, openscop_interface_p interface,
                              int level) {
  int j, first = 1;

  // Go to the right level.
  for (j = 0; j < level; j++)
    fprintf(file, "|\t");

  if (interface != NULL)
    fprintf(file, "+-- openscop_interface_t: URI = %s\n", interface->URI);
  else
    fprintf(file, "+-- NULL interface\n");

  
  while (interface != NULL) {
    if (!first) {
      // Go to the right level.
      for (j = 0; j < level; j++)
        fprintf(file, "|\t");
      fprintf(file, "|   openscop_interface_t: URI = %s\n", interface->URI);
    }
    else
      first = 0;

    interface = interface->next;

    // Next line.
    if (interface != NULL) {
      for (j = 0; j <= level + 1; j++)
        fprintf(file, "|\t");
      fprintf(file, "\n");
      for (j = 0; j <= level; j++)
        fprintf(file, "|\t");
      fprintf(file, "V\n");
    }
  }
  
  // The last line.
  for (j = 0; j <= level; j++)
    fprintf(file, "|\t");
  fprintf(file, "\n");
}


/**
 * openscop_interface_dump function:
 * this function prints the content of a openscop_interface_t structure
 * (*interface) into a file (file, possibly stdout).
 * \param file  File where informations are printed.
 * \param extension The extension idstructure to print.
 */
void openscop_interface_dump(FILE * file, openscop_interface_p interface) {
  openscop_interface_idump(file, interface, 0); 
}


/*****************************************************************************
 *                               Reading function                            *
 *****************************************************************************/


/*+***************************************************************************
 *                    Memory allocation/deallocation function                *
 *****************************************************************************/


/**
 * openscop_interface_add function:
 * this function adds an interface node (it may be a list as well) to a
 * list of interfaces provided as parameter (list). The new node
 * is inserted at the end of the list. 
 * \param list      The list of interfaces to add a node (NULL if empty).
 * \param interface The interface to add to the list.
 */
void openscop_interface_add(openscop_interface_p * list,
                            openscop_interface_p interface) {
  openscop_interface_p tmp = *list, check_interface;

  if (interface != NULL) {
    // First, check that the interface list is OK.
    check_interface = interface;
    while (check_interface != NULL) {
      if (check_interface->URI == NULL)
        OPENSCOP_error("no URI in an interface to add to a list");

      if (openscop_interface_lookup(*list, check_interface->URI) != NULL)
        OPENSCOP_error("only one interface with a given URI is allowed");
      check_interface = check_interface->next;
    }

    if (*list != NULL) {
      while (tmp->next != NULL)
        tmp = tmp->next;
      tmp->next = interface;
    }
    else {
      *list = interface;
    }
  }
}


/**
 * openscop_interface_malloc function:
 * This function allocates the memory space for a openscop_interface_t
 * structure and sets its fields with default values. Then it returns a
 * pointer to the allocated space.
 * \return A pointer to an empty interface structure with fields set to
 *         default values.
 */
openscop_interface_p openscop_interface_malloc() {
  openscop_interface_p interface;

  OPENSCOP_malloc(interface, openscop_interface_p,
                  sizeof(openscop_interface_t));
  interface->URI    = NULL;
  interface->idump  = NULL; 
  interface->dump   = NULL; 
  interface->sprint = NULL; 
  interface->sread  = NULL; 
  interface->malloc = NULL; 
  interface->free   = NULL; 
  interface->clone  = NULL; 
  interface->equal  = NULL;
  interface->next   = NULL;

  return interface;
}


/**
 * openscop_interface_free function:
 * this function frees the allocated memory for an openscop_interface_t
 * structure, and all the interfaces stored in the list.
 * \param[in] id The pointer to the interface we want to free.
 */
void openscop_interface_free(openscop_interface_p interface) {
  openscop_interface_p tmp;
  int i = 0;
 
  if (interface == NULL)
    return;

  while (interface != NULL) {
    tmp = interface->next;
    if (interface->URI != NULL)
      free(interface->URI);
    free(interface);
    interface = tmp;
    i++;
  }
}


/*+***************************************************************************
 *                            Processing functions                           *
 *****************************************************************************/


/**
 * openscop_interface_nclone function:
 * This function builds and returns a "hard copy" (not a pointer copy) of the
 * n first elements of an openscop_interface_t list.
 * \param interface The pointer to the interface structure we want to clone.
 * \param n         The number of nodes we want to copy (-1 for infinity).
 * \return The clone of the n first nodes of the interface list.
 */
openscop_interface_p openscop_interface_nclone(openscop_interface_p interface,
                                               int n) {
  openscop_interface_p clone = NULL, new;
  int i = 0;

  while ((interface != NULL) && ((n == -1) || (i < n))) {
    new = openscop_interface_malloc();
    OPENSCOP_strdup(new->URI, interface->URI);
    new->idump  = interface->idump;
    new->dump   = interface->dump;
    new->sprint = interface->sprint;
    new->sread  = interface->sread;
    new->malloc = interface->malloc;
    new->free   = interface->free;
    new->clone  = interface->clone;
    new->equal  = interface->equal;
    
    openscop_interface_add(&clone, new);
    interface = interface->next;
    i++;
  }

  return clone;
}


/**
 * openscop_interface_clone function:
 * This function builds and returns a "hard copy" (not a pointer copy) of an
 * openscop_interface_t data structure.
 * \param interface The pointer to the interface structure we want to copy.
 * \return A pointer to the copy of the interface structure.
 */
openscop_interface_p
openscop_interface_clone(openscop_interface_p interface) {

  return openscop_interface_nclone(interface, -1);
}


/**
 * openscop_interface_equal function:
 * this function returns true if the two interface structures are the same,
 * (content-wise) false otherwise. 
 * \param interface1 The first interface structure.
 * \param interface2 The second interface structure.
 * \return 1 if interface1 and interface2 are the same, 0 otherwise.
 */
int openscop_interface_equal(openscop_interface_p interface1,
                             openscop_interface_p interface2) {

  if (interface1 == interface2)
    return 1;
  
  if (((interface1 == NULL) && (interface2 != NULL)) ||
      ((interface1 != NULL) && (interface2 == NULL)))
    return 0;

  if (strcmp(interface1->URI, interface2->URI)   ||
      (interface1->idump  != interface2->idump)  ||
      (interface1->dump   != interface2->dump)   ||
      (interface1->sprint != interface2->sprint) ||
      (interface1->sread  != interface2->sread)  ||
      (interface1->malloc != interface2->malloc) ||
      (interface1->free   != interface2->free)   ||
      (interface1->clone  != interface2->clone)  ||
      (interface1->equal  != interface2->equal))
    return 0;

  return 1;
}


/**
 * openscop_interface_lookup function:
 * this function returns the first interface with a given URI in the
 * interface list provided as parameter and NULL if it doesn't find such
 * an interface.
 * \param list The interface list where to search a given interface URI.
 * \param URI  The URI of the interface we are looking for.
 * \return The first interface of the requested URI in the list.
 */
openscop_interface_p
openscop_interface_lookup(openscop_interface_p list, char * URI) {
  while (list != NULL) {
    if ((list->URI != NULL) && (!strcmp(list->URI, URI)))
      return list;

    list = list->next;
  }

  return NULL;
}
