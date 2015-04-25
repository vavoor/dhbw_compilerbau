#include "ast.hpp"
#include "err.h"

Program* the_program;

IntType* IntType::m_singleton = NULL;
FloatType* FloatType::m_singleton = NULL;
StringType* StringType::m_singleton = NULL;
VoidType* VoidType::m_singleton = NULL;

void Program::resolve()
{
}
