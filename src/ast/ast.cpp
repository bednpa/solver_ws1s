/*
 * MONA
 * Copyright (C) 1997-2013 Aarhus University.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335,
 * USA.
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "predlib.h"
#include "symboltable.h"

using std::cout;
using std::vector;



//////////  BitList ///////////////////////////////////////////////////////////

BitList::BitList(char *str)
{
  for (; *str; str++)
    if (*str == '0') 
      push_back(Zero);
    else
      push_back(One);
}

///////// Helper custom classes /////////

void remapVars::addRecord(Ident origin_ident, Ident new_ident)
{
  var_stack.push_back(origin_ident);
  var_stack.push_back(new_ident); 
}

Ident remapVars::getMapping(const Ident &var)
{
  // must be from back because of recursion calls (newest remap must be found)
  for (auto i = var_stack.rbegin(); i != var_stack.rend(); i = i + 2)
  {
    if (*(i+1) == var)
    {
      return *i;
    }
  } 
  return var; // because of complier warning
}


Ident remapVars::getRevMapping(const Ident &var)
{
  for (auto i = var_stack.begin(); i != var_stack.end(); i = i + 2)
  {
    if (*(i+1) == var)
    {
      return *i;
    }
  } 
  return var; // because of compiler warning
}

