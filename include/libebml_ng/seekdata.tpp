#ifndef EBML_NG_SEEKDATA_TPP
#define EBML_NG_SEEKDATA_TPP

#include "seekdata.h"

namespace ebml {
    template<typename T>
    seekDataWithKey_t<T>& seekData_t::withKeyType() {
        if (auto recast = std::dynamic_pointer_cast<seekDataWithKey_t<T>*>(this)) {
            return *recast;
        }
        throw std::bad_alloc();
    }

    template<typename T>
    const seekDataWithKey_t<T>& seekData_t::withKeyType() const {
        if (auto recast = std::dynamic_pointer_cast<const seekDataWithKey_t<T>*>(this)) {
            return *recast;
        }
        throw std::bad_alloc();
    }

    template<typename T>
    T& seekData_t::key() {
        return this->withKeyType<T>().key;
    }

    template<typename T>
    const T& seekData_t::key() const {
        return this->withKeyType<T>().key;
    }


    template<typename T>
    seekDataWithKey_t<T>::seekDataWithKey_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, const T& key)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize), key(key) {}

    template<typename T>
    seekDataWithKey_t<T>::seekDataWithKey_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, T&& key)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize), key(std::move(key)) {}

    template<typename T>
    seekDataWithKey_t<T>::seekDataWithKey_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, const T& key, const ebmlElement_sp& ref)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize, ref), key(key) {}

    template<typename T>
    seekDataWithKey_t<T>::seekDataWithKey_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, const T& key, const ebmlElement_sp& ref, const ebmlElement_sp& parent)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize, ref, parent), key(key) {}

    template<typename T>
    seekDataWithKey_t<T>::seekDataWithKey_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, T&& key, const ebmlElement_sp& ref)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize, ref), key(std::move(key)) {}

    template<typename T>
    seekDataWithKey_t<T>::seekDataWithKey_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, T&& key, const ebmlElement_sp& ref, const ebmlElement_sp& parent)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize, ref, parent), key(std::move(key)) {}


    // template<typename T>
    // seekDataWithKey_t<T>::seekDataWithKey_t(const seekDataWithKey_t<T>&);
    //
    // template<typename T>
    // seekDataWithKey_t<T>::seekDataWithKey_t(seekDataWithKey_t<T>&&);
    //
    // template<typename T>
    // seekDataWithKey_t<T>& seekDataWithKey_t<T>::operator=(const seekDataWithKey_t<T>&);
    //
    // template<typename T>
    // seekDataWithKey_t<T>& seekDataWithKey_t<T>::operator=(seekDataWithKey_t<T>&&);

    template<typename T>
    void seekDataWithKey_t<T>::add(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>& map) {
        auto& ptr = map[this->ebmlID];

        if (ptr == nullptr) {
            auto raw = new seekMap<T>();
            auto& submap = *raw;
            ptr = std::unique_ptr<seekMapBase>(raw);
            submap[this->key] = this;
        } else {
            auto& submap = *ptr;
            submap[this->key] = this;
        }
    }

    template<typename T>
    void seekDataWithKey_t<T>::rm(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>& map) {
        if (map.count(this->ebmlID)) {
            auto& ptr = map[this->ebmlID];
            auto& submap = *ptr;
            submap.erase(this->key);
        }
    }



    template<typename T>
    seekMap<T>& seekMapBase::withKeyType() {
        if (auto recast = std::dynamic_pointer_cast<seekMap<T>*>(this)) {
            return *recast;
        }
        throw std::bad_alloc();
    }

    template<typename T>
    const seekMap<T>& seekMapBase::withKeyType() const {
        if (auto recast = std::dynamic_pointer_cast<const seekMap<T>*>(this)) {
            return *recast;
        }
        throw std::bad_alloc();
    }

    template<typename T>
    size_t seekMapBase::count(const T& t) const{
        return this->withKeyType<T>().count(t);
    }

    template<typename T>
    size_t seekMapBase::erase(const T& t) {
        return this->withKeyType<T>().erase(t);
    }

    template<typename T>
    seekDataWithKey_t<T>*& seekMapBase::operator[](const T& t) {
        return this->withKeyType<T>()[t];
    }

    template<typename T>
    seekDataWithKey_t<T>*& seekMapBase::at(const T& t) {
        return this->withKeyType<T>().at(t);
    }

    template<typename T>
    seekDataWithKey_t<T>* const& seekMapBase::at(const T& t) const {
        return this->withKeyType<T>().at(t);
    }



    template<typename T>
    seekDataWithKey_t<T>*& seekMap<T>::operator[](const T& t) {
        return this->_items[t];
    }

    template<typename T>
    seekDataWithKey_t<T>*& seekMap<T>::at(const T& t) {
        return this->_items.at(t);
    }

    template<typename T>
    seekDataWithKey_t<T>* const& seekMap<T>::at(const T& t) const {
        return this->_items.at(t);
    }

    template<typename T>
    size_t seekMap<T>::count(const T& t) const {
        return this->_items.count(t);
    }

    template<typename T>
    size_t seekMap<T>::erase(const T& t) {
        return this->_items.erase(t);
    }
}

#endif
