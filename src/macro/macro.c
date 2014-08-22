/*
 ***********************************************************************
 * CVS Information
 *    $Author$
 *    $Date$
 *    $Revision$
 *    $RCSfile$
 *    $Name$
 ***********************************************************************
 */
/*
 ***********************************************************************
 * Module Name: macro.c                                                *
 *                                                                     * 
 * Purpose: Provides a simple Macro Processor for the SIC language,    *
 *          Based on Leland L.Beck's Macro Processor                   *
 *                                                                     *  
 * Comments: Lots of Bugs in this module, lots of clean up to do       *     
 ***********************************************************************
 */            
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "macro.h"

int main (int argc, char *argv[])
{
  printf ("\nA Macro Processor for SIC & SIC/XE\n\n");
  
  if (argc < 2)
    {
      printf ("The proper syntax is\n\nmacro <filename.asm>\n");
      exit (1);
    }
  src = fopen (argv[1], "r");
  dst = fopen ("expanded.asm", "w");
  processMacro ();
  printf ("Macros Processed Successfully\n Expanded file is expanded.asm\n");
  return 0;
}

/*
 ******************************************
 * Starts the process of Macro Processing *
 ******************************************
 */      
void processMacro (void)
{
  EXPANDING = !TRUE;
  
  while (strcmp (OPCODE, "END") != 0)
    {
      getLine ();
      processLine ();
    }
}//macro processor

/*
 ************************************************
 * Checks the current line for macro expansion, * 
 * macro definition, or to output to file       *
 ************************************************
 */      
void processLine (void)
{
  if (SearchNAM (NAMhead) != 0)
    expand ();
  else 
    if (strcmp (OPCODE, "MACRO") == 0)
      define ();
    else
      if ((LINE[0] != '.') && (strcmp (OPCODE, "MEND") != 0))
	fprintf (dst, "%s  %s    %s\n", LABEL, OPCODE, OPERAND);
}//processLine

/*
 ************************************************
 * Defines the Macro into a DEFTAB also checks  * 
 * for and defines recursive macro definition   *
 ************************************************
 */      
void define (void)
{
  int LEVEL = 1;
  line = 0;
  sprintf (DEFTAB[page][line++], "%s    %s", LABEL, OPERAND);
  strcpy (temp[0], strtok (OPERAND, ","));
  strcpy (temp[1], strtok (NULL, ","));
  strcpy (temp[2], strtok (NULL, "\0"));
  strcpy (macroNam, LABEL);

  while (LEVEL > 0)
    {
      getLine ();
      if (LABEL[0] != '.')
	{
	  substitute ('P');
	  if (OPCODE[0] != '\0')
	    sprintf (DEFTAB[page][line++], "%s    %s", OPCODE, OPERAND);
	  if (strcmp (OPCODE, "MACRO") == 0)
	    LEVEL++;
	  else
	    if (strcmp (OPCODE, "MEND") == 0)
	      LEVEL--;
	}//if not comment
    }//while
  
  if (NAMhead == NULL)
    {
      NAMhead =  (NAMlink *)malloc (sizeof (NAMlink));
      NAMhead->page = page;
      NAMhead->beg = 0;
      NAMhead->end = line;
      strcpy (NAMhead->OPCODE, macroNam);
      NAMhead->next = NULL; 
    }
  else
    InsertNAM (NAMhead);
  page++;
  line = 0;
}//define

/*
 ************************************************
 * Expands a macro into the real code with the  * 
 * help of DEFTAB, ARGTAB                       *
 ************************************************
 */
void expand (void)
{
  line = 0;
  EXPANDING = TRUE;
  strcpy (ARGTAB[0], strtok (OPERAND, ","));
  strcpy (ARGTAB[1], strtok (NULL, ","));
  strcpy (ARGTAB[2], strtok (NULL, "\0"));
  sscanf (DEFTAB [page][line++], "%s%s", OPCODE, OPERAND);
  
  strcpy (temp[0], "?1");
  strcpy (temp[1], "?2");
  strcpy (temp[2], "?3");
  if (LABEL[0] != ' ')
    fprintf (dst, ".%s\n", LINE);
  else
    fprintf (dst, "%s\n", LINE);
  
  while((strcmp (OPCODE, "MEND") != 0))
    {
      getLine ();
      processLine ();
      strcpy (LABEL, "      ");
    }
 
  EXPANDING = !TRUE;
}//expand

/*
 ************************************************
 * gets line from the input file or from the    * 
 * DEFTAB as per request                        *
 ************************************************
 */
void getLine (void)
{
  int i = 0;
  if (EXPANDING)
    {
      sscanf (DEFTAB [page][line++], "%s%s", OPCODE, OPERAND);
      substitute ('A');
    }//If
  else
    {
      fscanf (src, "%[^\n]", LINE);
      for (i = 0; i < 30; i++)
	LABEL[i] = OPERAND[i] = OPCODE[i] = '\0';
      if (LINE[0] != '.')
	{
	  if (LINE[0] != ' ')
	    sscanf (LINE, "%s%s%s", LABEL, OPCODE, OPERAND);
	  else
	    {
	      sscanf (LINE, "%s%s", OPCODE, OPERAND);
	      strcpy (LABEL, "      ");
	    }
	}
      fgetc (src);
    }
}
 
/*
 ************************************************
 * Creates & Maintains a Link List that contain *
 * OPCODEs that does MACRO calls                * 
 ************************************************
 */
void InsertNAM (NAMlink *head)
{
  NAMlink *current = head;
  if (current->next == NULL)
    {
      current->next =  (NAMlink *)malloc (sizeof (NAMlink));
      current = current->next;
      current->page = page;
      current->beg = 0;
      current->end = line;
      strcpy (current->OPCODE, macroNam);
      current->next = NULL; 
    }
  else
    InsertNAM (current->next);
}

/*
********************************************************
 * Searches the Link List that contain OPCODEs         *
 * that does MACRO calls, returns 1 if present else 0  * 
 *******************************************************
 */
int SearchNAM (NAMlink *head)
{
  NAMlink *current = head;
  if (current == NULL)
    retval = 0;
  else
    if (strcmp (current->OPCODE, OPCODE) == 0)
      {
	page = current->page;
	retval = 1;
      }
    else
      SearchNAM(current->next);

  return retval;
}

/*
************************************************************
 * Substitutes parameters with positional notaion or       *
 * positional notation with arguments from ARGTAB wrt type * 
 ***********************************************************
 */   
void substitute (char type)
{
  int i = 0;
  switch (type)
    {
    
    case 'P':
      {
	for (i = 0; i < params; i++)
	  if ((strstr (OPERAND,temp[i])) != NULL)
	    replace (OPERAND, temp[i], i + 1);
	break;
      }
    case 'A':
      {
	for (i = 0; i < params; i++)
	  if ((strstr (OPERAND, temp[i]) != NULL))
	      replace2 (OPERAND, ARGTAB[i]);
      }
    }
}

/*
 ***************************************************
 * Substitutes parameters with positional notaion  * 
 ***************************************************
 */
void replace (char *token, char *new, int n)
{
  int k, len;
  k = strlen (token) - strlen (strstr (token,new));
  len =  strlen (new) - 2;
  token[k++] = '?'; 
  token[k++] = 48 + n;
  //temp = k + len;
  while (token[k + len] != '\0')
    {
      token[k] = token[k + len];
      k++;
    }
  token[k] = '\0';
}

/*
****************************************************************
 * Substitutes positional notation with arguments from ARGTAB  * 
 ***************************************************************
 */
void replace2 (char *token, char *new)
{
  char buff[20], temp[20], *token2;
  strcpy (temp, token);
  if (token[0] != '?')
    {
      strcpy (buff, strtok (token, "?"));
      strcpy (token, temp);
      token2 = token + strlen (token) - strlen (buff) + 2;
      strcat (buff, new);
    }
  else
    {
      strcpy (buff, new);
      token2 = token + 2;
    }
  strcat (buff, token2);
  strcpy (token, buff);
}
