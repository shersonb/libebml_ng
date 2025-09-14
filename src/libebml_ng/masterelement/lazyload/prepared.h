#ifndef EBML_NG_LAZYLOAD_PREPARED_H
#define EBML_NG_LAZYLOAD_PREPARED_H

#include "children_m.h"
#include <string>
#include "../base.h"
#include "extent_t.h"

namespace ebml {
    // struct insert_t {
    //     off_t offset;
    //     ebmlID_t ebmlID;
    // };

    class prepared_insert_t {
    public:
        off_t prevOffset;
        off_t prevEnd;

        extent_t extent;

        off_t endOffset;
        off_t nextOffset;
        // sizetree_t sizetree;
        children_m::iterator insertionHint;

        // prepared_insert_t(off_t, off_t, off_t, off_t, off_t, const sizetree_t&, const children_m::iterator&);
        // prepared_insert_t(off_t, off_t, off_t, off_t, off_t, sizetree_t&&, children_m::iterator&&);
        // prepared_insert_t(off_t, off_t, off_t, off_t, off_t);
        prepared_insert_t(off_t, off_t, const extent_t&, off_t, off_t, const children_m::iterator&);
        prepared_insert_t(off_t, off_t, const extent_t&, off_t, off_t, children_m::iterator&&);
        prepared_insert_t(off_t, off_t, extent_t&&, off_t, off_t, const children_m::iterator&);
        prepared_insert_t(off_t, off_t, extent_t&&, off_t, off_t, children_m::iterator&&);
        prepared_insert_t(off_t, off_t, const extent_t&, off_t, off_t);
        prepared_insert_t(off_t, off_t, extent_t&&, off_t, off_t);
        // ~prepared_insert_t();

        prepared_insert_t(const prepared_insert_t&);
        prepared_insert_t(prepared_insert_t&&);

        prepared_insert_t& operator=(const prepared_insert_t&);
        prepared_insert_t& operator=(prepared_insert_t&&);

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlInsertionError() const;
    };

    class prepared_multiinsert_t {
    public:
        off_t prevOffset;
        off_t prevEnd;
        std::vector<extent_t> extents;
        off_t endOffset;
        off_t nextOffset;
        children_m::iterator insertionHint;

        prepared_multiinsert_t(off_t, off_t, const std::vector<extent_t>&, off_t, off_t, const children_m::iterator&);
        prepared_multiinsert_t(off_t, off_t, std::vector<extent_t>&&, off_t, off_t, const children_m::iterator&);
        prepared_multiinsert_t(off_t, off_t, const std::vector<extent_t>&, off_t, off_t, children_m::iterator&&);
        prepared_multiinsert_t(off_t, off_t, std::vector<extent_t>&&, off_t, off_t, children_m::iterator&&);
        prepared_multiinsert_t(off_t, off_t, const std::vector<extent_t>&, off_t, off_t);
        prepared_multiinsert_t(off_t, off_t, std::vector<extent_t>&&, off_t, off_t);
        // prepared_multiinsert_t(off_t, off_t, const std::vector<off_t>&, off_t, off_t, const std::vector<sizetree_t>&, const children_m::iterator&);
        // prepared_multiinsert_t(off_t, off_t, std::vector<off_t>&&, off_t, off_t, const std::vector<sizetree_t>&, const children_m::iterator&);
        // prepared_multiinsert_t(off_t, off_t, const std::vector<off_t>&, off_t, off_t, std::vector<sizetree_t>&&, const children_m::iterator&);
        // prepared_multiinsert_t(off_t, off_t, std::vector<off_t>&&, off_t, off_t, std::vector<sizetree_t>&&, const children_m::iterator&);
        // prepared_multiinsert_t(off_t, off_t, const std::vector<off_t>&, off_t, off_t);
        // prepared_multiinsert_t(off_t, off_t, std::vector<off_t>&&, off_t, off_t);
        // ~prepared_insert_t();

        prepared_multiinsert_t(const prepared_multiinsert_t&);
        prepared_multiinsert_t(prepared_multiinsert_t&&);

        prepared_multiinsert_t& operator=(const prepared_multiinsert_t&);
        prepared_multiinsert_t& operator=(prepared_multiinsert_t&&);

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlInsertionError() const;
    };

    class prepared_move_t {
    public:
        off_t srcPrevEnd;
        off_t srcOffset;
        off_t srcNextOffset;

        off_t prevOffset;
        off_t prevEnd;

        extent_t extent;
        // off_t dest_offset;
        // off_t endOffset;

        off_t nextOffset;
        children_m::iterator sourceIter;
        children_m::iterator insertionHint;

        prepared_move_t(off_t, off_t, off_t, off_t, off_t, off_t, off_t, off_t, const children_m::iterator&, const children_m::iterator&);
        prepared_move_t(off_t, off_t, off_t, off_t, off_t, off_t, off_t, off_t, children_m::iterator&&, children_m::iterator&&);
        prepared_move_t(off_t, off_t, off_t, off_t, off_t, off_t, off_t, off_t);

        prepared_move_t(const prepared_move_t&);
        prepared_move_t(prepared_move_t&&);

        prepared_move_t& operator=(const prepared_move_t&);
        prepared_move_t& operator=(prepared_move_t&&);

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlMoveError() const;
    };

    class prepared_remove_t {
    public:
        off_t prevEnd;
        off_t offset;
        off_t nextOffset;
        children_m::iterator removeHint;

        prepared_remove_t(off_t, off_t, off_t, const children_m::iterator&);
        prepared_remove_t(off_t, off_t, off_t, children_m::iterator&&);
        prepared_remove_t(off_t, off_t, off_t);

        prepared_remove_t(const prepared_remove_t&);
        prepared_remove_t(prepared_remove_t&&);

        prepared_remove_t& operator=(const prepared_remove_t&);
        prepared_remove_t& operator=(prepared_remove_t&&);

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlMoveError() const;
    };

    class prepared_resize_t {
    public:
        off_t offset;
        off_t endOffset;
        off_t lastChildOffset;
        off_t lastChildEnd;
        off_t nextSiblingOffset;
        seekData_t* seekData = nullptr;

        prepared_resize_t(off_t, off_t, off_t, off_t, off_t, seekData_t*);
        prepared_resize_t(off_t, off_t, off_t, off_t, off_t);

        prepared_resize_t(const prepared_resize_t&);
        prepared_resize_t(prepared_resize_t&&);

        prepared_resize_t& operator=(const prepared_move_t&);
        prepared_resize_t& operator=(prepared_resize_t&&);

        void throw_exc(const std::string&, const c_ebmlElement_sp&) const;
        // operator ebmlResizeError() const;
    };
}

#endif
