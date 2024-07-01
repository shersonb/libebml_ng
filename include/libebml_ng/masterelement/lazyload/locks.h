#ifndef EBML_NG_LAZYLOAD_LOCK_H
#define EBML_NG_LAZYLOAD_LOCK_H

#include <mutex>
#include <shared_mutex>
#include <unordered_set>
#include <vector>
#include <deque>

namespace ebml {
    class ebmlLazyLoad;

    class readLock_t {
        friend class ebmlLazyLoad;

    protected:
        mutable std::vector<std::shared_lock<std::shared_mutex>> locks;
        const ebmlLazyLoad* elem;
        readLock_t(std::vector<std::shared_lock<std::shared_mutex>>&&, const ebmlLazyLoad*);

    public:
        readLock_t(readLock_t&&);
        readLock_t& operator=(readLock_t&&);
        readLock_t& operator=(const readLock_t&) = delete;
    };

    class writeLock_t {
        friend class ebmlLazyLoad;

    protected:
        std::vector<std::shared_lock<std::shared_mutex>> rlocks;
        std::unique_lock<std::shared_mutex> wlock;
        const ebmlLazyLoad* elem;
        mutable std::unordered_set<const ebmlLazyLoad*> cache;
        mutable std::deque<const ebmlLazyLoad*> order;

        writeLock_t(std::unique_lock<std::shared_mutex>&&, const ebmlLazyLoad*);
        writeLock_t(std::vector<std::shared_lock<std::shared_mutex>>&&, std::unique_lock<std::shared_mutex>&&, const ebmlLazyLoad*);
        void _addToCache(const ebmlLazyLoad*) const;

    public:
        writeLock_t(writeLock_t&&);
        writeLock_t& operator=(writeLock_t&&);
        writeLock_t& operator=(const writeLock_t&) = delete;
    };
}
#endif
