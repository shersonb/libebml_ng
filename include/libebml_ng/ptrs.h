#ifndef EBML_NG_PTRS_H
#define EBML_NG_PTRS_H

#include <utility>
#include "forwards.h"
#include <memory>
#include <mutex>
#include <iostream>

namespace ebml {
    struct control_block {
        std::mutex mutex;
        long long strongcount = 0;
        long long weakcount = 0;
        bool valid = false;
    };

    /**
     * Replacement to std::shared_ptr, allowing for manual construction
     * of a control block. A weak pointer (ebml_weak_ptr) can also be
     * constructed from a raw pointer before any ebml_shared_ptr instance
     * is created.
     *
     * An ebml_shared_ptr to an object can even be created during its
     * construction so long as the valid member of the control block is
     * set to false, then set to true at the end of its construction when
     * said construction is deemed successful.
     *
     * Care should be taken, however, to ensure that at least one
     * ebml_shared_ptr instance survives the scope of the constructor
     */

    template<typename T>
    class ebml_shared_ptr;

    /**
     * Replacement to std::weak_ptr, allowing for manual construction
     * of a control block. An ebml_weak_ptr to an object can be constructed
     * before an ebml_shared_ptr is constructed (via ebml_weak_ptr::lock)
     * with no danger of deletion so long as no ebml_shared_ptr is created.
     */

    template<typename T>
    class ebml_weak_ptr;

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_dynamic_pointer_cast(const ebml_shared_ptr<U>&);

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_dynamic_pointer_cast(ebml_shared_ptr<U>&&);

    template<typename T, typename U>
    ebml_weak_ptr<T> ebml_dynamic_pointer_cast(const ebml_weak_ptr<U>&);

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_static_pointer_cast(const ebml_shared_ptr<U>&);

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_static_pointer_cast(ebml_shared_ptr<U>&&);

    template<typename T, typename U>
    ebml_weak_ptr<T> ebml_static_pointer_cast(const ebml_weak_ptr<U>&);

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_const_pointer_cast(const ebml_shared_ptr<U>&);

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_const_pointer_cast(ebml_shared_ptr<U>&&);

    template<typename T>
    class ebml_shared_ptr {
        friend T;

        template<typename U, typename V>
        friend ebml_shared_ptr<U> ebml_dynamic_pointer_cast(const ebml_shared_ptr<V>&);

        template<typename U, typename V>
        friend ebml_shared_ptr<U> ebml_dynamic_pointer_cast(ebml_shared_ptr<V>&&);

        template<typename U, typename V>
        friend ebml_shared_ptr<U> ebml_static_pointer_cast(const ebml_shared_ptr<V>&);

        template<typename U, typename V>
        friend ebml_shared_ptr<U> ebml_static_pointer_cast(ebml_shared_ptr<V>&&);

        template<typename U, typename V>
        friend ebml_shared_ptr<U> ebml_const_pointer_cast(const ebml_shared_ptr<V>&);

        template<typename U, typename V>
        friend ebml_shared_ptr<U> ebml_const_pointer_cast(ebml_shared_ptr<V>&&);

    protected:
        mutable std::mutex mutex;
        mutable control_block* ctl = nullptr;
        T* ptr = nullptr;

    private:

        void _incref(control_block& ctl) {
            auto lock = std::unique_lock<std::mutex>(ctl.mutex);
            ++ctl.strongcount;
        }

        bool _decref(control_block& ctl) {
            bool deleteobj = false;
            bool deletectl = false;

            {
                auto lock = std::unique_lock<std::mutex>(ctl.mutex);

                if (--ctl.strongcount <= 0) {
                    if (ctl.valid) {
                        ctl.valid = false;
                        deleteobj = true;
                    }

                    deletectl = (ctl.weakcount <= 0);
                }

            }

            if (deleteobj) {
                delete ptr;
                ptr = nullptr;
            }

            return deletectl;
        }

        void _destroyctl() {
            delete ctl;
            ctl = nullptr;
            ptr = nullptr;
        }

    protected:
        explicit ebml_shared_ptr(control_block* ctl, T* ptr) : ctl(ctl), ptr(ptr) {
            if (ctl != nullptr) {
                _incref(*ctl);
            }
        };

        explicit ebml_shared_ptr(control_block* ctl, T* ptr, const std::unique_lock<std::mutex>&) : ctl(ctl), ptr(ptr) {
            if (ctl != nullptr) {
                ++ctl->strongcount;
            }
        };

    public:
        ebml_shared_ptr() : ebml_shared_ptr(nullptr, nullptr) {}
        ebml_shared_ptr(const std::nullptr_t&) : ebml_shared_ptr(nullptr, nullptr) {}
        explicit ebml_shared_ptr(T* ptr) : ebml_shared_ptr(new control_block(), ptr) {
            ctl->valid = true;
        };

        ebml_shared_ptr(const ebml_shared_ptr<T>& other) {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                this->ctl = other.ctl;
                this->ptr = other.ptr;
                _incref(*ctl);
            }
        }

        template <class U>
        ebml_shared_ptr(const ebml_shared_ptr<U>& other) {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                this->ctl = other.ctl;
                this->ptr = other.ptr;
                _incref(*ctl);
            }
        }

        ebml_shared_ptr(ebml_shared_ptr<T>&& other) {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                this->ctl = std::exchange(other.ctl, nullptr);
                this->ptr = std::exchange(other.ptr, nullptr);
            }
        }

        ~ebml_shared_ptr() {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }
        }

        operator ebml_shared_ptr<const T>&() {
            return *reinterpret_cast<ebml_shared_ptr<const T>*>(this);
        }

        operator const ebml_shared_ptr<const T>&() const {
            return *reinterpret_cast<const ebml_shared_ptr<const T>*>(this);
        }

        // template<
        //     typename U,
        //     typename = std::enable_if_t<std::is_base_of<U, T>::value>
        //         && (std::is_polymorphic<U>::value == std::is_polymorphic<T>::value)
        // >
        // operator ebml_shared_ptr<U>&() {
        //     return *reinterpret_cast<ebml_shared_ptr<U>*>(this);
        // }

        // template<
        //     typename U,
        //     typename = std::enable_if_t<std::is_base_of<U, T>::value>
        //         && (std::is_polymorphic<U>::value == std::is_polymorphic<T>::value)
        // >
        // operator const ebml_shared_ptr<U>&() const {
        //     return *reinterpret_cast<const ebml_shared_ptr<U>*>(this);
        // }

        ebml_shared_ptr<T>& operator=(const ebml_shared_ptr<T>& other) {
            if (&other == this) {
                return *this;
            }

            using l = std::unique_lock<std::mutex>;
            using p = std::pair<l, l>;
            p lockpair = (&mutex < &other.mutex) ? p({l(mutex), l(other.mutex)}) : p({l(other.mutex), l(mutex)});

            if (other.ctl == ctl) {
                return *this;
            }

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }

            if (other.ctl != nullptr) {
                this->ctl = other.ctl;
                this->ptr = other.ptr;
                _incref(*ctl);
            }

            return *this;
        }

        ebml_shared_ptr<T>& operator=(ebml_shared_ptr<T>&& other) {
            if (&other == this) {
                return *this;
            }

            using l = std::unique_lock<std::mutex>;
            using p = std::pair<l, l>;
            p lockpair = (&mutex < &other.mutex) ? p({l(mutex), l(other.mutex)}) : p({l(other.mutex), l(mutex)});

            if (other.ctl == ctl) {
                if (ctl != nullptr) {
                    _decref(*ctl);
                }

                other.ctl = nullptr;
                other.ptr = nullptr;
                return *this;
            }

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }

            if (other.ctl != nullptr) {
                this->ctl = std::exchange(other.ctl, nullptr);
                this->ptr = std::exchange(other.ptr, nullptr);
            }

            return *this;
        }

        ebml_shared_ptr<T>& operator=(const std::nullptr_t&) {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }

            ctl = nullptr;
            ptr = nullptr;
            return *this;
        }

        bool operator==(const std::nullptr_t&) const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl == nullptr) {
                return true;
            }

            if (ptr == nullptr) {
                return true;
            }

            return !ctl->valid;
        }

        bool operator==(const ebml_shared_ptr<T>& other) const {
            using l = std::unique_lock<std::mutex>;
            using p = std::pair<l, l>;
            p lockpair = (&mutex < &other.mutex) ? p({l(mutex), l(other.mutex)}) : p({l(other.mutex), l(mutex)});

            if ((ctl == nullptr) and other.ctl == nullptr) {
                return true;
            }

            return (ptr == other.ptr);
        }

        template<typename U>
        std::enable_if_t<!std::is_same<T, U>::value, ebml_shared_ptr<T>&>
        operator=(const ebml_shared_ptr<U>& other) {
            using l = std::unique_lock<std::mutex>;
            using p = std::pair<l, l>;
            p lockpair = (&mutex < &other.mutex) ? p({l(mutex), l(other.mutex)}) : p({l(other.mutex), l(mutex)});

            if (other.ctl == ctl) {
                return *this;
            }

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }

            if (other.ctl != nullptr) {
                if (auto recast = dynamic_cast<T*>(other.ptr)) {
                    this->ctl = other.ctl;
                    this->ptr = recast;
                    _incref(*ctl);
                }
            }

            return *this;
        }

        void swap(ebml_shared_ptr<T>& other) {
            if (&other == this) {
                return;
            }

            using l = std::unique_lock<std::mutex>;
            using p = std::pair<l, l>;
            p lockpair = (&mutex < &other.mutex) ? p({l(mutex), l(other.mutex)}) : p({l(other.mutex), l(mutex)});

            if (other.ctl == ctl) {
                return;
            }

            this->ctl = std::exchange(other.ctl, ctl);
            this->ptr = std::exchange(other.ptr, ptr);
        }

        T* get() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                if (ctl->valid) {
                    return this->ptr;
                }
            }

            return nullptr;
        }
        T& operator*() const {
            T* ret = nullptr;

            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                if (ctl->valid) {
                    return *this->ptr;
                }
            }

            // Deliberately dereference nullptr
            return *ret;
        }

        T* operator->() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                if (ctl->valid) {
                    return this->ptr;
                }
            }

            return nullptr;
        }

        explicit operator bool() const {
            auto lock = std::unique_lock<std::mutex>(mutex);
            return ((ctl != nullptr) and (ptr != nullptr) and (ctl->valid));
        }

        int use_count() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                return ctl->strongcount;
            }

            return 0;
        }

        template<typename U>
        friend class ebml_shared_ptr;

        template<typename U>
        friend class ebml_weak_ptr;
    };

    template <class T>
    class ebml_weak_ptr {
        friend T;

    template<typename U, typename V>
    friend ebml_weak_ptr<U> ebml_static_pointer_cast(const ebml_weak_ptr<V>&);

    template<typename U, typename V>
    friend ebml_weak_ptr<U> ebml_dynamic_pointer_cast(const ebml_weak_ptr<V>&);

    protected:
        mutable std::mutex mutex;
        mutable control_block* ctl = nullptr;
        T* ptr = nullptr;

    private:
        void _incref(control_block& ctl) {
            auto lock = std::unique_lock<std::mutex>(ctl.mutex);
            ++ctl.weakcount;
        }

        bool _decref(control_block& ctl) {
            auto lock = std::unique_lock<std::mutex>(ctl.mutex);
            --ctl.weakcount;
            return (ctl.strongcount <= 0) and (ctl.weakcount <= 0);
        }

        void _destroyctl() {
            delete ctl;
            ctl = nullptr;
            ptr = nullptr;
        }

    protected:
        explicit ebml_weak_ptr(control_block* ctl, T* ptr) : ctl(ctl), ptr(ptr) {
            if (ctl != nullptr) {
                _incref(*ctl);
            }
        };

    public:
        ebml_weak_ptr() : ebml_weak_ptr(nullptr) {}
        ebml_weak_ptr(const std::nullptr_t&) : ebml_weak_ptr(nullptr, nullptr) {}

        explicit ebml_weak_ptr(const ebml_shared_ptr<T>& sharedPtr) {
            auto lock = std::unique_lock<std::mutex>(sharedPtr.mutex);

            ctl = sharedPtr.ctl;
            ptr = sharedPtr.ptr;

            if (ctl != nullptr) {
                _incref(*ctl);
            }
        }

        ebml_weak_ptr(const ebml_weak_ptr<T>& other) {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            ctl = other.ctl;
            ptr = other.ptr;

            if (ctl != nullptr) {
                _incref(*ctl);
            }
        }

        ebml_weak_ptr(ebml_weak_ptr<T>&& other) {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            ctl = std::exchange(other.ctl, nullptr);
            ptr = std::exchange(other.ptr, nullptr);
        }

        ebml_weak_ptr<T>& operator=(const ebml_weak_ptr<T>& other) {
            if (&other == this) {
                return *this;
            }

            using l = std::unique_lock<std::mutex>;
            using p = std::pair<l, l>;
            p lockpair = (&mutex < &other.mutex) ? p({l(mutex), l(other.mutex)}) : p({l(other.mutex), l(mutex)});

            if (other.ctl == ctl) {
                return *this;
            }

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }

            if (other.ctl != nullptr) {
                this->ctl = other.ctl;
                this->ptr = other.ptr;
                _incref(*ctl);
            }

            return *this;
        }

        ebml_weak_ptr<T>& operator=(ebml_weak_ptr<T>&& other) {
            if (&other == this) {
                return *this;
            }

            using l = std::unique_lock<std::mutex>;
            using p = std::pair<l, l>;
            p lockpair = (&mutex < &other.mutex) ? p({l(mutex), l(other.mutex)}) : p({l(other.mutex), l(mutex)});

            if (other.ctl == ctl) {
                if (ctl != nullptr) {
                    _decref(*ctl);
                }

                other.ctl = nullptr;
                other.ptr = nullptr;
                return *this;
            }

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }

            if (other.ctl != nullptr) {
                this->ctl = std::exchange(other.ctl, nullptr);
                this->ptr = std::exchange(other.ptr, nullptr);
            }

            return *this;
        }

        ~ebml_weak_ptr() {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                if (_decref(*ctl)) {
                    _destroyctl();
                }
            }
        }

        ebml_shared_ptr<T> lock() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                auto& c = *ctl;
                auto lock = std::unique_lock<std::mutex>(c.mutex);
                return ebml_shared_ptr<T>(ctl, ptr, lock);
            }

            return ebml_shared_ptr<T>();
        }

        ebml_shared_ptr<const T> c_lock() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                auto& c = *ctl;
                auto lock = std::unique_lock<std::mutex>(c.mutex);
                return ebml_shared_ptr<const T>(ctl, ptr, lock);
            }

            return ebml_shared_ptr<const T>();
        }

        template<typename U>
        ebml_shared_ptr<U> lock() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                auto& c = *ctl;
                auto lock = std::unique_lock<std::mutex>(c.mutex);

                if (auto recast = dynamic_cast<U*>(ptr)) {
                    return ebml_shared_ptr<U>(ctl, recast, lock);
                }
            }

            return ebml_shared_ptr<U>();
        }

        template<typename U>
        std::enable_if_t<!std::is_same<T, U>::value, ebml_shared_ptr<U>> c_lock() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                auto& c = *ctl;
                auto lock = std::unique_lock<std::mutex>(c.mutex);

                const T* cptr = ptr;

                if (auto recast = dynamic_cast<U*>(cptr)) {
                    return ebml_shared_ptr<U>(ctl, recast, lock);
                }
            }

            return ebml_shared_ptr<U>();
        }

        bool expired() const {
            auto lock = std::unique_lock<std::mutex>(mutex);

            if (ctl != nullptr) {
                auto& c = *ctl;
                auto lock = std::unique_lock<std::mutex>(c.mutex);
                return !c.valid;
            }

            return true;
        }
    };

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_dynamic_pointer_cast(const ebml_shared_ptr<U>& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = dynamic_cast<T*>(other.ptr);

                if (recast != nullptr) {
                    return ebml_shared_ptr<T>(other.ctl, recast);
                }
            }
        }

        return ebml_shared_ptr<T>();
    }

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_dynamic_pointer_cast(ebml_shared_ptr<U>&& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = dynamic_cast<T*>(other.ptr);

                if (recast != nullptr) {
                    ebml_shared_ptr<T> result;
                    result.ctl = std::exchange(other.ctl, nullptr);
                    result.ptr = recast;
                    other.ptr = nullptr;
                    return result;
                }
            }
        }

        return ebml_shared_ptr<T>();
    }

    template<typename T, typename U>
    ebml_weak_ptr<T> ebml_dynamic_pointer_cast(const ebml_weak_ptr<U>& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = dynamic_cast<T*>(other.ptr);

                if (recast != nullptr) {
                    return ebml_weak_ptr<T>(other.ctl, recast);
                }
            }
        }

        return ebml_weak_ptr<T>();
    }

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_static_pointer_cast(const ebml_shared_ptr<U>& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = static_cast<T*>(other.ptr);
                return ebml_shared_ptr<T>(other.ctl, recast);
            }
        }

        return ebml_shared_ptr<T>();
    }

    template<typename T, typename U>
    ebml_weak_ptr<T> ebml_static_pointer_cast(const ebml_weak_ptr<U>& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = static_cast<T*>(other.ptr);
                return ebml_weak_ptr<T>(other.ctl, recast);
            }
        }

        return ebml_weak_ptr<T>();
    }

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_static_pointer_cast(ebml_shared_ptr<U>&& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = static_cast<T*>(other.ptr);
                ebml_shared_ptr<T> result;
                result.ctl = std::exchange(other.ctl, nullptr);
                result.ptr = recast;
                other.ptr = nullptr;
                return result;
            }
        }

        return ebml_shared_ptr<T>();
    }

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_const_pointer_cast(const ebml_shared_ptr<U>& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = const_cast<T*>(other.ptr);
                return ebml_shared_ptr<T>(other.ctl, recast);
            }
        }

        return ebml_shared_ptr<T>();
    }

    // template<typename T, typename U>
    // ebml_weak_ptr<T> ebml_const_pointer_cast(const ebml_weak_ptr<U>& other) {
    //     {
    //         auto lock = std::unique_lock<std::mutex>(other.mutex);
    //
    //         if (other.ctl != nullptr) {
    //             auto recast = const_cast<T*>(other.ptr);
    //             return ebml_weak_ptr<T>(other.ctl, recast);
    //         }
    //     }
    //
    //     return ebml_weak_ptr<T>();
    // }

    template<typename T, typename U>
    ebml_shared_ptr<T> ebml_const_pointer_cast(ebml_shared_ptr<U>&& other) {
        {
            auto lock = std::unique_lock<std::mutex>(other.mutex);

            if (other.ctl != nullptr) {
                auto recast = const_cast<T*>(other.ptr);
                ebml_shared_ptr<T> result;
                result.ctl = std::exchange(other.ctl, nullptr);
                result.ptr = recast;
                other.ptr = nullptr;
                return result;
            }
        }

        return ebml_shared_ptr<T>();
    }
    // template<typename T>
    // using ptr = std::shared_ptr<T>;
    //
    // template<typename T>
    // using wptr = std::weak_ptr<T>;

    template<typename T>
    using ptr = ebml_shared_ptr<T>;

    template<typename T>
    using wptr = ebml_weak_ptr<T>;

    /**
     * Shared pointer to an ebmlElement instance.
     */

    typedef ptr<ebmlElement> ebmlElement_sp;

    /**
     * Shared pointer to a const ebmlElement instance.
     */

    typedef ptr<const ebmlElement> c_ebmlElement_sp;

    /**
     * Shared pointer to an ebmlMasterElement instance.
     */

    typedef ptr<ebmlMasterElement> ebmlMasterElement_sp;

    /**
     * Shared pointer to a const ebmlMasterElement instance.
     */

    typedef ptr<const ebmlMasterElement> c_ebmlMasterElement_sp;


    /**
     * Shared pointer to an ebmlList instance.
     */

    typedef ptr<ebmlList> ebmlList_sp;

    /**
     * Shared pointer to a const ebmlList instance.
     */

    typedef ptr<const ebmlList> c_ebmlList_sp;

    /**
     * Shared pointer to an ebmlLazyLoad instance.
     */

    typedef ptr<ebmlLazyLoad> ebmlLazyLoad_sp;

    /**
     * Shared pointer to a const ebmlLazyLoad instance.
     */

    typedef ptr<const ebmlLazyLoad> c_ebmlLazyLoad_sp;

    /**
     * Weak pointer to an ebmlElement instance.
     */

    typedef wptr<ebmlElement> ebmlElement_wp;

    /**
     * Weak pointer to a const ebmlElement instance.
     */

    typedef wptr<const ebmlElement> c_ebmlElement_wp;

    /**
     * Weak pointer to an ebmlElement instance.
     */

    typedef wptr<ebmlMasterElement> ebmlMasterElement_wp;

    /**
     * Weak pointer to a const ebmlElement instance.
     */

    typedef wptr<const ebmlMasterElement> c_ebmlMasterElement_wp;
    /**
     * Shared pointer to an ebmlDocument instance.
     */

    typedef std::shared_ptr<ebmlDocument> ebmlDocument_sp;

    /**
     * Weak pointer to an ebmlDocument instance.
     */

    typedef std::weak_ptr<ebmlDocument> ebmlDocument_wp;

    /**
     * Shared pointer to an ioBase instance.
     */

    typedef ptr<ioBase> ioBase_sp;
}
#endif
