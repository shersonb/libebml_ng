#ifndef EBML_NG_LAZYLOAD_LOCK_CPP
#define EBML_NG_LAZYLOAD_LOCK_CPP

#include "libebml_ng/masterelement/lazyload/locks.h"
#include <utility>

namespace ebml {
    readLock_t::readLock_t(std::vector<std::shared_lock<std::shared_mutex>>&& locks, const ebmlLazyLoad* elem) : locks(std::move(locks)), elem(elem) {}
    readLock_t::readLock_t(readLock_t&& other) : locks(std::move(other.locks)), elem(std::exchange(other.elem, nullptr)) {
        other.locks.clear();
    }

    readLock_t& readLock_t::operator=(readLock_t&& other) {
        locks = std::move(other.locks);
        other.locks.clear();
        elem = std::exchange(other.elem, nullptr);
        return *this;
    }

    writeLock_t::writeLock_t(std::vector<std::shared_lock<std::shared_mutex>>&& rlocks, std::unique_lock<std::shared_mutex>&& wlock, const ebmlLazyLoad* elem)
    : rlocks(std::move(rlocks)), wlock(std::move(wlock)), elem(elem) {}

    writeLock_t::writeLock_t(std::unique_lock<std::shared_mutex>&& wlock, const ebmlLazyLoad* elem) : wlock(std::move(wlock)), elem(elem) {}

    writeLock_t::writeLock_t(writeLock_t&& other) : rlocks(std::move(other.rlocks)), wlock(std::move(wlock)), elem(std::exchange(other.elem, nullptr)),
    cache(std::move(cache)), order(std::move(order)) {
        other.rlocks.clear();
    }

    writeLock_t& writeLock_t::operator=(writeLock_t&& other) {
        rlocks = std::move(other.rlocks);
        other.rlocks.clear();
        wlock = std::move(other.wlock);
        elem = std::exchange(other.elem, nullptr);
        cache = std::move(other.cache);
        order = std::move(other.order);
        return *this;
    }

}
#endif
// if (readLock.elem == this) {
//     // If elem == this, convert the last shared_lock to unique_lock
//     std::shared_lock<std::shared_mutex>& lastSharedLock = readLocks.back();
//     auto lock = std::unique_lock<std::shared_mutex>(lastSharedLock.mutex(), std::adopt_lock);
//     return writeLock_t(std::move(writeLock), this);
// }
