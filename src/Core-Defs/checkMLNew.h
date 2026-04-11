#ifndef CHECKMLNEW_H
#define CHECKMLNEW_H

#include "checkML.h"

// sobreescribir new
#if defined(_WIN32) && defined(_DEBUG) && defined(_MSC_VER)
#ifndef CHECKML_REMAP_NEW_ACTIVE
#define CHECKML_REMAP_NEW_ACTIVE
#define new DBG_NEW
#endif
#endif

#endif // CHECKMLNEW_H
