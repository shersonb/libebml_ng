#ifndef EBML_NG_LAZYLOAD_REMOVE_CPP
#define EBML_NG_LAZYLOAD_REMOVE_CPP

#include "libebml_ng/masterelement/lazyload/base.h"
// #include "libebml_ng/void.h"
// #include "libebml_ng/ebmlElementClass.tpp"
// #include "libebml_ng/element.tpp"
// #include "libebml_ng/document.h"
// #include "libebml_ng/parsing/string.h"
// #include "libebml_ng/masterelement/lazyload/status.tpp"

namespace ebml {
        // prepared_remove_t prepareRemove(off_t, off_t, const writeLock_t&);
        void ebmlLazyLoad::remove(off_t offset) {

        }

        void ebmlLazyLoad::remove(off_t offset, const readLock_t&) {

        }

        void ebmlLazyLoad::remove(off_t offset, const writeLock_t&) {

        }

        void ebmlLazyLoad::remove(off_t offset, const writeLock_t&, const prepared_remove_t&) {

        }

        void ebmlLazyLoad::_remove(off_t offset, const prepared_remove_t&) {
            // auto iters = this->_find(offset, false);
            auto iter = this->_children_by_offset.lower_bound(offset);
            auto end = this->_children_by_offset.end();

            if (iter == end) {
                // throw ebmlWriteError("No child element at this offset", offset, -1, offset, -1, -1);
                throw ebmlWriteError("No child element at this offset");
            }

            if (iter->first != offset) {
                // throw ebmlWriteError("No child element at this offset", offset, -1, offset, -1, -1);
                throw ebmlWriteError("No child element at this offset");
            }

            this->_remove(iter);
        }
//     void ebmlLazyLoad::_remove(off_t offset) {
//         // auto iters = this->_find(offset, false);
//         auto iter = this->_children_by_offset.lower_bound(offset);
//         auto end = this->_children_by_offset.end();
//
//         if (iter == end) {
//             // throw ebmlWriteError("No child element at this offset", offset, -1, offset, -1, -1);
//             throw ebmlWriteError("No child element at this offset");
//         }
//
//         if (iter->first != offset) {
//             // throw ebmlWriteError("No child element at this offset", offset, -1, offset, -1, -1);
//             throw ebmlWriteError("No child element at this offset");
//         }
//
//         this->_remove(iter);
//     }

    void ebmlLazyLoad::_remove(const children_m::iterator& pos) {
        off_t prevEnd = 0;
        off_t nextOffset = static_cast<off_t>(this->_dataSize);

        if (pos != this->_children_by_offset.begin()) {
            children_m::iterator prev = pos; --prev;
            prevEnd = prev->second->endOffset();
        }

        children_m::iterator next = pos; ++next;

        if (next != this->_children_by_offset.end()) {
            nextOffset = next->second->offsetInParent;
        }

        auto& f = this->file();
        auto poffset = this->dataOffsetInFile();

        if (nextOffset == -1) {
            Void.writeVoid(f, poffset + prevEnd, UNKNOWN);
        } else if (nextOffset > prevEnd) {
            Void.writeVoid(f, poffset + prevEnd, nextOffset - prevEnd);
        }

        this->_children_by_ebmlID[pos->second->ebmlID].erase(pos->first);
        pos->second->rm(this->_children_by_key);

        auto elem = pos->second->ref.get();

        if (elem != nullptr) {
            this->_detachChild(elem);
        }

        auto seekData = std::move(pos->second);
        this->_children_by_offset.erase(pos);
        // this->_postremove_hook(seekData.get());
    }
}
#endif
