/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                         extensions/dependence.c                          **
 **--------------------------------------------------------------------------**
 **                        First version: 02/07/2012                         **
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
#include <osl/relation.h>
#include <osl/scop.h>
#include <osl/statement.h>
#include <osl/util.h>

#include <osl/extensions/dependence.h>

/**
 * Most of these functions where extracted from candl and ported to osl
 */

/*******************************************************************************
 *                          Structure display function                         *
 *******************************************************************************/

/**
 * osl_dependence_idump function:
 * Displays a osl_dependence* structure (dependence) into a file (file,
 * possibly stdout) in a way that trends to be understandable without falling
 * in a deep depression or, for the lucky ones, getting a headache... It
 * includes an indentation level (level) in order to work with others
 * idump functions.
 * - 18/09/2003: first version.
 */
void osl_dependence_idump(FILE* const file, const osl_dependence* dependence,
                          const int level) {
  bool first = true;
  osl_statement* tmp;

  osl_util_idump_indent(file, level);
  fprintf(file, "+-- %s\n", dependence ? "osl_dependence" : "NULL dependence");

  while (dependence) {
    if (!first) { /* Go to the right level. */
      osl_util_idump_indent(file, level);
      fprintf(file, "|   osl_dependence\n");
    } else {
      first = 0;
    }

    /* A blank line. */
    osl_util_idump_blank_line(file, level + 2);
    /* Go to the right level and print the type. */
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "Type: ");
    switch (dependence->type) {
      case OSL_UNDEFINED:
        fprintf(file, "UNSET\n");
        break;
      case OSL_DEPENDENCE_RAW:
        fprintf(file, "RAW (flow)\n");
        break;
      case OSL_DEPENDENCE_WAR:
        fprintf(file, "WAR (anti)\n");
        break;
      case OSL_DEPENDENCE_WAW:
        fprintf(file, "WAW (output)\n");
        break;
      case OSL_DEPENDENCE_RAR:
        fprintf(file, "RAR (input)\n");
        break;
      case OSL_DEPENDENCE_RAW_SCALPRIV:
        fprintf(file, "RAW_SCALPRIV (scalar priv)\n");
        break;
      default:
        fprintf(file, "unknown\n");
        break;
    }

    /* A blank line. */
    osl_util_idump_blank_line(file, level + 2);
    /* Go to the right level and print the depth. */
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "Depth: %d\n", dependence->depth);

    /* A blank line. */
    osl_util_idump_blank_line(file, level + 2);
    /* Ref source and target */
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "Ref source: %d, Ref target: %d\n", dependence->ref_source,
            dependence->ref_target);

    /* A blank line. */
    osl_util_idump_blank_line(file, level + 2);

    /* Print the source statement. */
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "Statement label: %d\n", dependence->label_source);
    tmp = dependence->stmt_source_ptr->next;
    dependence->stmt_source_ptr->next = NULL;
    osl_statement_idump(file, dependence->stmt_source_ptr, level + 1);
    dependence->stmt_source_ptr->next = tmp;

    /* Print the target statement. */
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "Target label: %d\n", dependence->label_target);
    tmp = dependence->stmt_target_ptr->next;
    dependence->stmt_target_ptr->next = NULL;
    osl_statement_idump(file, dependence->stmt_target_ptr, level + 1);
    dependence->stmt_target_ptr->next = tmp;

    /* Print the dependence polyhedron. */
    osl_util_idump_indent(file, level + 1);
    fprintf(file, "%d %d %d %d %d %d %d %d\n",
            dependence->source_nb_output_dims_domain,
            dependence->source_nb_output_dims_access,
            dependence->target_nb_output_dims_domain,
            dependence->target_nb_output_dims_access,
            dependence->source_nb_local_dims_domain,
            dependence->source_nb_local_dims_access,
            dependence->target_nb_local_dims_domain,
            dependence->target_nb_local_dims_access);
    osl_relation_idump(file, dependence->domain, level + 1);

    dependence = dependence->next;

    /* Next line. */
    if (dependence)
      osl_util_idump_next_link(file, level + 1);
  }

  /* The last line. */
  osl_util_idump_blank_line(file, level + 1);
}

/**
 * osl_dependence_dump function:
 * This function prints the content of a osl_dependence* structure (dependence)
 * into a file (file, possibly stdout).
 */
void osl_dependence_dump(FILE* const file,
                         const osl_dependence* const dependence) {
  osl_dependence_idump(file, dependence, 0);
}

/**
 * osl_dependence_print function:
 * Print the dependence, formatted to fit the .scop representation.
 */
void osl_dependence_print(FILE* const file,
                          const osl_dependence* const dependence) {
  char* const string = osl_dependence_sprint(dependence);
  fprintf(file, "%s\n", string);
  free(string);
}

/**
 * osl_dependence_sprint function:
 * Returns a string containing the dependence, formatted to fit the
 * .scop representation.
 */
char* osl_dependence_sprint(const osl_dependence* const dependence) {
  const osl_dependence* tmp = dependence;
  int nb_deps;
  size_t buffer_size = 2048;
  char* buffer;
  char buff[2048];

  OSL_malloc(buffer, char*, buffer_size);
  buffer[0] = '\0';

  for (tmp = dependence, nb_deps = 0; tmp; tmp = tmp->next, ++nb_deps)
    ;
  snprintf(buff, OSL_MAX_STRING, "# Number of dependences\n%d\n", nb_deps);
  strcat(buffer, buff);

  if (nb_deps) {
    const char* type;
    for (tmp = dependence, nb_deps = 1; tmp; tmp = tmp->next, ++nb_deps) {
      switch (tmp->type) {
        case OSL_UNDEFINED:
          type = "UNSET";
          break;
        case OSL_DEPENDENCE_RAW:
          type = "RAW #(flow)";
          break;
        case OSL_DEPENDENCE_WAR:
          type = "WAR #(anti)";
          break;
        case OSL_DEPENDENCE_WAW:
          type = "WAW #(output)";
          break;
        case OSL_DEPENDENCE_RAR:
          type = "RAR #(input)";
          break;
        case OSL_DEPENDENCE_RAW_SCALPRIV:
          type = "RAW_SCALPRIV #(scalar priv)";
          break;
        default:
          exit(1);
          break;
      }

      /* Output dependence information. */
      snprintf(buff, OSL_MAX_STRING,
               "# Description of dependence %d\n"
               "# type\n%s\n"
               "# From source statement id\n%d\n"
               "# To target statement id\n%d\n"
               "# Depth \n%d\n"
               "# From source access ref\n%d\n"
               "# To target access ref\n%d\n"
               "# Dependence domain\n",
               nb_deps, type, tmp->label_source, tmp->label_target, tmp->depth,
               tmp->ref_source, tmp->ref_target);

      osl_util_safe_strcat(&buffer, buff, &buffer_size);

      /* Output dependence domain. */
      char* const pbuffer = osl_relation_sprint(tmp->domain);
      osl_util_safe_strcat(&buffer, pbuffer, &buffer_size);
      free(pbuffer);
    }
  }

  return buffer;
}

/**
 * osl_dependence_read_one_dep function:
 * Read one dependence from a string.
 */
static osl_dependence* osl_dependence_read_one_dep(char** const input,
                                                   const int precision) {
  osl_dependence* const dep = osl_dependence_malloc();

  /* Dependence type */
  char* const buffer = osl_util_read_string(NULL, input);
  if (!strcmp(buffer, "RAW"))
    dep->type = OSL_DEPENDENCE_RAW;
  else if (!strcmp(buffer, "RAR"))
    dep->type = OSL_DEPENDENCE_RAR;
  else if (!strcmp(buffer, "WAR"))
    dep->type = OSL_DEPENDENCE_WAR;
  else if (!strcmp(buffer, "WAW"))
    dep->type = OSL_DEPENDENCE_WAW;
  else if (!strcmp(buffer, "RAW_SCALPRIV"))
    dep->type = OSL_DEPENDENCE_RAW_SCALPRIV;
  free(buffer);

  /* # From source statement xxx */
  dep->label_source = osl_util_read_int(NULL, input);

  /* # To target statement xxx */
  dep->label_target = osl_util_read_int(NULL, input);

  /* # Depth */
  dep->depth = osl_util_read_int(NULL, input);

  /* # From source access ref */
  dep->ref_source = osl_util_read_int(NULL, input);

  /* # To target access ref */
  dep->ref_target = osl_util_read_int(NULL, input);

  /* Read the osl_relation */
  dep->domain = osl_relation_psread(input, precision);

  return dep;
}

/**
 * osl_dependence_sread function:
 * Retrieve a osl_dependence* list from the option tag in the scop.
 */
osl_dependence* osl_dependence_sread(char** const input) {
  const int precision = osl_util_get_precision();
  return osl_dependence_psread(input, precision);
}

/**
 * osl_dependence_psread function
 * Retrieve a osl_dependence* list from the option tag in the scop.
 */
osl_dependence* osl_dependence_psread(char** const input, const int precision) {
  osl_dependence* first = NULL;
  osl_dependence* currdep = NULL;

  if (!*input) {
    OSL_debug("no dependence optional tag");
    return NULL;
  }

  int i;
  /* Get the number of dependences. */
  int nbdeps = osl_util_read_int(NULL, input);

  /* For each of them, read 1 and shift of the read size. */
  for (i = 0; i < nbdeps; i++) {
    osl_dependence* adep = osl_dependence_read_one_dep(input, precision);
    if (!first) {
      currdep = first = adep;
    } else {
      currdep->next = adep;
      currdep = currdep->next;
    }
  }

  return first;
}

/*******************************************************************************
 *                         Memory deallocation function                        *
 *******************************************************************************/

/**
 * osl_dependence_malloc function:
 * This function allocates the memory space for a osl_dependence* structure and
 * sets its fields with default values. Then it returns a pointer to the
 * allocated space.
 * - 07/12/2005: first version.
 */
osl_dependence* osl_dependence_malloc(void) {
  osl_dependence* dependence;

  /* Memory allocation for the osl_dependence* structure. */
  OSL_malloc(dependence, osl_dependence*, sizeof(osl_dependence));

  /* We set the various fields with default values. */
  dependence->depth = OSL_UNDEFINED;
  dependence->type = OSL_UNDEFINED;
  dependence->label_source = OSL_UNDEFINED;
  dependence->label_target = OSL_UNDEFINED;
  dependence->ref_source = OSL_UNDEFINED;
  dependence->ref_target = OSL_UNDEFINED;
  dependence->domain = NULL;
  dependence->next = NULL;
  dependence->usr = NULL;
  dependence->source_nb_output_dims_domain = OSL_UNDEFINED;
  dependence->source_nb_output_dims_access = OSL_UNDEFINED;
  dependence->target_nb_output_dims_domain = OSL_UNDEFINED;
  dependence->target_nb_output_dims_access = OSL_UNDEFINED;
  dependence->source_nb_local_dims_domain = OSL_UNDEFINED;
  dependence->source_nb_local_dims_access = OSL_UNDEFINED;
  dependence->target_nb_local_dims_domain = OSL_UNDEFINED;
  dependence->target_nb_local_dims_access = OSL_UNDEFINED;
  dependence->ref_source_access_ptr = NULL;
  dependence->ref_target_access_ptr = NULL;
  dependence->stmt_source_ptr = NULL;
  dependence->stmt_target_ptr = NULL;

  return dependence;
}

/**
 * osl_dependence_free function:
 * This function frees the allocated memory for a osl_dependence* structure.
 * - 18/09/2003: first version.
 */
void osl_dependence_free(osl_dependence* dependence) {
  osl_dependence* next;
  while (dependence) {
    next = dependence->next;
    osl_relation_free(dependence->domain);
    free(dependence);
    dependence = next;
  }
}

/*******************************************************************************
 *                            Processing functions                             *
 *******************************************************************************/

/**
 * osl_dependence_nclone function:
 * This function builds and returns a "hard copy" (not a pointer copy) of the
 * n first elements of an osl_dependence list.
 * \param statement The pointer to the dependence structure we want to clone.
 * \param n         The number of nodes we want to copy (-1 for infinity).
 * \return The clone of the n first nodes of the dependence list.
 */
static osl_dependence* osl_dependence_nclone(const osl_dependence* dep, int n) {
  bool first = true;
  int i = 0;
  osl_dependence* clone = NULL;
  osl_dependence* node;
  osl_dependence* previous = NULL;

  while (dep && ((n == -1) || (i < n))) {
    node = osl_dependence_malloc();
    node->stmt_source_ptr = dep->stmt_source_ptr;
    node->stmt_target_ptr = dep->stmt_target_ptr;
    node->depth = dep->depth;
    node->type = dep->type;
    node->label_source = dep->label_source;
    node->label_target = dep->label_target;
    node->ref_source = dep->ref_source;
    node->ref_target = dep->ref_target;
    node->domain = osl_relation_clone(dep->domain);
    node->source_nb_output_dims_domain = dep->source_nb_output_dims_domain;
    node->source_nb_output_dims_access = dep->source_nb_output_dims_access;
    node->target_nb_output_dims_domain = dep->target_nb_output_dims_domain;
    node->target_nb_output_dims_access = dep->target_nb_output_dims_access;
    node->source_nb_local_dims_domain = dep->source_nb_local_dims_domain;
    node->source_nb_local_dims_access = dep->source_nb_local_dims_access;
    node->target_nb_local_dims_domain = dep->target_nb_local_dims_domain;
    node->target_nb_local_dims_access = dep->target_nb_local_dims_access;
    node->next = NULL;
    node->usr = NULL;

    if (first) {
      first = false;
      clone = node;
      previous = node;
    } else {
      previous->next = node;
      previous = previous->next;
    }

    i++;
    dep = dep->next;
  }

  return clone;
}

/**
 * osl_dependence_clone function:
 * This functions builds and returns a "hard copy" (not a pointer copy) of an
 * osl_dependence data structure provided as parameter.
 * \param[in] statement The pointer to the dependence we want to clone.
 * \return A pointer to the clone of the dependence provided as parameter.
 */
osl_dependence* osl_dependence_clone(const osl_dependence* const dep) {
  return osl_dependence_nclone(dep, -1);
}

/**
 * osl_dependence_equal function:
 * this function returns true if the two dependences provided as parameters
 * are the same, false otherwise (the usr field is not tested).
 * NOTE: the different pointer to statements or relations are nto compared
 * \param[in] d1 The first dependence.
 * \param[in] d2 The second dependence.
 * \return 1 if d1 and d2 are the same (content-wise), 0 otherwise.
 */
bool osl_dependence_equal(const osl_dependence* d1, const osl_dependence* d2) {
  if (d1 == d2)
    return true;

  if ((d1->next && !d2->next) || (!d1->next && d2->next))
    return false;

  if (d1->next && d2->next)
    if (!osl_dependence_equal(d1->next, d2->next))
      return false;

  if (!osl_relation_equal(d1->domain, d2->domain))
    return false;

  if (d1->label_source != d2->label_source ||
      d1->label_target != d2->label_target ||
      d1->ref_source != d2->ref_source || d1->ref_target != d2->ref_target ||
      d1->depth != d2->depth || d1->type != d2->type ||
      d1->source_nb_output_dims_domain != d2->source_nb_output_dims_domain ||
      d1->source_nb_output_dims_access != d2->source_nb_output_dims_access ||
      d1->target_nb_output_dims_domain != d2->target_nb_output_dims_domain ||
      d1->target_nb_output_dims_access != d2->target_nb_output_dims_access ||
      d1->source_nb_local_dims_domain != d2->source_nb_local_dims_domain ||
      d1->source_nb_local_dims_access != d2->source_nb_local_dims_access ||
      d1->target_nb_local_dims_domain != d2->target_nb_local_dims_domain ||
      d1->target_nb_local_dims_access != d2->target_nb_local_dims_access)
    return false;

  return true;
}

/**
 * osl_dependence_add function:
 * This function adds a osl_dependence* structure (dependence) at a given place
 * (now) of a NULL terminated list of osl_dependence* structures. The beginning
 * of this list is (start). This function updates (now) to the end of the loop
 * list (loop), and updates (start) if the added element is the first one -that
 * is when (start) is NULL-.
 * - 18/09/2003: first version.
 */
void osl_dependence_add(osl_dependence** start, osl_dependence** now,
                        osl_dependence* dependence) {
  if (dependence) {
    if (!*start) {
      *start = dependence;
      *now = *start;
    } else {
      (*now)->next = dependence;
      *now = (*now)->next;
    }

    while ((*now)->next)
      *now = (*now)->next;
  }
}

/**
 * osl_nb_dependences function:
 * This function returns the number of dependences in the dependence
 * list
 * \param dependence The first dependence of the dependence list.
 **
 */
int osl_nb_dependences(const osl_dependence* deps) {
  const osl_dependence* dep = deps;
  int num = 0;
  while (dep) {
    num++;
    dep = dep->next;
  }
  return num;
}

/**
 * osl_dependence_interface function:
 * this function creates an interface structure corresponding to the dependence
 * extension and returns it).
 * \return An interface structure for the dependence extension.
 */
osl_interface* osl_dependence_interface(void) {
  osl_interface* const interface = osl_interface_malloc();

  OSL_strdup(interface->URI, OSL_URI_DEPENDENCE);
  interface->idump = (osl_idump_f)osl_dependence_idump;
  interface->sprint = (osl_sprint_f)osl_dependence_sprint;
  interface->sread = (osl_sread_f)osl_dependence_sread;
  interface->malloc = (osl_malloc_f)osl_dependence_malloc;
  interface->free = (osl_free_f)osl_dependence_free;
  interface->clone = (osl_clone_f)osl_dependence_clone;
  interface->equal = (osl_equal_f)osl_dependence_equal;

  return interface;
}
