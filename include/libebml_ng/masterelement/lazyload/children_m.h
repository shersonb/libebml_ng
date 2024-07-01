#ifndef EBML_NG_LAZYLOAD_CHILDREN_M_H
#define EBML_NG_LAZYLOAD_CHILDREN_M_H

#include <map>
#include "status.h"
#include "../../seekdata.h"


namespace ebml {
    typedef std::map<off_t, std::unique_ptr<seekData_t>> children_m;
}
#endif
