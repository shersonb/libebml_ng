#ifndef EBML_NG_IO_STDIO_H
#define EBML_NG_IO_STDIO_H

#include "libebml_ng/io.h"
#include <stdio.h>

namespace ebml {
    extern template class io<FILE*>;
}

#endif
