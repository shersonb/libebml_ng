#ifndef EBML_NG_PTRS_H
#define EBML_NG_PTRS_H

#include <utility>
#include "forwards.h"
#include <memory>
#include <mutex>
#include <iostream>

namespace ebml {
    /**
     * @defgroup ptr_module Pointers and Memory Management
     * @brief Classes and templates for managing object ownership and memory.
     *
     * This module defines the custom smart pointer types and other utilities used
     * internally by the library to ensure safe and efficient memory management.
     */

    struct control_block {
        std::mutex mutex;
        long long strongcount = 0;
        long long weakcount = 0;
        bool valid = false;
    };

    template<typename T>
    class ebml_shared_ptr;

    template<typename T>
    class ebml_weak_ptr;

    /**
     * @ingroup ptr_module
     * @{
     */
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

    /**
     * @}
     */

    /**
     * @ingroup ptr_module
     * @brief A drop-in replacement for std::shared_ptr tailored for EBML objects.
     *
     * This custom smart pointer, `ebml_shared_ptr`, is designed to mimic the behavior of
     * std::shared_ptr while supporting a unique lifetime management scheme required
     * by the EBML library.
     *
     * **Key Features and Usage Guidelines:**
     * - **Controlled Self-Referencing:**
     *   Although self-referencing (i.e. creating a shared pointer from `this`) may be necessary
     *   in some designs, it is considered unsafe to do so from within an object's constructor in
     *   conjunction with ebml_shared_ptr. The internal control block's `valid` flag is set to true
     *   the moment the first shared pointer is created.
     *
     * - **Safe Management of Lifetime:**
     *   Once the first ebml_shared_ptr is created for an object, the `valid` member is marked as true.
     *   After this point, the object is considered active and further manipulation of the `valid`
     *   member is not recommended.
     *
     * - **Error Prevention:**
     *   This mechanism avoids double deletion scenarios; if an error occurs during construction, it is
     *   the caller’s responsibility to ensure that no shared pointer is created prematurely (i.e., within
     *   the object's constructor).
     *
     * **Note for Subclassing:**
     * - For classes derived from `ebmlElement`, the construction of the internal weak pointer is already
     *   handled by the base class. Users are only expected to use these custom pointer types (ebml_shared_ptr
     *   and ebml_weak_ptr) for other classes.
     *
     * @tparam T The type of the object being managed.
     */
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

        /**
         * @brief Constructs an ebml_shared_ptr that takes ownership of @p ptr.
         *
         * When the first ebml_shared_ptr for the object is created, the control block's `valid` member is
         * set to true. It is then unsafe to modify the `valid` flag or create additional shared pointers
         * from within the object's constructor.
         *
         * @param ptr Pointer to the object to be managed.
         */
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

            ctl = other.ctl;
            ptr = other.ptr;

            if (ctl != nullptr) {
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

            ctl = std::exchange(other.ctl, nullptr);
            ptr = std::exchange(other.ptr, nullptr);

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
                    ctl = other.ctl;
                    ptr = recast;
                    _incref(*ctl);
                } else {
                    ctl = nullptr;
                    ptr = nullptr;
                }
            } else {
                ctl = nullptr;
                ptr = nullptr;
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

        /**
         * @brief Retrieves the raw pointer to the managed object.
         *
         * This function returns the pointer stored in the control block. It is expected that the control block's
         * `valid` flag is already set to true (which happens when the first ebml_shared_ptr is constructed).
         *
         * @return T* Pointer to the managed object.
         */
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

    /**
     * @ingroup ptr_module
     * @brief A drop-in replacement for std::weak_ptr tailored for EBML objects.
     *
     * The ebml_weak_ptr provides a safe mechanism to observe an object managed
     * by ebml_shared_ptr without affecting its lifetime.
     *
     * @tparam T Type of the object being observed.
     */
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
