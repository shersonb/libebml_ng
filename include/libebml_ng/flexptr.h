#ifndef EBML_NG_FLEXPTR_H
#define EBML_NG_FLEXPTR_H

#include <memory>
#include <utility>

namespace ebml {
    template<typename T>
    class flexible_ptr {
    private:
        char _flags = 0;
        union {
            ptr<T> sp;
            ptr<const T> const_sp;
            wptr<T> wp;
            wptr<const T> const_wp;
        };

        // Initialize pointer member (placement new)

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

        // Reinitalize pointer

        void _reinit_ptr() {
            if (this->_flags == 0x00) {
                this->_set_ptr();
                return;
            }

            this->_destroy_ptr();
            this->_init_ptr();;
        }

        // Set pointer member (copy/move assign)

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

        // Destroy pointer

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
        flexible_ptr() {
            _init_ptr();
        }
        flexible_ptr(const std::nullptr_t&) {
            _init_ptr();
        }
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
        flexible_ptr(const ptr<T>& p) {
            this->_init_ptr(p);
        }
        flexible_ptr(ptr<T>&& p) {
            this->_init_ptr(std::move(p));
        }
        flexible_ptr(const ptr<const T>& p) {
            this->_init_ptr(p);
        }
        flexible_ptr(ptr<const T>&& p) {
            this->_init_ptr(std::move(p));
        }
        flexible_ptr(const wptr<T>& p) {
            this->_init_ptr(p);
        }
        flexible_ptr(wptr<T>&& p) {
            this->_init_ptr(std::move(p));
        }
        flexible_ptr(const wptr<const T>& p) {
            this->_init_ptr(p);
        }
        flexible_ptr(wptr<const T>&& p) {
            this->_init_ptr(std::move(p));
        }

        ~flexible_ptr() {
            this->_destroy_ptr();
        }

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
                    break;
                    other._reinit_ptr();
                }
            }

            return *this;
        }

        flexible_ptr<T>& operator=(const ptr<T>& other) {
            if (this->_flags == 0x00) {
                this->_set_ptr(other);
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        flexible_ptr<T>& operator=(ptr<T>&& other) {
            if (this->_flags == 0x00) {
                this->_set_ptr(std::move(other));
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        flexible_ptr<T>& operator=(const wptr<T>& other) {
            if (this->_flags == 0x02) {
                this->_set_ptr(other);
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        flexible_ptr<T>& operator=(wptr<T>&& other) {
            if (this->_flags == 0x01) {
                this->_set_ptr(std::move(other));
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        flexible_ptr<T>& operator=(const ptr<const T>& other) {
            if (this->_flags == 0x02) {
                this->_set_ptr(other);
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        flexible_ptr<T>& operator=(ptr<const T>&& other) {
            if (this->_flags == 0x02) {
                this->_set_ptr(std::move(other));
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        flexible_ptr<T>& operator=(const wptr<const T>& other) {
            if (this->_flags == 0x03) {
                this->_set_ptr(other);
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(other);
            return *this;
        }

        flexible_ptr<T>& operator=(wptr<const T>&& other) {
            if (this->_flags == 0x03) {
                this->_set_ptr(std::move(other));
                return *this;
            }

            this->_destroy_ptr();
            this->_init_ptr(std::move(other));
            return *this;
        }

        flexible_ptr<T>& operator=(const std::nullptr_t&) {
            this->_reinit_ptr();
            return *this;
        }

        bool is_const() const {
            return (this->_flags & 0x02);
        }

        bool is_weak() const {
            return (this->_flags & 0x01);
        }

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
}
#endif
