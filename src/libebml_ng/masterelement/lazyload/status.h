#ifndef EBML_NG_LAZYLOAD_STATUS_H
#define EBML_NG_LAZYLOAD_STATUS_H

#include <string>
#include "../../ebmlElement.h"
// #include "../base.h"
// #include "../seekdata.h"
// #include "../exceptions.h"
// #include "../ebmlDataElement.h"


namespace ebml {
    template<typename T>
    class status_t {
    protected:
        bool result;
        union {
            std::string reason;
        };
        T _data;

    public:
        status_t(bool);
        status_t(bool, const T&);
        status_t(bool, T&&);

        // template<typename... Args>
        // status_t(bool, Args...);

        status_t(bool, const std::string&);
        status_t(bool, std::string&&);
        status_t(bool, const char*);

        status_t(bool, const std::string&, const T&);
        status_t(bool, const char*, const T&);
        status_t(bool, std::string&&, T&&);
        status_t(bool, const char*, T&&);

        // template<typename... Args>
        // status_t(bool, const std::string&, Args...);

        // template<typename... Args>
        // status_t(bool, std::string&&, Args...);

        status_t(const status_t&);
        status_t(status_t&&);
        ~status_t();

        status_t& operator=(const status_t&);
        status_t& operator=(status_t&&);

        operator bool() const;
        operator const T&() const;
        const T& data() const;
        operator std::string() const;
        void throw_exc(const ptr<const ebmlElement>&) const;
        void throw_exc(ptr<const ebmlElement>&&) const;
    };
}

#endif
