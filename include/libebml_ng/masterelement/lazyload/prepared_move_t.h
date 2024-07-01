#ifndef EBML_NG_LAZYLOAD_PREPARED_MOVE_H
#define EBML_NG_LAZYLOAD_PREPARED_MOVE_H

#include "prepared_insert_t.h"
#include "prepared_remove_t.h"

namespace ebml {
    class prepared_move_t {
    public:
        prepared_remove_t src;
        prepared_insert_t dest;

        prepared_move_t();

        prepared_move_t(const prepared_remove_t&, const prepared_insert_t&);
        prepared_move_t(const prepared_remove_t&, prepared_insert_t&&);
        prepared_move_t(prepared_remove_t&&, const prepared_insert_t&);
        prepared_move_t(prepared_remove_t&&, prepared_insert_t&&);

        prepared_move_t(const prepared_move_t&) = default;
        prepared_move_t(prepared_move_t&&) = default;

        prepared_move_t& operator=(const prepared_move_t&) = default;
        prepared_move_t& operator=(prepared_move_t&&) = default;

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
    };

    extern template class status_t<prepared_move_t>;
}

#endif
