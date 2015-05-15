#ifndef IDe05afd8b_20ea_4b15_a468_54277c21d640
#define IDe05afd8b_20ea_4b15_a468_54277c21d640

/* This file makes sure that the automatically generates header files
   along with the necessary definitions of data types are included in 
   the right order.
*/

#include "ast.hpp"

struct StringR {
  string* str;
  int location;
};
#include "parser.tab.h"

#endif
