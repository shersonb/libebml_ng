#ifndef EBML_NG_LAZYLOAD_STATUS_TPP
#define EBML_NG_LAZYLOAD_STATUS_TPP

#include "status.h"

namespace ebml {
    template<typename T>
    status_t<T>::status_t(bool result) : result(result) {
        if (!result) {
            new (&this->reason) std::string();
        }
    }

    template<typename T>
    status_t<T>::status_t(bool result, const T& data) : result(result), _data(data) {
        if (!result) {
            new (&this->reason) std::string();
        }
    }

    template<typename T>
    status_t<T>::status_t(bool result, const std::string& reason, const T& data) : result(result), _data(data) {
        if (!result) {
            new (&this->reason) std::string(reason);
        }
    }

    template<typename T>
    status_t<T>::status_t(bool result, const char* reason, const T& data) : result(result), _data(data) {
        if (!result) {
            new (&this->reason) std::string(reason);
        }
    }

    template<typename T>
    status_t<T>::status_t(bool result, T&& data) : result(result), _data(std::move(data)) {
        if (!result) {
            new (&this->reason) std::string();
        }
    }

    template<typename T>
    status_t<T>::status_t(bool result, std::string&& reason, T&& data) : result(result), _data(data) {
        if (!result) {
            new (&this->reason) std::string(std::move(reason));
        }
    }

    template<typename T>
    status_t<T>::status_t(bool result, const char* reason, T&& data) : result(result), _data(data) {
        if (!result) {
            new (&this->reason) std::string(reason);
        }
    }

    template<typename T>
    const T& status_t<T>::data() const {
        return this->_data;
    }

    template<typename T>
    status_t<T>::~status_t() {
        if (!this->result) {
            using p = std::string;
            this->reason.~p();
        }
    }

    template<typename T>
    status_t<T>::operator bool() const {
        return this->result;
    }

    template<typename T>
    void status_t<T>::throw_exc(const c_ebmlElement_sp& elem) const {
        _data.throw_exc(reason, elem);
    }

    template<typename T>
    void status_t<T>::throw_exc(c_ebmlElement_sp&& elem) const {
        _data.throw_exc(reason, std::move(elem));
    }
}

#endif
