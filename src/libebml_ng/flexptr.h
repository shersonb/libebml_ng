#ifndef EBML_NG_FLEXPTR_H
#define EBML_NG_FLEXPTR_H

#include <memory>
#include <utility>
#include <stdexcept>
#include "ptrs.h"  // Assuming that ptr and wptr types are declared in ptrs.h

namespace ebml {

    /**
     * @brief A flexible pointer that can store either a strong (shared) or weak pointer.
     *
     * The flexible_ptr class provides the ability to select at runtime whether the
     * underlying stored pointer is a strong pointer (ebml_shared_ptr) or a weak pointer
     * (ebml_weak_ptr). It also supports both mutable and const types.
     *
     * @tparam T Type of the managed object.
     */
    template<typename T>
    class flexible_ptr {
    private:
        char _flags = 0; ///< Flag to indicate the pointer type:
                         ///< 0x00: ptr<T>, 0x01: wptr<T>, 0x02: ptr<const T>, 0x03: wptr<const T>

        union {
            ptr<T> sp;           ///< Stored strong pointer for mutable T.
            ptr<const T> const_sp; ///< Stored strong pointer for const T.
            wptr<T> wp;          ///< Stored weak pointer for mutable T.
            wptr<const T> const_wp; ///< Stored weak pointer for const T.
        };

        // Initialization functions for the internal pointer (using placement new).
        void _init_ptr() {
            new (&this->sp) ptr<T>();
            this->_flags = 0x00;
        }

        void _init_ptr(const ptr<T>& p) {
            new (&this->sp) ptr<T>(p);
            this->_flags = 0x00;
        }

        void _init_ptr(ptr<T>&& p) {
            new (&this->sp) ptr<T>(std::move(p));
            this->_flags = 0x00;
        }

        void _init_ptr(const wptr<T>& p) {
            new (&this->wp) wptr<T>(p);
            this->_flags = 0x01;
        }

        void _init_ptr(wptr<T>&& p) {
            new (&this->wp) wptr<T>(std::move(p));
            this->_flags = 0x01;
        }

        void _init_ptr(const ptr<const T>& p) {
            new (&this->const_sp) ptr<const T>(p);
            this->_flags = 0x02;
        }

        void _init_ptr(ptr<const T>&& p) {
            new (&this->const_sp) ptr<const T>(std::move(p));
            this->_flags = 0x02;
        }

        void _init_ptr(const wptr<const T>& p) {
            new (&this->const_wp) wptr<const T>(p);
            this->_flags = 0x03;
        }

        void _init_ptr(wptr<const T>&& p) {
            new (&this->const_wp) wptr<const T>(std::move(p));
            this->_flags = 0x03;
        }

        // Reinitializes the pointer: if the flag indicates a strong pointer,
        // its default value is set; otherwise, the current pointer is destroyed.
        void _reinit_ptr() {
            if (this->_flags == 0x00) {
                this->_set_ptr();
                return;
            }
            this->_destroy_ptr();
            this->_init_ptr();
        }

        // Set pointer members (copy/move assign)
        void _set_ptr() {
            this->sp = nullptr;
        }

        void _set_ptr(const ptr<T>& p) {
            this->sp = p;
        }

        void _set_ptr(ptr<T>&& p) {
            this->sp = std::move(p);
        }

        void _set_ptr(const ptr<const T>& p) {
            this->const_sp = p;
        }

        void _set_ptr(ptr<const T>&& p) {
            this->const_sp = std::move(p);
        }

        void _set_ptr(const wptr<T>& p) {
            this->wp = p;
        }

        void _set_ptr(wptr<T>&& p) {
            this->wp = std::move(p);
        }

        void _set_ptr(const wptr<const T>& p) {
            this->const_wp = p;
        }

        void _set_ptr(wptr<const T>&& p) {
            this->const_wp = std::move(p);
        }

        // Destroys the stored pointer, calling the appropriate destructor
        // based on the flag bits stored in _flags.
        void _destroy_ptr() {
            unsigned char flags = this->_flags & 0x03;
            switch (flags) {
                case 0x00: {
                    using p = ptr<T>;
                    sp.~p();
                    break;
                }
                case 0x01: {
                    using p = wptr<T>;
                    wp.~p();
                    break;
                }
                case 0x02: {
                    using p = ptr<const T>;
                    const_sp.~p();
                    break;
                }
                case 0x03: {
                    using p = wptr<const T>;
                    const_wp.~p();
                    break;
                }
            }
        }

    public:
        /// @brief Default constructor. Initializes with an empty strong pointer.
        flexible_ptr() {
            _init_ptr();
        }

        /// @brief Constructs a flexible_ptr that is null.
        flexible_ptr(const std::nullptr_t&) {
            _init_ptr();
        }

        /**
         * @brief Copy constructor.
         *
         * Creates a flexible_ptr by copying from another.
         *
         * @param other The source flexible_ptr.
         */
        flexible_ptr(const flexible_ptr<T>& other) {
            unsigned char flags = other._flags & 0x03;
            switch (flags) {
                case 0x00: {
                    this->_init_ptr(other.sp);
                    break;
                }
                case 0x01: {
                    this->_init_ptr(other.wp);
                    break;
                }
                case 0x02: {
                    this->_init_ptr(other.const_sp);
                    break;
                }
                case 0x03: {
                    this->_init_ptr(other.const_wp);
                    break;
                }
            }
        }

        /**
         * @brief Move constructor.
         *
         * Transfers ownership from another flexible_ptr.
         *
         * @param other The source flexible_ptr to move.
         */
        flexible_ptr(flexible_ptr<T>&& other) {
            unsigned char flags = other._flags & 0x03;
            switch (flags) {
                case 0x00: {
                    this->_init_ptr(std::move(other.sp));
                    break;
                }
                case 0x01: {
                    this->_init_ptr(std::move(other.wp));
                    other._reinit_ptr();
                    break;
                }
                case 0x02: {
                    this->_init_ptr(std::move(other.const_sp));
                    other._reinit_ptr();
                    break;
                }
                case 0x03: {
                    this->_init_ptr(std::move(other.const_wp));
                    other._reinit_ptr();
                    break;
                }
            }
        }

        /**
         * @brief Constructs a flexible_ptr from a strong pointer to T.
         *
         * @param p The strong pointer (ptr<T>) to be stored.
         */
        flexible_ptr(const ptr<T>& p) {
            this->_init_ptr(p);
        }

        /**
         * @brief Constructs a flexible_ptr from a movable strong pointer to T.
         *
         * @param p The strong pointer (ptr<T>) to be moved in.
         */
        flexible_ptr(ptr<T>&& p) {
            this->_init_ptr(std::move(p));
        }

        /**
         * @brief Constructs a flexible_ptr from a strong pointer to a const T.
         *
         * @param p The strong pointer (ptr<const T>) to be stored.
         */
        flexible_ptr(const ptr<const T>& p) {
            this->_init_ptr(p);
        }

        /**
         * @brief Constructs a flexible_ptr from a movable strong pointer to a const T.
         *
         * @param p The strong pointer (ptr<const T>) to be moved in.
         */
        flexible_ptr(ptr<const T>&& p) {
            this->_init_ptr(std::move(p));
        }

        /**
         * @brief Constructs a flexible_ptr from a weak pointer to T.
         *
         * @param p The weak pointer (wptr<T>) to be stored.
         */
        flexible_ptr(const wptr<T>& p) {
            this->_init_ptr(p);
        }

        /**
         * @brief Constructs a flexible_ptr from a movable weak pointer to T.
         *
         * @param p The weak pointer (wptr<T>) to be moved in.
         */
        flexible_ptr(wptr<T>&& p) {
            this->_init_ptr(std::move(p));
        }

        /**
         * @brief Constructs a flexible_ptr from a weak pointer to a const T.
         *
         * @param p The weak pointer (wptr<const T>) to be stored.
         */
        flexible_ptr(const wptr<const T>& p) {
            this->_init_ptr(p);
        }

        /**
         * @brief Constructs a flexible_ptr from a movable weak pointer to a const T.
         *
         * @param p The weak pointer (wptr<const T>) to be moved in.
         */
        flexible_ptr(wptr<const T>&& p) {
            this->_init_ptr(std::move(p));
        }

        /**
         * @brief Destructor.
         *
         * Calls the appropriate destructor for the underlying pointer.
         */
        ~flexible_ptr() {
            this->_destroy_ptr();
        }

        /**
         * @brief Copy assignment operator.
         *
         * Assigns from another flexible_ptr.
         *
         * @param other The source flexible_ptr.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(const flexible_ptr<T>& other) {
            if (this == &other) {
                return *this;
            }

            if (this->_flags == other._flags) {
                switch (this->_flags) {
                    case 0x00: {
                        this->_set_ptr(other.sp);
                        break;
                    }
                    case 0x01: {
                        this->_set_ptr(other.wp);
                        break;
                    }
                    case 0x02: {
                        this->_set_ptr(other.const_sp);
                        break;
                    }
                    case 0x03: {
                        this->_set_ptr(other.const_wp);
                        break;
                    }
                }
                return *this;
            }

            this->_destroy_ptr();
            switch (other._flags) {
                case 0x00: {
                    this->_init_ptr(other.sp);
                    break;
                }
                case 0x01: {
                    this->_init_ptr(other.wp);
                    break;
                }
                case 0x02: {
                    this->_init_ptr(other.const_sp);
                    break;
                }
                case 0x03: {
                    this->_init_ptr(other.const_wp);
                    break;
                }
            }

            return *this;
        }

        /**
         * @brief Move assignment operator.
         *
         * Transfers the pointer held by another flexible_ptr.
         *
         * @param other The source flexible_ptr.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(flexible_ptr<T>&& other) {
            if (this == &other) {
                return *this;
            }

            if (this->_flags == other._flags) {
                switch (this->_flags) {
                    case 0x00: {
                        this->_set_ptr(std::move(other.sp));
                        break;
                    }
                    case 0x01: {
                        this->_set_ptr(std::move(other.wp));
                        other._reinit_ptr();
                        break;
                    }
                    case 0x02: {
                        this->_set_ptr(std::move(other.const_sp));
                        other._reinit_ptr();
                        break;
                    }
                    case 0x03: {
                        this->_set_ptr(std::move(other.const_wp));
                        other._reinit_ptr();
                        break;
                    }
                }
                return *this;
            }

            this->_destroy_ptr();
            switch (other._flags) {
                case 0x00: {
                    this->_init_ptr(std::move(other.sp));
                    break;
                }
                case 0x01: {
                    this->_init_ptr(std::move(other.wp));
                    other._reinit_ptr();
                    break;
                }
                case 0x02: {
                    this->_init_ptr(std::move(other.const_sp));
                    other._reinit_ptr();
                    break;
                }
                case 0x03: {
                    this->_init_ptr(std::move(other.const_wp));
                    other._reinit_ptr();
                    break;
                }
            }

            return *this;
        }

        /**
         * @brief Assignment operator from a strong pointer of type ptr<T>.
         *
         * @param other The strong pointer to assign from.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(const ptr<T>& other) {
            if (this->_flags == 0x00) {
                this->_set_ptr(other);
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        /**
         * @brief Assignment operator from a movable strong pointer of type ptr<T>.
         *
         * @param other The strong pointer to move in.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(ptr<T>&& other) {
            if (this->_flags == 0x00) {
                this->_set_ptr(std::move(other));
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        /**
         * @brief Assignment operator from a weak pointer of type wptr<T>.
         *
         * @param other The weak pointer to assign from.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(const wptr<T>& other) {
            if (this->_flags == 0x02) {
                this->_set_ptr(other);
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        /**
         * @brief Assignment operator from a movable weak pointer of type wptr<T>.
         *
         * @param other The weak pointer to move in.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(wptr<T>&& other) {
            if (this->_flags == 0x01) {
                this->_set_ptr(std::move(other));
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        /**
         * @brief Assignment operator from a strong pointer of type ptr<const T>.
         *
         * @param other The strong pointer to const T to assign from.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(const ptr<const T>& other) {
            if (this->_flags == 0x02) {
                this->_set_ptr(other);
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        /**
         * @brief Assignment operator from a movable strong pointer of type ptr<const T>.
         *
         * @param other The strong pointer to const T to move in.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(ptr<const T>&& other) {
            if (this->_flags == 0x02) {
                this->_set_ptr(std::move(other));
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        /**
         * @brief Assignment operator from a weak pointer of type wptr<const T>.
         *
         * @param other The weak pointer to assign from.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(const wptr<const T>& other) {
            if (this->_flags == 0x03) {
                this->_set_ptr(other);
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        /**
         * @brief Assignment operator from a movable weak pointer of type wptr<const T>.
         *
         * @param other The weak pointer to move in.
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(wptr<const T>&& other) {
            if (this->_flags == 0x03) {
                this->_set_ptr(std::move(other));
                return *this;
            }
            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        /**
         * @brief Assignment operator from nullptr.
         *
         * Resets the flexible_ptr to an empty strong pointer.
         *
         * @return Reference to this flexible_ptr.
         */
        flexible_ptr<T>& operator=(const std::nullptr_t&) {
            this->_reinit_ptr();
            return *this;
        }

        /**
         * @brief Checks whether the stored pointer is pointing to a const object.
         *
         * @return true if the stored pointer is const, false otherwise.
         */
        bool is_const() const {
            return (this->_flags & 0x02);
        }

        /**
         * @brief Checks whether the stored pointer is a weak pointer.
         *
         * @return true if the stored pointer is weak, false otherwise.
         */
        bool is_weak() const {
            return (this->_flags & 0x01);
        }

        /**
         * @brief Retrieves the stored strong pointer.
         *
         * Returns a strong pointer to T. Throws if the underlying pointer is const.
         *
         * @return ptr<T> The strong pointer to T.
         * @throws std::runtime_error if non-const access is requested on a const object.
         */
        ptr<T> get() const {
            if (this->is_const()) {
                throw std::runtime_error("non-const access to const object");
            }
            unsigned char flags = this->_flags & 0x03;
            switch (flags) {
                case 0x00: {
                    return this->sp;
                }
                case 0x01: {
                    return this->wp.lock();
                }
            }
            throw std::runtime_error("this should never happen");
        }

        /**
         * @brief Retrieves the stored strong pointer to a const T.
         *
         * Always returns a strong pointer that treats the managed object as const.
         *
         * @return ptr<const T> The strong const pointer.
         * @throws std::runtime_error if an unexpected flag value is encountered.
         */
        ptr<const T> cget() const {
            unsigned char flags = this->_flags & 0x03;
            switch (flags) {
                case 0x00: {
                    return ebml::ebml_const_pointer_cast<const T>(sp);
                }
                case 0x01: {
                    return ebml::ebml_const_pointer_cast<const T>(wp.lock());
                }
                case 0x02: {
                    return const_sp;
                }
                case 0x03: {
                    return const_wp.lock();
                }
            }
            throw std::runtime_error("this should never happen");
        }

        /**
         * @brief Checks whether the stored pointer is not null.
         *
         * The check is performed based on the type of pointed object (strong or weak).
         *
         * @return true if the underlying pointer is non-null (and not expired for weak pointers), false otherwise.
         * @throws std::runtime_error if an unexpected flag value is encountered.
         */
        bool notnull() const {
            unsigned char flags = this->_flags & 0x03;
            switch (flags) {
                case 0x00: {
                    return this->sp != nullptr;
                }
                case 0x01: {
                    return not this->wp.expired();
                }
                case 0x02: {
                    return this->const_sp != nullptr;
                }
                case 0x03: {
                    return not this->const_wp.expired();
                }
            }
            throw std::runtime_error("this should never happen");
        }
    };

} // namespace ebml

#endif
