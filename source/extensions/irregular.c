
    /*+-----------------------------------------------------------------**
     **                       OpenScop Library                          **
     **-----------------------------------------------------------------**
     **                     extensions/irregular.c                        **
     **-----------------------------------------------------------------**
     **                   First version: 07/12/2010                     **
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
# include <openscop/extension.h>


/*+***************************************************************************
 *                          Structure display function                       *
 *****************************************************************************/


/**
 * openscop_irregular_print_structure function:
 * this function displays an openscop_irregular_t structure (*irregular) into a
 * file (file, possibly stdout) in a way that trends to be understandable. It
 * includes an indentation level (level) in order to work with others
 * print_structure functions.
 * \param file    The file where the information has to be printed.
 * \param irregular The irregular structure whose information has to be printed.
 * \param level   Number of spaces before printing, for each line.
 */
void
openscop_irregular_print_structure(FILE * file, openscop_irregular_p irregular,
                                   int level)
{
  int i,j;

  // Go to the right level.
  for (j = 0; j < level; j++)
    fprintf(file, "|\t");

  if (irregular != NULL)
    fprintf(file, "+-- openscop_irregular_t\n");
  else
    fprintf(file, "+-- NULL irregular\n");

  if (irregular != NULL)
  {
    // Go to the right level.
    for(j = 0; j <= level; j++)
      fprintf(file, "|\t");
  
    // Display the irregular contents.

    // Print statements
    for (i=0; i<irregular->nb_statements; i++)
    {
      fprintf(file, "statement%d's predicats : ", i);
      for(j=0; j<irregular->nb_predicates[i]; j++)
        fprintf(file, "%d ", irregular->predicates[i][j]);
      fprintf(file, "\n");
    }
    // Print predicats
    for (i=0; i<irregular->nb_control; i++)
    {
      fprintf(file, "predicat%d's\niterators : ", i);
      for(j=0; j<irregular->nb_iterators[i]; j++)
        fprintf(file, "%s ", irregular->iterators[i][j]);
      fprintf(file, "\nbody: %s\n", irregular->body[i]);
    }
  }

  // The last line.
  for (j = 0; j <= level; j++)
    fprintf(file, "|\t");
  fprintf(file, "\n");
}


/**
 * openscop_irregular_print function:
 * this function prints the content of an openscop_irregular_t structure
 * (*irregular) into a file (file, possibly stdout).
 * \param file    The file where the information has to be printed.
 * \param irregular The irregular structure whose information has to be printed.
 */
void
openscop_irregular_print(FILE * file, openscop_irregular_p irregular)
{
  openscop_irregular_print_structure(file, irregular, 0);
}


/**
 * openscop_irregular_print_openscop function:
 * this function prints the content of an openscop_irregular_t structure
 * (*irregular) into a string (returned) in the OpenScop textual format.
 * \param  irregular The irregular structure whose information has to be printed.
 * \return A string containing the OpenScop dump of the irregular structure.
 */
char *
openscop_irregular_print_openscop(openscop_irregular_p irregular)
{
  int high_water_mark = OPENSCOP_MAX_STRING,i,j;
  char * string = NULL;
  char * buffer;

  if (irregular != NULL)
  {
    string = (char *)malloc(high_water_mark * sizeof(char));
    buffer = (char *)malloc(OPENSCOP_MAX_STRING * sizeof(char));
    if ((string == NULL) || (buffer == NULL))
    {
      fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
      exit(1);
    }
    string[0] = '\0';
   
    // Print the begin tag.
    sprintf(buffer, OPENSCOP_TAG_IRREGULAR_START);
    openscop_util_safe_strcat(&string, buffer, &high_water_mark);

    // Print the content.
    sprintf(buffer, "\n%d\n", irregular->nb_statements);
    for(i=0; i<irregular->nb_statements; i++)
    {
      sprintf(buffer, "%s%d ", buffer, irregular->nb_predicates[i]);
      for(j=0; j<irregular->nb_predicates[i]; j++)
      {
        sprintf(buffer, "%s%d ", buffer, irregular->predicates[i][j]);
      }
      sprintf(buffer, "%s\n", buffer);
    }
    // Print the predicats.
    sprintf(buffer, "%s%d\n", buffer, irregular->nb_control);
    for(i=0; i<irregular->nb_control; i++)
    {
      sprintf(buffer, "%s%d ", buffer, irregular->nb_iterators[i]);
      for(j=0; j<irregular->nb_iterators[i];j++)
        sprintf(buffer, "%s%s ", buffer, irregular->iterators[i][j]);
      sprintf(buffer, "%s\n%s\n", buffer, irregular->body[i]);
    }

    openscop_util_safe_strcat(&string, buffer, &high_water_mark);

    // Print the end tag.
    sprintf(buffer, OPENSCOP_TAG_IRREGULAR_STOP"\n");
    openscop_util_safe_strcat(&string, buffer, &high_water_mark);
  
    // Keep only the memory space we need.
    string = (char *)realloc(string, (strlen(string) + 1) * sizeof(char));
    free(buffer);
  }

  return string;
}


/*****************************************************************************
 *                               Reading function                            *
 *****************************************************************************/

/**
 * openscop_irregular_read function:
 * this function reads a irregular structure from a string complying to the
 * OpenScop textual format and returns a pointer to this irregular structure.
 * The string should contain only one textual format of a irregular structure.
 * \param  extensions The input string where to find a irregular structure.
 * \return A pointer to the irregular structure that has been read.
 */
openscop_irregular_p
openscop_irregular_read(char * extensions)
{
  char * content,*tok;
  int i,j;
  openscop_irregular_p irregular;

  content = openscop_util_tag_content(extensions, OPENSCOP_TAG_IRREGULAR_START,
                                                  OPENSCOP_TAG_IRREGULAR_STOP);
  if (content == NULL)
  {
    fprintf(stderr, "[OpenScop] Info: no irregular optional tag.\n");
    return NULL;
  }

  if (strlen(content) > OPENSCOP_MAX_STRING)
  { 
    fprintf(stderr, "[OpenScop] Error: irregular too long.\n");
    exit(1);
  }

  irregular = openscop_irregular_malloc();

  // nb statements
  tok = strtok(content," \n");
  irregular->nb_statements = atoi(tok);
  irregular->predicates = (int**) malloc(sizeof(int*) *
                                         irregular->nb_statements);
  irregular->nb_predicates = (int*) malloc(sizeof(int) *
                                         irregular->nb_statements);
  if (irregular->predicates == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  // get predicats
  for(i=0; i<irregular->nb_statements; i++)
  {
    // nb conditions
    tok = strtok(NULL," \n");
    irregular->nb_predicates[i] = atoi(tok);
    irregular->predicates[i] = (int* )malloc(sizeof(int)*
                                             irregular->nb_predicates[i]);
    if (irregular->predicates[i] == NULL)
    {
      fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
      exit(1);
    }
    for(j=0; j<irregular->nb_predicates[i]; j++){
      tok = strtok(NULL," \n");
      irregular->predicates[i][j]=atoi(tok);
    }
  }
  // Get nb predicat
  tok = strtok(NULL," \n");
  irregular->nb_control=atoi(tok);
  irregular->iterators = (char***) malloc(sizeof(char**) *
                                         irregular->nb_control);
  if (irregular->iterators == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  irregular->nb_iterators = (int*) malloc(sizeof(int)    *
                                         irregular->nb_control);
  if (irregular->nb_iterators == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  irregular->body = (char**) malloc(sizeof(char*)   * 
                                         irregular->nb_control);
  if (irregular->body == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  for(i=0; i<irregular->nb_control; i++)
  {
    // Get number of iterators
    tok = strtok(NULL," \n");
    irregular->nb_iterators[i]=atoi(tok);
    irregular->iterators[i] = (char**) malloc(sizeof(char*) * 
                                              irregular->nb_iterators[i]);
    if (irregular->iterators[i] == NULL)
    {
      fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
      exit(1);
    }
    // Get iterators
    for(j=0; j<irregular->nb_iterators[i]; j++)
      irregular->iterators[i][j]=strdup(strtok(NULL," \n"));
    // Get predicat string
    irregular->body[i] = strdup(strtok(NULL,"\n"));
  }
  
  return irregular;
}


/*+***************************************************************************
 *                    Memory allocation/deallocation function                *
 *****************************************************************************/


/**
 * openscop_irregular_malloc function:
 * This function allocates the memory space for an openscop_irregular_t
 * structure and sets its fields with default values. Then it returns a
 * pointer to the allocated space.
 * \return A pointer to an empty irregular structure with fields set to
 *         default values.
 */
openscop_irregular_p
openscop_irregular_malloc()
{
  openscop_irregular_p irregular;

  irregular = (openscop_irregular_p)malloc(sizeof(openscop_irregular_t));
  if (irregular == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  
  irregular->nb_statements = 0;
  irregular->predicates = NULL;
  irregular->nb_predicates = NULL;
  irregular->nb_control = 0;
  irregular->nb_iterators = NULL;
  irregular->iterators = NULL;
  irregular->body = NULL;

  return irregular;
}


/**
 * openscop_irregular_free function:
 * This function frees the allocated memory for an openscop_irregular_t
 * structure.
 * \param irregular The pointer to the irregular structure we want to free.
 */
void
openscop_irregular_free(openscop_irregular_p irregular)
{
  int i,j;
  if (irregular != NULL)
  {
    for(i=0; i<irregular->nb_statements; i++)
      free(irregular->predicates[i]);
    
    if(irregular->predicates != NULL)
      free(irregular->predicates);
    
    for(i=0; i<irregular->nb_control; i++)
    {
      for(j=0; j<irregular->nb_iterators[i]; j++)
        free(irregular->iterators[i][j]);
      free(irregular->body[i]);
    }
    if(irregular->iterators != NULL)
      free(irregular->iterators);
    if(irregular->nb_iterators != NULL)
      free(irregular->nb_iterators);
    if(irregular->body != NULL)
      free(irregular->body);
    if(irregular->nb_predicates != NULL)
      free(irregular->nb_predicates);
    free(irregular);
  }
}


/*+***************************************************************************
 *                            Processing functions                           *
 *****************************************************************************/


/**
 * openscop_irregular_copy function:
 * This function builds and returns a "hard copy" (not a pointer copy) of an
 * openscop_irregular_t data structure.
 * \param irregular The pointer to the irregular structure we want to copy.
 * \return A pointer to the copy of the irregular structure.
 */
openscop_irregular_p
openscop_irregular_copy(openscop_irregular_p irregular)
{
  int i,j;
  openscop_irregular_p copy;

  if (irregular == NULL)
    return NULL;

  copy = openscop_irregular_malloc();
  if (copy == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }

  copy->nb_statements = irregular->nb_statements;
  copy->nb_predicates = (int *)malloc(sizeof(int)*copy->nb_statements);
  if (copy->nb_predicates == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  copy->predicates = (int **)malloc(sizeof(int*)*copy->nb_statements);
  if (copy->predicates == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  for(i=0; i<copy->nb_statements; i++)
  {
    copy->nb_predicates[i]=irregular->nb_predicates[i];
    copy->predicates[i] = (int *)malloc(sizeof(int)*copy->nb_predicates[i]);
    if (copy->predicates[i] == NULL)
    {
      fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
      exit(1);
    }
    for(j=0; j<copy->nb_predicates[i]; j++)
      copy->predicates[i][j] = irregular->predicates[i][j];
  }

  copy->nb_control = irregular->nb_control;
  copy->nb_iterators = (int *)malloc(sizeof(int)*copy->nb_control);
  if (copy->nb_predicates == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  copy->iterators = (char ***)malloc(sizeof(char**)*copy->nb_control);
  if (copy->iterators == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  copy->body = (char **)malloc(sizeof(char*)*copy->nb_control);
  if (copy->body == NULL)
  {
    fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
    exit(1);
  }
  for(i=0; i<copy->nb_control; i++)
  {
    copy->nb_iterators[i] = irregular->nb_iterators[i];
    copy->iterators[i] = (char**)malloc(sizeof(char*)*copy->nb_iterators[i]);
    if (copy->iterators[i] == NULL)
    {
      fprintf(stderr, "[OpenScop] Error: memory overflow.\n");
      exit(1);
    }
    for(j=0;j<copy->nb_iterators[i];j++)
      copy->iterators[i][j] = strdup(irregular->iterators[i][j]);
    copy->body[i] = strdup(irregular->body[i]);
  }

  return copy;
}


/**
 * openscop_irregular_equal function:
 * this function returns true if the two irregular structures are the same
 * (content-wise), false otherwise. This functions considers two irregular
 * \param c1  The first irregular structure.
 * \param c2  The second irregular structure.
 * \return 1 if c1 and c2 are the same (content-wise), 0 otherwise.
 */
int
openscop_irregular_equal(openscop_irregular_p c1, openscop_irregular_p c2)
{
  int i,j,bool = 0;
  if (((c1 == NULL) && (c2 == NULL)) || c1 == c2)
    return 1;

  if (((c1 == NULL) && (c2 != NULL)) || ((c1 != NULL) && (c2 == NULL)))
    return 0;

  if(c1->nb_statements != c2->nb_statements||c1->nb_control != c2->nb_control)
    return 0;
  i=0;
  while(bool == 0 && i < c1->nb_statements)
  {
    bool = c1->nb_predicates[i] != c2->nb_predicates[i] ? 1 : 0;
    i++;
  }
  if(bool != 0)
    return 0;

  i = 0;
  while(bool == 0 && i < c1->nb_control)
  {
    bool += c1->nb_iterators[i] != c2->nb_iterators[i] ? 1 : 0;
    bool += strcmp(c1->body[i],c2->body[i]);
    j = 0;
    while(bool == 0 && j < c1->nb_iterators[i])
    {
      bool += strcmp(c1->iterators[i][j],c2->iterators[i][j]);
      j++;
    }
    i++;
  }
  if(bool != 0)
    return 0;
  return 1;
}

openscop_irregular_p openscop_irregular_add_control(
                                               openscop_irregular_p irregular,
                                               char** iterators,
					       int nb_iterators,
					       char* body)
{
  int i,j;
  openscop_irregular_p result=openscop_irregular_malloc();

  result->nb_control = irregular->nb_control+1;
  result->nb_statements = irregular->nb_statements;

  result->iterators = (char***)malloc(sizeof(char**)*result->nb_control);
  result->nb_iterators = (int*)malloc(sizeof(int)*result->nb_control);
  result->body = (char**)malloc(sizeof(char*)*result->nb_control);
  //copy
  for(i=0; i<irregular->nb_control; i++)
  {
    result->nb_iterators[i] = irregular->nb_iterators[i];
    result->body[i] = strdup(irregular->body[i]); 
    result->iterators[i] = (char**)malloc(sizeof(char*)  *  
                                          irregular->nb_iterators[i]);
    for(j=0; j<irregular->nb_iterators[i];j++)
      result->iterators[i][j] = strdup(irregular->iterators[i][j]);
  }
  result->nb_predicates = (int*)malloc(sizeof(int)*irregular->nb_statements);
  result->predicates = (int**)malloc(sizeof(int*)*irregular->nb_statements);
  for(i=0; i<irregular->nb_statements; i++)
  { 
    result->predicates[i] = (int*)malloc(sizeof(int)*irregular->nb_predicates[i]);
    result->nb_predicates[i] = irregular->nb_predicates[i];
    for(j=0; j<irregular->nb_predicates[i]; j++)
      result->predicates[i][j]=irregular->predicates[i][j];
  }
  //add
  result->iterators[irregular->nb_control] = (char**)malloc(sizeof(char*)*nb_iterators);
  for(i=0; i<nb_iterators; i++)
    result->iterators[irregular->nb_control][i] = strdup(iterators[i]);
  result->nb_iterators[irregular->nb_control] = nb_iterators;
  result->body[irregular->nb_control] = strdup(body);

  return result;

}

openscop_irregular_p openscop_irregular_add_predicates(
                                               openscop_irregular_p irregular,
					       int* predicates, 
					       int nb_predicates)
{
  int i,j;
  openscop_irregular_p result=openscop_irregular_malloc();

  result->nb_control = irregular->nb_control;
  result->nb_statements = irregular->nb_statements+1;

  result->iterators = (char***)malloc(sizeof(char**)*irregular->nb_control);
  result->nb_iterators = (int*)malloc(sizeof(int)*irregular->nb_control);
  result->body = (char**)malloc(sizeof(char*)*irregular->nb_control);
  //copy
  for(i=0; i<irregular->nb_control; i++)
  {
    result->nb_iterators[i] = irregular->nb_iterators[i];
    result->body[i] = strdup(irregular->body[i]); 
    result->iterators[i] = (char**)malloc(sizeof(char*)  *  
                                          irregular->nb_iterators[i]);
    for(j=0; j<irregular->nb_iterators[i];j++)
      result->iterators[i][j] = irregular->iterators[i][j];
  }
  result->nb_predicates = (int*)malloc(sizeof(int)*result->nb_statements);
  result->predicates = (int**)malloc(sizeof(int*)*result->nb_statements);
  for(i=0; i<irregular->nb_statements; i++)
  { 
    result->predicates[i] = (int*)malloc(sizeof(int)*irregular->nb_predicates[i]);
    result->nb_predicates[i] = irregular->nb_predicates[i];
    for(j=0; j<irregular->nb_predicates[i]; j++)
      result->predicates[i][j]=irregular->predicates[i][j];
  }
  //add
  result->predicates[irregular->nb_statements] = (int*)malloc(sizeof(int)*nb_predicates);
  for(i=0; i<nb_predicates; i++)
    result->predicates[irregular->nb_statements][i] = predicates[i];
  result->nb_predicates[irregular->nb_statements] = nb_predicates;

  return result;


}