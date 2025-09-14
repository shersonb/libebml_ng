#ifndef EBML_NG_ELEMENTCLS_TPP
#define EBML_NG_ELEMENTCLS_TPP

#include "ebmlElementType.h"
#include "ebmlElement.h"
#include "exceptions.h"
#include "typeof.h"

namespace ebml {
    template<typename ebmltype_t, typename ebmlinst_t, typename typebase_t>
    ebmlElement* ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::_decode_v(const parseString& parsed) const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value) {
            ebmlinst_t* elem_p;

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
    ebmlElement* ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::_cdecode_v(const parseString& parsed) const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value) {
            ebmlinst_t* elem_p;

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
    ebmlElement* ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::_decode_v(const parseFile& parsed) const {
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
    ebmlElement* ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::_cdecode_v(const parseFile& parsed) const {
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
    ebmlElement* ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>::_new() const {
        if constexpr (!std::is_abstract<ebmlinst_t>::value and std::is_constructible<ebmlinst_t, const ebmltype_t*>::value) {
            return new ebmlinst_t(static_cast<const ebmltype_t*>(this));
        } else {
            return nullptr;
        }
    }
}
#endif
