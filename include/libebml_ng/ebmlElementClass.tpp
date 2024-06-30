#ifndef EBML_NG_ELEMENTCLS_TPP
#define EBML_NG_ELEMENTCLS_TPP

#include "ebmlElementClass.h"
#include "ebmlElement.h"
#include "exceptions.h"
// #include <exception>

namespace ebml {
    // /**
    // * Convenience member function that dynamically casts
    // * ebmlElementClass to a subclass.
    // *
    // * @return Reference to instance as a T object.
    // *
    // * Example:
    // * @code
    // * const ebmlElementClass cls = ebmlVoidClass();
    // * const ebmlVoidClass& Void = cls.as<ebmlVoidClass>();
    // * @endcode
    // * @throws std::bad_cast If instance is not an instance of T.
    // */

    /**
    * Convenience member function that dynamically casts
    * const ebmlElementClass to a subclass.
    *
    * @return Const reference to instance as a T object.
    *
    * Example:
    * @code
    * const ebmlElementClass cls = ebmlVoidClass();
    * const ebmlVoidClass& Void = cls.as<ebmlVoidClass>();
    * @endcode
    * @throws std::bad_cast If instance is not an instance of T.
    */

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // ebmlElement* ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::_decode(const parseString& parsed) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), parsed);
    //     elem->_validate();
    //     return elem;
    // }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    ebmlElement* ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::_decode_v(const parseString& parsed) const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value) {
            ebmlinst_t* elem_p;

            // if constexpr (std::is_constructible<ebmlinst_t, const ebmltype_t*, const parseString&>::value) {
            if constexpr (has_constructor<ebmlinst_t, const ebmltype_t*, const parseString&>) {
                elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this), parsed);
                elem_p->_validate();
                return elem_p;
            } else if constexpr (has_init<ebmlinst_t, const parseString&>) {
                elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this));
                auto& elem = *elem_p;

                try {
                    elem._init(parsed);
                } catch (const std::exception& e) {
                    delete elem_p;
                    throw;
                }

                elem._validate();
                return elem_p;
            } else {
                throw std::runtime_error("No decoding logic defined.");
            }
        } else {
            return nullptr;
        }
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    ebmlElement* ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::_cdecode_v(const parseString& parsed) const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value) {
            ebmlinst_t* elem_p;

            // if constexpr (std::is_constructible<ebmlinst_t, const ebmltype_t*, const parseString&>::value) {
            if constexpr (has_constructor<ebmlinst_t, const ebmltype_t*, const parseString&>) {
                elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this), parsed);
            } else {
                elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this));
            }

            auto& elem = *elem_p;

            if constexpr (has_cinit<ebmlinst_t, parseString>) {
                try {
                    elem._cinit(parsed);
                } catch (const std::exception& e) {
                    delete elem_p;
                    throw;
                }
            } else if constexpr (has_init<ebmlinst_t, parseString>) {
                try {
                    elem._init(parsed);
                } catch (const std::exception& e) {
                    delete elem_p;
                    throw;
                }
            }

            elem._validate();

            return elem_p;
        } else {
            return nullptr;
        }
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    ebmlElement* ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::_decode_v(const parseFile& parsed) const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value) {
            if constexpr (std::is_constructible<ebmlinst_t, const ebmltype_t*, const parseFile&>::value) {
                auto elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this), parsed);
                auto& elem = *elem_p;

                if constexpr (has_init<ebmlinst_t, parseFile>) {
                    try {
                        elem._init(parsed);
                    } catch (const std::exception& e) {
                        delete elem_p;
                        throw;
                    }
                }

                elem._validate();
                return elem_p;
            } else if constexpr (has_init<ebmlinst_t, parseFile>) {
                auto elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this));
                auto& elem = *elem_p;

                try {
                    elem._init(parsed);
                } catch (const std::exception& e) {
                    delete elem_p;
                    throw;
                }

                elem._validate();
                return elem_p;
            } else {
                auto data = std::make_unique<char[]>(parsed.dataSize);
                auto s_parsed = parseString(parsed, data.get());
                return this->_decode(s_parsed);
            }
        } else {
            return nullptr;
        }
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    ebmlElement* ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::_cdecode_v(const parseFile& parsed) const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value) {
            if constexpr (std::is_constructible<ebmlinst_t, const ebmltype_t*, const parseFile&>::value) {
                auto elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this), parsed);
                auto& elem = *elem_p;
                elem._validate();

                if constexpr (has_cinit<ebmlinst_t, parseFile>) {
                    try {
                        elem._cinit(parsed);
                    } catch (const std::exception& e) {
                        delete elem_p;
                        throw;
                    }
                } else if constexpr (has_init<ebmlinst_t, parseFile>) {
                    try {
                        elem._init(parsed);
                    } catch (const std::exception& e) {
                        delete elem_p;
                        throw;
                    }
                }

                return elem_p;
            } else if constexpr (has_cinit<ebmlinst_t, parseFile>) {
                auto elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this));
                auto& elem = *elem_p;

                try {
                    elem._cinit(parsed);
                } catch (const std::exception& e) {
                    delete elem_p;
                    throw;
                }

                elem._validate();
                return elem_p;
            } else if constexpr (has_init<ebmlinst_t, parseFile>) {
                auto elem_p = new ebmlinst_t(static_cast<const ebmltype_t*>(this));
                auto& elem = *elem_p;

                try {
                    elem._init(parsed);
                } catch (const std::exception& e) {
                    delete elem_p;
                    throw;
                }

                elem._validate();
                return elem_p;
            } else {
                auto data = std::make_unique<char[]>(parsed.dataSize);
                auto s_parsed = parseString(parsed, data.get());
                return _cdecode_v(s_parsed);
            }
        } else {
            return nullptr;
        }
    }

    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    ebmlElement* ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::_new() const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value and std::is_constructible<ebmlinst_t, const ebmltype_t*>::value) {
            return new ebmlinst_t(static_cast<const ebmltype_t*>(this));
        } else {
            return nullptr;
        }
    }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // ebmlElement* ClsMixin<ebmltype_t, ebmlinst_t>::_cdecode(const parseFile& parsed) const {
    //     auto elem = new ebmlinst_t(parsed);
    //     elem->_validate();
    //     return elem;
    // }

    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // template<typename... Args>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::operator()(Args... args) const {
    //     auto elem = new ebmlinst_t(static_cast<const ebmltype_t*>(this), args...);
    //     elem->_validate();
    //     return elem->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const parseString& parsed) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(parsed))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const parseFile& parsed) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(parsed))->sp();
    // }
    //
    // // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // // ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const char* data, size_t size) const {
    // //     return static_cast<ebmlinst_t*>(this->_decode(data, size))->sp();
    // // }
    //
    // // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // // ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const std::string& data) const {
    // //     return static_cast<ebmlinst_t*>(this->_decode(data))->sp();
    // // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file, off_t& offset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file, off_t& offset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(const ioBase_sp& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset, endoffset))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::decode(ioBase& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<ebmlinst_t*>(this->_decode(file, offset, endoffset))->sp();
    // }
    //
    //
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const parseString& parsed) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(parsed))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const parseFile& parsed) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(parsed))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const char* data, size_t size) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(data, size))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const std::string& data) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(data))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file, off_t& offset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file, off_t& offset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(const ioBase_sp& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset, endoffset))->sp();
    // }
    //
    // template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    // inline ebml::ptr<const ebmlinst_t> ClsMixin<ebmltype_t, ebmlinst_t, typebase_t>::cdecode(ioBase& file, off_t offset, off_t& endoffset) const {
    //     return static_cast<const ebmlinst_t*>(this->_cdecode(file, offset, endoffset))->sp();
    // }
    //
}
#endif
