/******************************************************************************
 **                            OpenScop Library                              **
 **--------------------------------------------------------------------------**
 **                           extensions/region.c                            **
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

#include "osl/extensions/region.h"
#include "osl/macros.h"

/******************************************************************************
 * Local functions declarations                                               *
 ******************************************************************************/

static inline void osl_region_idump_indent(FILE* const file, int level);

/**
 * \brief Get a region's annotations.
 * \param[in, out] region The targetted region.
 * \return The region's annotations.
 *
 * \note If the region has not been annotated yet, this function first creates
 * an osl_annotation and stores it in the region's extension list.
 *
 * \return The region's annotation.
 */
static inline osl_annotation* osl_region_get_annotation(osl_region* region);

/******************************************************************************
 * Local variables                                                            *
 ******************************************************************************/

static const char* _region_location_strings[] = {
  [OSL_REGION_NONE] = "none",
  [OSL_REGION_GLOBAL] = "global",
};

/******************************************************************************
 * Structure display functions                                                *
 ******************************************************************************/

void osl_region_idump_indent(FILE* const file, int level) {
  for (int j = 0; j < level; ++j) {
    fprintf(file, "|\t");
  }
}

void osl_region_idump(FILE* const file, const osl_region* region, int level) {
  /* Region header. */
  osl_region_idump_indent(file, level);
  fprintf(file, "+-- %s\n", region ? "osl_region" : "NULL region");
  /* Blank line... */
  osl_region_idump_indent(file, level + 2);
  fprintf(file, "\n");

  bool first = true;
  size_t count = 1;
  while (region) {
    /* Region counter, if needed. */
    if (!first) {
      osl_region_idump_indent(file, level);
      fprintf(file, "|   osl_region (node %zu)\n", count);
      /* Blank line... */
      osl_region_idump_indent(file, level + 2);
      fprintf(file, "\n");
    } else {
      first = 0;
    }
    /* Location */
    osl_region_idump_indent(file, level + 1);
    fprintf(file, "+--location: %d (%s)\n", region->location,
            _region_location_strings[region->location]);
    /* Blank line... */
    osl_region_idump_indent(file, level + 2);
    fprintf(file, "\n");
    /* Extensions */
    osl_generic_idump(file, region->extensions, level + 1);
    ++count;
    region = region->next;
  }

  /* Last line. */
  osl_region_idump_indent(file, level);
  fprintf(file, "\n");
}

void osl_region_dump(FILE* const file, const osl_region* region) {
  osl_region_idump(file, region, 0);
}

char* osl_region_sprint(const osl_region* region) {
  const size_t count = osl_region_count(region);

  char buffer[OSL_MAX_STRING];
  char* string = NULL;
  size_t step = 0;
  size_t high_water_mark = OSL_MAX_STRING;

  OSL_malloc(string, char*, high_water_mark * sizeof(char));
  string[0] = '\0';

  sprintf(buffer, "# Number of regions\n%d\n", (int)count);
  osl_util_safe_strcat(&string, buffer, &high_water_mark);

  while (region) {
    sprintf(buffer, "# ===========================================\n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    sprintf(buffer, "# Region number %d / %d \n", (int)++step, (int)count);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    sprintf(buffer, "# Location \n");
    osl_util_safe_strcat(&string, buffer, &high_water_mark);
    sprintf(buffer, "%d\n", region->location);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    const int extension_count = osl_generic_number(region->extensions);
    sprintf(buffer, "# Number of extensions \n%d\n", extension_count);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);

    char* const extensions_string = osl_generic_sprint(region->extensions);
    sprintf(buffer, "%s\n", extensions_string);
    osl_util_safe_strcat(&string, buffer, &high_water_mark);
    free(extensions_string);

    region = region->next;
  }

  OSL_realloc(string, char*, (strlen(string) + 1) * sizeof(char));
  return string;
}

/******************************************************************************
 * Reading functions                                                          *
 ******************************************************************************/

osl_region* osl_region_sread(char** input) {
  if (!input) {
    OSL_debug("no region optional tag");
    return NULL;
  }

  const int region_count = osl_util_read_int(NULL, input);
  if (!region_count)
    return NULL;

  osl_region* const output = osl_region_malloc();
  osl_region* current = output;
  for (int i = 0; i < region_count; ++i) {
    current->location = osl_util_read_int(NULL, input);

    const int extension_count = osl_util_read_int(NULL, input);
    osl_interface* const registry = osl_interface_get_default_registry();
    for (int j = 0; j < extension_count; ++j) {
      osl_generic* const extension = osl_generic_sread_one(input, registry);
      osl_generic_add(&current->extensions, extension);
    }
    osl_interface_free(registry);

    if (i + 1 < region_count) {
      current->next = osl_region_malloc();
      current = current->next;
    }
  }

  return output;
}

/******************************************************************************
 * Memory allocation/deallocation functions                                   *
 ******************************************************************************/

osl_region* osl_region_malloc(void) {
  osl_region* const region = malloc(sizeof *region);
  if (!region) {
    fprintf(stderr, "[osl] Error: memory overflow (%s).\n", __func__);
    exit(1);
  }

  region->location = 0;
  region->extensions = 0;
  region->next = NULL;

  return region;
}

void osl_region_free(osl_region* region) {
  while (region) {
    osl_region* const tmp = region;

    /* Free extensions. */
    osl_generic_free(region->extensions);

    /* Move along. */
    region = region->next;

    /* Do not forget to free the current region. */
    free(tmp);
  }
}

/******************************************************************************
 * Processing functions                            *
 ******************************************************************************/

osl_region* osl_region_clone_one(const osl_region* source) {
  osl_region* clone = osl_region_malloc();

  clone->location = source->location;
  clone->extensions = osl_generic_clone(source->extensions);

  return clone;
}

osl_region* osl_region_clone(const osl_region* source) {
  if (!source)
    return NULL;

  /* Clone the first region. */
  osl_region* const clone = osl_region_clone_one(source);
  osl_region* last = clone;

  /* Clone the remaining regions. */
  const osl_region* current = source->next;
  while (current) {
    last->next = osl_region_clone_one(current);
    last = last->next;
    current = current->next;
  }

  return clone;
}

bool osl_region_equal_one(const osl_region* r1, const osl_region* r2) {
  if (r1 == r2)
    return 1;

  if ((!r1 && r2) || (r1 && !r2))
    return 0;

  /* Both r1 and r2 are non null pointers at this point. */
  bool equal = r1->location == r2->location &&
               osl_generic_equal(r1->extensions, r2->extensions);

  return equal;
}

bool osl_region_equal(const osl_region* r1, const osl_region* r2) {
  bool equal = osl_region_equal_one(r1, r2);

  if (equal)
    return osl_region_equal(r1->next, r2->next);
  else
    return equal;
}

size_t osl_region_count(const osl_region* region) {
  size_t count = 0;
  for (const osl_region* current = region; current; current = current->next)
    ++count;

  return count;
}

void osl_region_append_prefix(osl_region* const region, const int prefix_type,
                              char* const prefix) {
  osl_annotation* const annotation = osl_region_get_annotation(region);
  if (annotation) {
    osl_annotation_append_prefix(annotation, prefix_type, prefix);
  }
}

void osl_region_append_suffix(osl_region* const region, const int suffix_type,
                              char* const suffix) {
  osl_annotation* const annotation = osl_region_get_annotation(region);
  if (annotation) {
    osl_annotation_append_suffix(annotation, suffix_type, suffix);
  }
}

void osl_region_append_prelude(osl_region* const region, const int prelude_type,
                               char* const prelude) {
  osl_annotation* const annotation = osl_region_get_annotation(region);
  if (annotation) {
    osl_annotation_append_prelude(annotation, prelude_type, prelude);
  }
}

void osl_region_append_postlude(osl_region* const region, const int postlude_type,
                                char* const postlude) {
  osl_annotation* const annotation = osl_region_get_annotation(region);
  if (annotation) {
    osl_annotation_append_postlude(annotation, postlude_type, postlude);
  }
}


osl_interface* osl_region_interface(void) {
  osl_interface* const interface = osl_interface_malloc();

  OSL_strdup(interface->URI, OSL_URI_REGION);

  interface->idump = (osl_idump_f)osl_region_idump;
  interface->sprint = (osl_sprint_f)osl_region_sprint;
  interface->sread = (osl_sread_f)osl_region_sread;
  interface->malloc = (osl_malloc_f)osl_region_malloc;
  interface->free = (osl_free_f)osl_region_free;
  interface->clone = (osl_clone_f)osl_region_clone;
  interface->equal = (osl_equal_f)osl_region_equal;

  return interface;
}

/******************************************************************************
 * Local functions definitions                                                *
 ******************************************************************************/

osl_annotation* osl_region_get_annotation(osl_region* const region) {
  osl_annotation* const existing =  osl_generic_lookup(region->extensions, OSL_URI_ANNOTATION);
  if (existing)
    return existing;

  /* Create a new annotation. */
  osl_annotation* const annotation = osl_annotation_malloc();

  /* Embed the annotation in an osl_generic. */
  osl_generic* const container = osl_generic_malloc();
  container->interface = osl_annotation_interface();
  container->data = annotation;

  /* Add the annotation in the extensions list in the region. */
  osl_generic_add(&region->extensions, container);

  return annotation;
}

