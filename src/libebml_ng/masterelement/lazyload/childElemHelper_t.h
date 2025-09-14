#ifndef EBML_NG_LAZYLOAD_HELPER_H
#define EBML_NG_LAZYLOAD_HELPER_H

// #include <unistd.h>
// #include "ebmlLazyLoad.h"
// #include <unordered_map>
// #include <map>
// #include <unordered_set>
// #include <mutex>
// #include <shared_mutex>
//
// #include "base.h"
// #include "../seekdata.h"
// #include "../exceptions.h"
// #include "../ebmlDataElement.h"
#include "../../ebmlElement.h"


namespace ebml {
    class ebmlLazyLoad;

    class childElemHelper_t {
    public:
        childElemHelper_t();
        childElemHelper_t(const childElemHelper_t&);

    protected:
        virtual void _onBeforeInsert(ebmlLazyLoad&, off_t, const ebmlElement_sp&) const;
        virtual void _onAfterInsert(ebmlLazyLoad&, off_t, const ebmlElement_sp&, seekData_t*) const;
        virtual void _onBeforeMove(ebmlLazyLoad&) const;
        virtual void _onAfterMove(ebmlLazyLoad&) const;
        virtual void _onBeforeRemove(ebmlLazyLoad&) const;
        virtual void _onAfterRemove(ebmlLazyLoad&) const;
        friend class ebmlLazyLoad;
    };

    extern childElemHelper_t childHelper;

}

#endif
