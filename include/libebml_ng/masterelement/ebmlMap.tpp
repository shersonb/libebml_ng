#ifndef EBML_NG_MASTERELEMENT_MAP_TPP
#define EBML_NG_MASTERELEMENT_MAP_TPP

#include "ebmlMap.h"
#include "../basictypes.h"
#include "../struct/unicode.h"
#include "../ebmlDataElement.tpp"

namespace ebml {
    // template<typename K, typename V, typename H, typename E, typename A, typename B>
    // ebmlMapClass<K, V, H, E, A, B>::ebmlMapClass(const char* ebmlID, const std::wstring& name, const ebmlElementClass* paircls)
    // : ebmlMapClass(unpackVint(ebmlID), name, paircls) {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMapClass<K, V, H, E, A, B>::ebmlMapClass(const char* ebmlID, const std::wstring& name, const ebmlPairClass<const K, V>* paircls)
    : ebmlMapClass(unpackVint(ebmlID), name, paircls) {}

    // template<typename K, typename V, typename H, typename E, typename A, typename B>
    // ebmlMapClass<K, V, H, E, A, B>::ebmlMapClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlElementClass* paircls)
    // : ClsMixin<ebmlMapClass<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElementClass>(ebmlID, name), _paircls(paircls->ptr<ebmlPairClass<const K, V>>()) {
    //     this->_childClasses.add({this->_paircls, 0, -1});
    // }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMapClass<K, V, H, E, A, B>::ebmlMapClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlPairClass<const K, V>* paircls)
    : ClsMixin<ebmlMapClass<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElementClass>(ebmlID, name), _paircls(paircls) {
        this->_childClasses.add({this->_paircls, 0, -1});
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    const ebmlPairClass<const K, V>& ebmlMapClass<K, V, H, E, A, B>::paircls() const {
        return *this->_paircls;
    }

    // template<typename K, typename V, typename H, typename E, typename A, typename B>
    // ebmlElement_sp ebmlMapClass<K, V, H, E, A, B>::operator()() const {
    //     return ebmlElement_sp(this->_new());
    // }

    // template<typename K, typename V, typename H, typename E, typename A, typename B>
    // ebmlElement* ebmlMapClass<K, V, H, E, A, B>::_new() const {
    //     return new ebmlMap<K, V, H, E, A, B>(this);
    // }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::ebmlMap(const ebmlMapClass<K, V, H, E, A, B>* cls) : InstMixin<ebmlMapClass<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElement>(cls) {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::ebmlMap(const ebmlMap<K, V, H, E, A, B>& orig) : ebmlMap(&orig.cls()) {
        for (auto& pair : orig._data) {
            auto elem = pair.second->clone();
            this->_attachChild(elem);
            this->_data.insert({pair.first, std::move(elem)});
        }
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::_clear() {
        auto iter = this->_data.cbegin();
        auto end = this->_data.cbegin();

        while (iter != end) {
            const auto& p = *iter;
            this->_detachChild(p.second);
            ++iter;
        }
        this->_data.clear();
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    std::wstring ebmlMap<K, V, H, E, A, B>::minirepr() const {
        std::wstring result = L"{";

        auto iter = this->_data.cbegin();
        auto end = this->_data.cend();

        if (iter != end) {
            const auto& p = *iter;
            const auto& elem = *p.second;

            result += ebml::repr(elem.key());
            result += L": ";

            if constexpr (IS_EBML_TYPE(V)) {
                result += elem.repr();
            } else {
                result += ebml::repr(elem.value());
            }

            ++iter;
        }

        while (iter != end) {
            const auto& p = *iter;
            const auto& elem = *p.second;

            result += L", ";
            result += ebml::repr(elem.key());
            result += L": ";

            if constexpr (IS_EBML_TYPE(V)) {
                result += elem.repr();
            } else {
                result += ebml::repr(elem.value());
            }

            ++iter;
        }

        result += L"}";
        return result;
    }

    // Element access
    template<typename K, typename V, typename H, typename E, typename A, typename B>
    pair_value_slot_t<V> ebmlMap<K, V, H, E, A, B>::operator[](const K& key) {
        ebml::ptr<ebmlPair<const K, V>>& ebmlpair = this->_data[key];

        if (ebmlpair == nullptr) {
            const auto& paircls = this->cls().paircls();
            const auto& valcls = paircls.valcls();

            if constexpr (IS_EBML_TYPE(V)) {
                auto elem = paircls(key, valcls());
                this->_attachChild(elem);
                ebmlpair = std::move(elem);
            } else {
                auto elem = paircls(key, valcls.defaultval);
                this->_attachChild(elem);
                ebmlpair = std::move(elem);
            }
        }

        return ebmlpair->value();
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    pair_value_slot_t<V> ebmlMap<K, V, H, E, A, B>::at(const K& key) {
        ebml::ptr<ebmlPair<const K, V>>& ebmlpair = this->_data.at(key);

        if (ebmlpair == nullptr) {
            const auto& paircls = this->cls().paircls();
            const auto& valcls = paircls.valcls();

            if constexpr (IS_EBML_TYPE(V)) {
                auto elem = paircls(key, valcls());
                this->_attachChild(elem);
                ebmlpair = std::move(elem);
            } else {
                auto elem = paircls(key, valcls.defaultval);
                this->_attachChild(elem);
                ebmlpair = std::move(elem);
            }
        }

        return ebmlpair->value();
    }

    // template<typename K, typename V, typename H, typename E, typename A, typename B>
    // const ebmlMapClass<K, V, H, E, A, B>* ebmlMap<K, V, H, E, A, B>::cls() const {
    //     return this->_cls->template ptr<ebmlMapClass<K, V, H, E, A, B>>();
    // }

    // Const element access

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    pair_const_value_t<V> ebmlMap<K, V, H, E, A, B>::at(const K& key) const {
        const auto& ebmlpair = *this->_data.at(key);
        // ebmlpair + "asdf";
        return ebmlpair.value();
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_iterator::_iterator(const ebml::ptr<ebmlMap<K, V, H, E, A, B>>& elem, const typename ebmlMapData<K, V, H, E, A>::iterator& iter)
    : _elem(elem), _iter(iter) {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_iterator::_iterator(ebml::ptr<ebmlMap<K, V, H, E, A, B>>&& elem, typename ebmlMapData<K, V, H, E, A>::iterator&& iter)
    : _elem(std::move(elem)), _iter(std::move(iter)) {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_iterator::_iterator() {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_iterator::~_iterator() {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_iterator* ebmlMap<K, V, H, E, A, B>::_iterator::copy() const {
        return new ebmlMap<K, V, H, E, A, B>::_iterator(this->_elem, this->_iter);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlElement& ebmlMap<K, V, H, E, A, B>::_iterator::operator*() const {
        // _current = (*_iter).second;
        // return _current;
        return *(*_iter).second;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_iterator& ebmlMap<K, V, H, E, A, B>::_iterator::operator++() {
        ++this->_iter;
        return *this;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_iterator& ebmlMap<K, V, H, E, A, B>::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlMap<K, V, H, E, A, B>::_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_iter = recast->_iter;
        } else {
            throw std::invalid_argument("invalid iterator type");
        }

        return *this;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    bool ebmlMap<K, V, H, E, A, B>::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlMap<K, V, H, E, A, B>::_iterator*>(&other)) {
            return ((this->_elem == recast->_elem)
                and (this->_iter == recast->_iter));
        }

        return false;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    bool ebmlMap<K, V, H, E, A, B>::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
        return not (*this == other);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_iterator* ebmlMap<K, V, H, E, A, B>::_begin() {
        // auto elem = this->shared_from_this();
        return new ebmlMap<K, V, H, E, A, B>::_iterator(this->sp(), _data.begin());
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_iterator* ebmlMap<K, V, H, E, A, B>::_end() {
        // auto elem = this->shared_from_this();
        return new ebmlMap<K, V, H, E, A, B>::_iterator(this->sp(), _data.end());
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_const_iterator::_const_iterator(
        const ebml::ptr<const ebmlMap<K, V, H, E, A, B>>& elem, const typename ebmlMapData<K, V, H, E, A>::const_iterator& iter)
    : _elem(elem), _iter(iter) {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_const_iterator::_const_iterator(
        ebml::ptr<const ebmlMap<K, V, H, E, A, B>>&& elem, typename ebmlMapData<K, V, H, E, A>::const_iterator&& iter)
    : _elem(std::move(elem)), _iter(std::move(iter)) {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_const_iterator::_const_iterator() {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::_const_iterator::~_const_iterator() {}

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_const_iterator* ebmlMap<K, V, H, E, A, B>::_const_iterator::copy() const {
        return new ebmlMap<K, V, H, E, A, B>::_const_iterator(this->_elem, this->_iter);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    const ebmlElement& ebmlMap<K, V, H, E, A, B>::_const_iterator::operator*() const {
        return *(*this->_iter).second;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_const_iterator& ebmlMap<K, V, H, E, A, B>::_const_iterator::operator++() {
        ++this->_iter;
        return *this;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_const_iterator& ebmlMap<K, V, H, E, A, B>::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlMap<K, V, H, E, A, B>::_const_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_iter = recast->_iter;
        } else {
            throw std::invalid_argument("invalid iterator type");
        }

        return *this;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    bool ebmlMap<K, V, H, E, A, B>::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlMap<K, V, H, E, A, B>::_const_iterator*>(&other)) {
            return ((this->_elem == recast->_elem)
                and (this->_iter == recast->_iter));
        }

        return false;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    bool ebmlMap<K, V, H, E, A, B>::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
        return not (*this == other);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_const_iterator* ebmlMap<K, V, H, E, A, B>::_cbegin() const {
        // auto elem = this->shared_from_this();
        // auto c_elem = std::const_pointer_cast<const ebmlElement>(elem);
        // return new ebmlMap<K, V, H, E, A, B>::_const_iterator(std::move(c_elem), std::move(this->_data.cbegin()));
        return new ebmlMap<K, V, H, E, A, B>::_const_iterator(this->sp(), _data.cbegin());
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMasterElement::_const_iterator* ebmlMap<K, V, H, E, A, B>::_cend() const {
        // auto elem = this->shared_from_this();
        // auto c_elem = std::const_pointer_cast<const ebmlElement>(elem);
        // return new ebmlMap<K, V, H, E, A, B>::_const_iterator(std::move(c_elem), std::move(this->_data.cend()));
        return new ebmlMap<K, V, H, E, A, B>::_const_iterator(this->sp(), _data.cend());
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::_addChild(const ebmlElement_sp& elem) {
        const K& key = elem->as<ebmlPair<const K, V>>().key();

        if (this->_data.count(key)) {
            this->_detachChild(this->_data[key]);
            this->_data[key] = elem;
        } else if (auto recast = elem->sp<ebmlPair<const K, V>>()) {
            this->_data.insert({key, std::move(recast)});
        }
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::_addChild(ebmlElement_sp&& elem) {
        const K& key = elem->as<ebmlPair<const K, V>>().key();

        if (this->_data.count(key)) {
            this->_detachChild(this->_data[key]);
            this->_data[key] = std::move(elem);
        } else if (auto recast = ebml_dynamic_pointer_cast<ebmlPair<const K, V>>(std::move(elem))) {
            this->_data.insert({key, std::move(recast)});
        }
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    size_t ebmlMap<K, V, H, E, A, B>::erase(const K& key) {
        if (this->_data.count(key)) {
            this->_detachChild(this->_data[key]);
        }

        return this->_data.erase(key);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    size_t ebmlMap<K, V, H, E, A, B>::count(const K& key) const {
        return this->_data.count(key);
    }

//     // V = void specialization
//
//     template<typename T>
//     ebmlPairClass<T>::ebmlPairClass(const char* ebmlID, const std::wstring& name, const ebmlElementClass* keycls, const ebmlElementClass* valcls)
//     : ebmlPairClass(unpackVint(ebmlID), name, keycls, valcls) {}
//
//     template<typename T>
//     ebmlPairClass<T>::ebmlPairClass(const char* ebmlID, const std::wstring& name, const ebmlDataElementClass<T>* keycls, const ebmlElementClass* valcls)
//     : ebmlPairClass(unpackVint(ebmlID), name, keycls, valcls) {}
//
//     template<typename T>
//     ebmlPairClass<T>::ebmlPairClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlElementClass* keycls, const ebmlElementClass* valcls)
//     : ebmlMasterElementClass(ebmlID, name), _keycls(keycls->ptr<ebmlDataElementClass<T>>()), _valcls(valcls) {
//         if (this->_keycls == nullptr) {
//             throw std::invalid_argument("cannot convert to const ebmlDataElementClass<T>*");
//         }
//         this->_childClasses = {{keycls, 1, 1}, {valcls, 1, 1}};
//         this->_valueSpec = {{valcls, 1, 1}};
//     }
//
//     template<typename T>
//     ebmlPairClass<T>::ebmlPairClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlDataElementClass<T>* keycls, const ebmlElementClass* valcls)
//     : ebmlMasterElementClass(ebmlID, name), _keycls(keycls), _valcls(valcls) {
//         this->_childClasses = {{keycls, 1, 1}, {valcls, 1, 1}};
//         this->_valueSpec = {{valcls, 1, 1}};
//     }
//
//     template<typename T>
//     ebmlElement_sp ebmlPairClass<T>::operator()(const T& key, const ebmlElement_sp& value) const {
//         auto elem = new ebmlPair<T>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         elem->_validate(value);
//         elem->_setData(key, value);
//         return elem_sp;
//     }
//
//     template<typename T>
//     ebmlElement_sp ebmlPairClass<T>::operator()(const T& key, ebmlElement_sp&& value) const {
//         auto elem = new ebmlPair<T>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         elem->_validate(value);
//         elem->_setData(key, std::move(value));
//         return elem_sp;
//     }
//
//     template<typename T>
//     ebmlElement_sp ebmlPairClass<T>::operator()(T&& key, ebmlElement_sp&& value) const {
//         auto elem = new ebmlPair<T>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         elem->_validate(value);
//         elem->_setData(std::move(key), std::move(value));
//         return elem_sp;
//     }
//
//     template<typename T>
//     ebmlElement* ebmlPairClass<T>::_new() const {
//         return new ebmlPair<T>(this);
//     }
//
//     template<typename T>
//     const ebmlDataElementClass<T>* ebmlPairClass<T>::keycls() const {
//         return this->_keycls->template ptr<ebmlDataElementClass<T>>();
//     }
//
//     template<typename T>
//     const ebmlElementClass* ebmlPairClass<T>::valcls() const {
//         return this->_valcls;
//     }
//
//     template<typename T>
//     ebmlPair<T>::ebmlPair(const ebmlPairClass<T>* cls) : ebmlMasterElement(cls) {}
//
//     template<typename T>
//     void ebmlPair<T>::_validate(const ebmlElement_sp& value) {
//         auto c = this->cls();
//
//         if (value->cls() != c->_valcls) {
//             std::string errmsg = "ebmlPair(): cannot assign '";
//             errmsg += pack(value->cls()->name);
//             errmsg += "' element to value slot (expected '";
//             errmsg += pack(c->_valcls->name);
//             errmsg += "' element)";
//             throw std::invalid_argument(errmsg);
//         }
//
//         this->_attachChild(value);
//     }
//
//     template<typename T>
//     void ebmlPair<T>::_setData(const T& key, const ebmlElement_sp& value) {
//         auto c = this->cls();
//         auto keyelem = (*c->_keycls)(key);
//         this->_key = std::move(keyelem);
//         this->_value = value;
//     }
//
//     template<typename T>
//     void ebmlPair<T>::_setData(const T& key, ebmlElement_sp&& value) {
//         auto c = this->cls();
//         auto keyelem = (*c->_keycls)(key);
//         this->_key = std::move(keyelem);
//         this->_value = std::move(value);
//     }
//
//     template<typename T>
//     void ebmlPair<T>::_setData(T&& key, ebmlElement_sp&& value) {
//         auto c = this->cls();
//         auto keyelem = (*c->_keycls)(std::move(key));
//         this->_key = std::move(keyelem);
//         this->_value = std::move(value);
//     }
//
//     template<typename T>
//     void ebmlPair<T>::_clear() {
//         if (this->_key != nullptr) {
//             this->_detachChild(this->_key);
//             this->_key = nullptr;
//         }
//
//         if (this->_value != nullptr) {
//             this->_detachChild(this->_value);
//             this->_value = nullptr;
//         }
//     }
//
//     template<typename T>
//     std::wstring ebmlPair<T>::minirepr() const {
//         std::wstring result = L"(";
//         result += this->_key->minirepr();
//         result += L", ";
//         result += this->_value->repr();
//         result += L")";
//         return result;
//     }
//
//     template<typename T>
//     const ebmlPairClass<T>* ebmlPair<T>::cls() const {
//         return this->_cls->ptr<ebmlPairClass<T>>();
//     }
//
//     // Element access
//     template<typename T>
//     T& ebmlPair<T, void>::key() {
//         return data<T>(this->_key);
//     }
//
//     template<typename T>
//     const T& ebmlPair<T>::key() const {
//         return data<T>(this->_key);
//     }
//
//     // template<typename T>
//     // const ebmlElement_sp& ebmlPair<T>::keyElem() {
//     //     return this->_key;
//     // }
//
//     template<typename K>
//     template<typename T>
//     childSlot_t<T> ebmlPair<K>::value() {
//         return {this, this->cls()->_valueSpec, this->_value, false};
//     }
//
// #if RAW
//     template<typename K>
//     const ebmlElement* ebmlPair<K>::value() const {
//         return this->_value.get();
//     }
//
//     // template<typename T>
//     // const ebmlElement* ebmlPair<T>::keyElem() const {
//     //     return this->_key.get();
//     // }
// #else
//     template<typename T>
//     c_ebmlElement_sp ebmlPair<T>::value() const {
//         return std::const_pointer_cast<const ebmlElement>(this->_value);
//     }
//
//     // template<typename T>
//     // c_ebmlElement_sp ebmlPair<T>::keyElem() const {
//     //     return std::const_pointer_cast<const ebmlElement>(this->_key);
//     // }
// #endif
//
//     template<typename T>
//     ebmlPair<T>::_iterator::_iterator(const ebmlPair_sp<T>& elem, unsigned char pos) {
//         this->_elem = elem;
//         this->_pos = pos;
//     }
//
//     template<typename T>
//     ebmlPair<T>::_iterator::_iterator(ebmlPair_sp<T>&& elem, unsigned char pos) {
//         this->_elem = std::move(elem);
//         this->_pos = pos;
//     }
//
//     template<typename T>
//     ebmlPair<T>::_iterator::_iterator() {}
//
//     template<typename T>
//     ebmlPair<T>::_iterator::~_iterator() {}
//
//     template<typename T>
//     ebmlMasterElement::_iterator* ebmlPair<T>::_iterator::copy() const {
//         return new ebmlPair<T>::_iterator(this->_elem, this->_pos);
//     }
//
//     template<typename T>
//     const ebmlElement_sp& ebmlPair<T>::_iterator::operator*() const {
//         if (this->_pos == 0) {
//             return this->_elem->_key;
//         }
//         return this->_elem->_value;
//
//     }
//
//     template<typename T>
//     ebmlMasterElement::_iterator& ebmlPair<T>::_iterator::operator++() {
//         ++this->_pos;
//         return *this;
//     }
//
//     template<typename T>
//     ebmlMasterElement::_iterator& ebmlPair<T>::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
//         if (auto recast = dynamic_cast<const ebmlPair<T>::_iterator*>(&other)) {
//             this->_elem = recast->_elem;
//             this->_pos = recast->_pos;
//         } else {
//             throw std::bad_alloc();
//         }
//
//         return *this;
//     }
//
//     template<typename T>
//     bool ebmlPair<T>::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
//         if (auto recast = dynamic_cast<const ebmlPair<T>::_iterator*>(&other)) {
//             if (this->_elem != recast->_elem) {
//                 return false;
//             }
//
//             if ((this->_pos > 1) and (recast->_pos > 1)) {
//                 return true;
//             }
//
//             return this->_pos == recast->_pos;
//         }
//
//         return false;
//     }
//
//     template<typename T>
//     bool ebmlPair<T>::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
//         return not (*this == other);
//     }
//
//     template<typename T>
//     ebmlMasterElement::_iterator* ebmlPair<T>::_begin() {
//         auto elem = this->shared_from_this();
//         auto recast = std::dynamic_pointer_cast<ebmlPair<T>>(elem);
//         return new ebmlPair<T>::_iterator(std::move(recast), 0);
//     }
//
//     template<typename T>
//     ebmlMasterElement::_iterator* ebmlPair<T>::_end() {
//         auto elem = this->shared_from_this();
//         auto recast = std::dynamic_pointer_cast<ebmlPair<T>>(elem);
//         return new ebmlPair<T>::_iterator(std::move(recast), 2);
//     }
//
//     template<typename T>
//     ebmlPair<T>::_const_iterator::_const_iterator(const c_ebmlPair_sp<T>& elem, unsigned char pos) {
//         this->_elem = elem;
//         this->_pos = pos;
//     }
//
//     template<typename T>
//     ebmlPair<T>::_const_iterator::_const_iterator(c_ebmlPair_sp<T>&& elem, unsigned char pos) {
//         this->_elem = std::move(elem);
//         this->_pos = pos;
//     }
//
//     template<typename T>
//     ebmlPair<T>::_const_iterator::_const_iterator() {}
//
//     template<typename T>
//     ebmlPair<T>::_const_iterator::~_const_iterator() {}
//
//     template<typename T>
//     ebmlMasterElement::_const_iterator* ebmlPair<T>::_const_iterator::copy() const {
//         return new ebmlPair<T>::_const_iterator(this->_elem, this->_pos);
//     }
//
// #if RAW
//     template<typename T>
//     const ebmlElement* ebmlPair<T>::_const_iterator::operator*() const {
//         if (this->_pos == 0) {
//             return this->_elem->_key.get();
//         }
//         return this->_elem->_value.get();
//     }
// #else
//     template<typename T>
//     c_ebmlElement_sp ebmlPair<T>::_const_iterator::operator*() const {
//         if (this->_pos == 0) {
//             return std::const_pointer_cast<const ebmlElement>(this->_elem->_key);
//         }
//         return std::const_pointer_cast<const ebmlElement>(this->_elem->_value);
//     }
// #endif
//     template<typename T>
//     ebmlMasterElement::_const_iterator& ebmlPair<T>::_const_iterator::operator++() {
//         ++this->_pos;
//         return *this;
//     }
//
//     template<typename T>
//     ebmlMasterElement::_const_iterator& ebmlPair<T>::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
//         if (auto recast = dynamic_cast<const ebmlPair<T>::_const_iterator*>(&other)) {
//             this->_elem = recast->_elem;
//             this->_pos = recast->_pos;
//         } else {
//             throw std::bad_alloc();
//         }
//
//         return *this;
//     }
//
//     template<typename T>
//     bool ebmlPair<T>::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
//         if (auto recast = dynamic_cast<const ebmlPair<T>::_const_iterator*>(&other)) {
//             if (this->_elem != recast->_elem) {
//                 return false;
//             }
//
//             if ((this->_pos > 1) and (recast->_pos > 1)) {
//                 return true;
//             }
//
//             return this->_pos == recast->_pos;
//         }
//
//         return false;
//     }
//
//     template<typename T>
//     bool ebmlPair<T>::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
//         return not (*this == other);
//     }
//
//     template<typename T>
//     ebmlMasterElement::_const_iterator* ebmlPair<T>::_cbegin() const {
//         auto elem = this->shared_from_this();
//         auto recast = std::dynamic_pointer_cast<const ebmlPair<T>>(elem);
//         return new ebmlPair<T>::_const_iterator(std::move(recast), 0);
//     }
//
//     template<typename T>
//     ebmlMasterElement::_const_iterator* ebmlPair<T>::_cend() const {
//         auto elem = this->shared_from_this();
//         auto recast = std::dynamic_pointer_cast<const ebmlPair<T>>(elem);
//         return new ebmlPair<T>::_const_iterator(std::move(recast), 2);
//     }
//
//     template<typename T>
//     void ebmlPair<T>::_addChild(const ebmlElement_sp& elem) {
//         auto childcls = elem->cls();
//         auto _cls = this->cls();
//
//         if (childcls == _cls->_keycls) {
//             this->_key = elem;
//         } else if (childcls == _cls->_valcls) {
//             this->_value = elem;
//         }
//     }
//
//     template<typename T>
//     void ebmlPair<T>::_addChild(ebmlElement_sp&& elem) {
//         auto childcls = elem->cls();
//         auto _cls = this->cls();
//
//         if (childcls == _cls->_keycls) {
//             this->_key = std::move(elem);
//             elem = nullptr;
//         } else if (childcls == _cls->_valcls) {
//             this->_value = std::move(elem);
//         }
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMapClass<K, void, H, E, A>::ebmlMapClass(const char* ebmlID, const std::wstring& name, const ebmlElementClass* paircls)
//     : ebmlMapClass(unpackVint(ebmlID), name, paircls) {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMapClass<K, void, H, E, A>::ebmlMapClass(const char* ebmlID, const std::wstring& name, const ebmlPairClass<const K>* paircls)
//     : ebmlMapClass(unpackVint(ebmlID), name, paircls) {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMapClass<K, void, H, E, A>::ebmlMapClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlElementClass* paircls)
//     : ebmlMasterElementClass(ebmlID, name), _paircls(paircls->ptr<ebmlPairClass<const K>>()) {
//         this->_childClasses.add({this->_paircls, 0, -1});
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMapClass<K, void, H, E, A>::ebmlMapClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlPairClass<const K>* paircls)
//     : ebmlMasterElementClass(ebmlID, name), _paircls(paircls) {
//         this->_childClasses.add({this->_paircls, 0, -1});
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     const ebmlPairClass<const K>* ebmlMapClass<K, void, H, E, A>::paircls() const {
//         return this->_paircls;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlElement_sp ebmlMapClass<K, void, H, E, A>::operator()() const {
//         return ebmlElement_sp(this->_new());
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlElement_sp ebmlMapClass<K, void, H, E, A>::operator()(std::initializer_list<std::pair<K, ebmlElement_sp>> items) const {
//         auto elem = new ebmlMap<K, void, H, E, A>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         std::list<std::pair<K, ebmlElement_sp>> items_l = items;
//         std::list<ebmlElement_sp> prepared = elem->_prepareData(std::move(items_l));
//         elem->_setData(std::move(prepared));
//         // elem->_data = std::move(prepared);
//         return elem_sp;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlElement_sp ebmlMapClass<K, void, H, E, A>::operator()(const std::list<std::pair<K, ebmlElement_sp>>& items) const {
//         auto elem = new ebmlMap<K, void, H, E, A>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         std::list<ebmlElement_sp> prepared = elem->_prepareData(items);
//         elem->_setData(std::move(prepared));
//         // elem->_data = std::move(prepared);
//         return elem_sp;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlElement_sp ebmlMapClass<K, void, H, E, A>::operator()(std::list<std::pair<K, ebmlElement_sp>>&& items) const {
//         auto elem = new ebmlMap<K, void, H, E, A>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         std::list<ebmlElement_sp> prepared = elem->_prepareData(std::move(items));
//         elem->_setData(std::move(prepared));
//         // elem->_data = std::move(prepared);
//         return elem_sp;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlElement_sp ebmlMapClass<K, void, H, E, A>::operator()(const std::unordered_map<K, ebmlElement_sp, H, E, A>& items) const {
//         auto elem = new ebmlMap<K, void, H, E, A>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         std::unordered_map<K, ebmlElement_sp, H, E, A> prepared = elem->_prepareData(items);
//         // elem->_setData(std::move(prepared));
//         elem->_data = std::move(prepared);
//         return elem_sp;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlElement_sp ebmlMapClass<K, void, H, E, A>::operator()(std::unordered_map<K, ebmlElement_sp, H, E, A>&& items) const {
//         auto elem = new ebmlMap<K, void, H, E, A>(this);
//         auto elem_sp = ebmlElement_sp(elem);
//         std::unordered_map<K, ebmlElement_sp, H, E, A> prepared = elem->_prepareData(std::move(items));
//         // elem->_setData(std::move(prepared));
//         elem->_data = std::move(prepared);
//         return elem_sp;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlElement* ebmlMapClass<K, void, H, E, A>::_new() const {
//         return new ebmlMap<K, void, H, E, A>(this);
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::ebmlMap(const ebmlMapClass<K, void, H, E, A>* cls) : ebmlMasterElement(cls) {}

    // Initialize from list
    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::_setData(std::vector<ebml::ptr<ebml::ebmlPair<const K, V>>>&& items) {
        for (auto& item : items) {
            const K& key = item->key();
            _data.insert({key, std::move(item)});
        }

        items.clear();
    }

//     template<typename K, typename H, typename E, typename A>
//     void ebmlMap<K, void, H, E, A>::_setData(std::list<ebmlElement_sp>&& items) {
//         for (auto& item : items) {
//             const K& key = item->as<ebmlPair<const K>>().key();
//             this->_data[key] = std::move(item);
//         }
//     }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    std::vector<ebml::ptr<ebmlPair<const K, V>>> ebmlMap<K, V, H, E, A, B>::_prepareData(const std::vector<std::pair<K, pair_value_t<V>>>& items) {
        std::vector<ebml::ptr<ebmlPair<const K, V>>> pairlist;
        pairlist.reserve(items.size());

        auto iter = items.cbegin();
        auto end = items.cend();
        auto& paircls = this->cls().paircls();

        while (iter != end) {
            auto& item = *iter;
            auto pair = paircls(item.first, item.second);
            this->_attachChild(pair);
            pairlist.push_back(std::move(pair));
            ++iter;
        }

        return pairlist;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    std::vector<ebml::ptr<ebmlPair<const K, V>>> ebmlMap<K, V, H, E, A, B>::_prepareData(std::vector<std::pair<K, pair_value_t<V>>>&& items) {
        std::vector<ebml::ptr<ebmlPair<const K, V>>> pairlist;
        auto start = items.begin();
        auto iter = start;
        auto end = items.end();
        auto& paircls = this->cls().paircls();

        while (iter != end) {
            try {
                auto& item = *iter;
                auto pair = paircls(std::move(item.first), std::move(item.second));
                this->_attachChild(pair);
                pairlist.push_back(std::move(pair));
            } catch (std::exception& e) {
                auto pairiter = pairlist.end();

                while (iter != start) {
                    --iter;
                    --pairiter;
                    auto& pair = *pairiter;

                    if constexpr (IS_EBML_TYPE(V)) {
                        *iter = {pair->_key->data, std::move(pair->_value)};
                    } else {
                        *iter = {pair->_key->data, std::move(pair->_value->data)};
                    }
                }

                throw;
            }
            ++iter;
        }

        items.clear();
        return pairlist;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMapData<K, V, H, E, A> ebmlMap<K, V, H, E, A, B>::_prepareData(const ebmlMapArgs<K, V, H, E, B>& items) {
        ebmlMapData<K, V, H, E, A> map;
        auto iter = items.cbegin();
        auto end = items.cend();
        auto& paircls = this->cls().paircls();
        auto& valcls = paircls.valcls();

        while (iter != end) {
            auto& item = *iter;

            if constexpr (IS_EBML_TYPE(V)) {
                if (&item.second->cls() != &valcls) {
                    std::string errmsg = "ebmlMap(): expected '";
                    errmsg += pack(valcls.name);
                    errmsg += "' object, got '";
                    errmsg += pack(item.second->cls().name);
                    errmsg += "' instead";
                    throw std::invalid_argument(errmsg);
                }
            }

            auto pair = paircls(item.first, item.second);
            this->_attachChild(pair);
            map.insert({item.first, std::move(pair)});
            ++iter;
        }

        return map;
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMapData<K, V, H, E, A> ebmlMap<K, V, H, E, A, B>::_prepareData(ebmlMapArgs<K, V, H, E, B>&& items) {
        ebmlMapData<K, V, H, E, A> map;
        auto iter = items.cbegin();
        auto end = items.cend();
        auto& paircls = this->cls().paircls();
        auto& valcls = paircls.valcls();

        while (iter != end) {
            try {
                auto& item = *iter;

                if constexpr (IS_EBML_TYPE(V)) {
                    if (&item.second->cls() != &valcls) {
                        std::string errmsg = "ebmlMap(): expected '";
                        errmsg += pack(valcls.name);
                        errmsg += "' object, got '";
                        errmsg += pack(item.second->cls().name);
                        errmsg += "' instead";
                        throw std::invalid_argument(errmsg);
                    }
                }

                auto pair = paircls(item.first, std::move(item.second));
                this->_attachChild(pair);
                map.insert({item.first, std::move(pair)});
                ++iter;
            } catch (std::exception& e) {
                for (auto& p : map) {
                    if constexpr (IS_EBML_TYPE(V)) {
                        items.at(p.first) = std::move(p.second->_value);
                    } else {
                        items.at(p.first) = std::move(p.second->_value->data);
                    }
                }
                throw;
            }
        }

        items.clear();
        return map;
    }

//     template<typename K, typename H, typename E, typename A>
//     void ebmlMap<K, void, H, E, A>::setData(std::initializer_list<std::pair<K, ebmlElement_sp>> items) {
//         std::list<std::pair<K, ebmlElement_sp>> items_l = items;
//         std::list<ebmlElement_sp> prepared = this->_prepareData(std::move(items_l));
//         this->_clear();
//         this->_setData(std::move(prepared));
//     }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::setData(const std::vector<std::pair<K, pair_value_t<V>>>& items) {
        auto prepared = this->_prepareData(items);
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::setData(std::vector<std::pair<K, pair_value_t<V>>>&& items) {
        auto prepared = this->_prepareData(std::move(items));
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::setData(const ebmlMapArgs<K, V, H, E, B>& items) {
        auto prepared = _prepareData(items);
        _clear();
        _data = std::move(prepared);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    void ebmlMap<K, V, H, E, A, B>::setData(ebmlMapArgs<K, V, H, E, B>&& items) {
        auto prepared = _prepareData(items);
        _clear();
        _data = std::move(prepared);
    }

//     template<typename K, typename H, typename E, typename A>
//     void ebmlMap<K, void, H, E, A>::_clear() {
//         auto iter = this->_data.cbegin();
//         auto end = this->_data.cbegin();
//
//         while (iter != end) {
//             const auto& p = *iter;
//             this->_detachChild(p.second);
//             ++iter;
//         }
//         this->_data.clear();
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     std::wstring ebmlMap<K, void, H, E, A>::minirepr() const {
//         std::wstring result = L"{";
//
//         auto iter = this->_data.cbegin();
//         auto end = this->_data.cend();
//
//         if (iter != end) {
//             const auto& p = *iter;
//             const auto& elem = p.second->template as<ebmlPair<const K>>();
//
//             result += ebml::repr(elem.key());
//             result += L": ";
//             result += elem.value()->repr();
//
//             ++iter;
//         }
//
//         while (iter != end) {
//             const auto& p = *iter;
//             const auto& elem = p.second->template as<ebmlPair<const K>>();
//
//             result += L", ";
//             result += ebml::repr(elem.key());
//             result += L": ";
//             result += elem.value()->repr();
//
//             ++iter;
//         }
//
//         result += L"}";
//         return result;
//     }
//
//     // Element access
//     template<typename K, typename H, typename E, typename A>
//     childSlot_t<> ebmlMap<K, void, H, E, A>::operator[](const K& key) {
//         ebmlElement_sp& ebmlpair = this->_data[key];
//
//         if (ebmlpair == nullptr) {
//             const auto& paircls = *this->cls()->paircls();
//             const auto& valcls = *paircls.valcls();
//             auto elem = paircls(key, valcls());
//             this->_attachChild(elem);
//             ebmlpair = std::move(elem);
//         }
//
//         return ebmlpair->as<ebmlPair<const K>>().value();
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     template<typename T>
//     childSlot_t<T> ebmlMap<K, void, H, E, A>::at(const K& key) {
//         ebmlElement_sp& ebmlpair = this->_data.at(key);
//
//         if (ebmlpair == nullptr) {
//             const auto& paircls = *this->cls()->paircls();
//             const auto& valcls = *paircls.valcls();
//             auto elem = paircls(key, valcls());
//             this->_attachChild(elem);
//             ebmlpair = std::move(elem);
//         }
//
//         return ebmlpair->as<ebmlPair<const K>>().template value<T>();
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     const ebmlMapClass<K, void, H, E, A>* ebmlMap<K, void, H, E, A>::cls() const {
//         return this->_cls->ptr<ebmlMapClass<K, void, H, E, A>>();
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, void, H, E, A>::insert(const std::pair<K, ebmlElement_sp>& pair) {
//         auto ebmlpair = (*this->cls()->paircls())(pair.first, pair.second);
//         std::pair<K, ebmlElement_sp> p = {pair.first, std::move(ebmlpair)};
//
//         auto q = this->_data.insert(std::move(p));
//
//         if (q.second) {
//             this->_attachChild(this->_data[pair.first]);
//         }
//
//         auto iter = new ebmlMap<K, void, H, E, A>::_iterator(this->shared_from_this(), q.first);
//         return {this->make_iter(iter), q.second};
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, void, H, E, A>::insert(std::pair<K, ebmlElement_sp>&& pair) {
//         auto ebmlpair = (*this->cls()->paircls())(pair.first, pair.second);
//         std::pair<K, ebmlElement_sp> p = {pair.first, std::move(ebmlpair)};
//
//         auto q = this->_data.insert(std::move(p));
//
//         if (q.second) {
//             this->_attachChild(this->_data[pair.first]);
//         } else {
//             // Move element back
//             pair.second = std::move(p.second->template as<ebmlPair<K>>().value());
//         }
//
//         auto iter = new ebmlMap<K, void, H, E, A>::_iterator(this->shared_from_this(), q.first);
//         return {this->make_iter(iter), q.second};
//     }
//
//     // Const element access
// #if RAW
//     template<typename K, typename H, typename E, typename A>
//     const ebmlElement* ebmlMap<K, void, H, E, A>::at(const K& key) const {
//         const ebmlElement_sp& ebmlpair = this->_data.at(key);
//         return ebmlpair->as<const ebmlPair<const K>>().value();
//     }
// #else
//     template<typename K, typename H, typename E, typename A>
//     c_ebmlElement_sp ebmlMap<K, void, H, E, A>::at(const K& key) const {
//         const ebmlElement_sp& ebmlpair = this->_data.at(key);
//         return ebmlpair->as<const ebmlPair<const K>>().value();
//     }
// #endif
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_iterator::_iterator(const ebmlElement_sp& elem, const typename std::unordered_map<K, ebmlElement_sp, H, E, A>::iterator& iter)
//     : _elem(elem), _iter(iter) {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_iterator::_iterator(ebmlElement_sp&& elem, typename std::unordered_map<K, ebmlElement_sp, H, E, A>::iterator&& iter)
//     : _elem(std::move(elem)), _iter(std::move(iter)) {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_iterator::_iterator() {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_iterator::~_iterator() {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_iterator* ebmlMap<K, void, H, E, A>::_iterator::copy() const {
//         return new ebmlMap<K, void, H, E, A>::_iterator(this->_elem, this->_iter);
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     const ebmlElement_sp& ebmlMap<K, void, H, E, A>::_iterator::operator*() const {
//         return (*this->_iter).second;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_iterator& ebmlMap<K, void, H, E, A>::_iterator::operator++() {
//         ++this->_iter;
//         return *this;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_iterator& ebmlMap<K, void, H, E, A>::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
//         if (auto recast = dynamic_cast<const ebmlMap<K, void, H, E, A>::_iterator*>(&other)) {
//             this->_elem = recast->_elem;
//             this->_iter = recast->_iter;
//         } else {
//             throw std::invalid_argument("invalid iterator type");
//         }
//
//         return *this;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     bool ebmlMap<K, void, H, E, A>::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
//         if (auto recast = dynamic_cast<const ebmlMap<K, void, H, E, A>::_iterator*>(&other)) {
//             return ((this->_elem == recast->_elem)
//                 and (this->_iter == recast->_iter));
//         }
//
//         return false;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     bool ebmlMap<K, void, H, E, A>::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
//         return not (*this == other);
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_iterator* ebmlMap<K, void, H, E, A>::_begin() {
//         auto elem = this->shared_from_this();
//         return new ebmlMap<K, void, H, E, A>::_iterator(std::move(elem), std::move(this->_data.begin()));
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_iterator* ebmlMap<K, void, H, E, A>::_end() {
//         auto elem = this->shared_from_this();
//         return new ebmlMap<K, void, H, E, A>::_iterator(std::move(elem), std::move(this->_data.end()));
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_const_iterator::_const_iterator(const c_ebmlElement_sp& elem, const typename std::unordered_map<K, ebmlElement_sp, H, E, A>::const_iterator& iter)
//     : _elem(elem), _iter(iter) {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_const_iterator::_const_iterator(c_ebmlElement_sp&& elem, typename std::unordered_map<K, ebmlElement_sp, H, E, A>::const_iterator&& iter)
//     : _elem(std::move(elem)), _iter(std::move(iter)) {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_const_iterator::_const_iterator() {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMap<K, void, H, E, A>::_const_iterator::~_const_iterator() {}
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_const_iterator* ebmlMap<K, void, H, E, A>::_const_iterator::copy() const {
//         return new ebmlMap<K, void, H, E, A>::_const_iterator(this->_elem, this->_iter);
//     }
//
// #if RAW
//     template<typename K, typename H, typename E, typename A>
//     const ebmlElement* ebmlMap<K, void, H, E, A>::_const_iterator::operator*() const {
//         return (*this->_iter).second.get();
//     }
// #else
//     template<typename K, typename H, typename E, typename A>
//     c_ebmlElement_sp ebmlMap<K, void, H, E, A>::_const_iterator::operator*() const {
//         return std::const_pointer_cast<const ebmlElement>((*this->_iter).second);
//     }
// #endif
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_const_iterator& ebmlMap<K, void, H, E, A>::_const_iterator::operator++() {
//         ++this->_iter;
//         return *this;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_const_iterator& ebmlMap<K, void, H, E, A>::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
//         if (auto recast = dynamic_cast<const ebmlMap<K, void, H, E, A>::_const_iterator*>(&other)) {
//             this->_elem = recast->_elem;
//             this->_iter = recast->_iter;
//         } else {
//             throw std::invalid_argument("invalid iterator type");
//         }
//
//         return *this;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     bool ebmlMap<K, void, H, E, A>::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
//         if (auto recast = dynamic_cast<const ebmlMap<K, void, H, E, A>::_const_iterator*>(&other)) {
//             return ((this->_elem == recast->_elem)
//                 and (this->_iter == recast->_iter));
//         }
//
//         return false;
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     bool ebmlMap<K, void, H, E, A>::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
//         return not (*this == other);
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_const_iterator* ebmlMap<K, void, H, E, A>::_cbegin() const {
//         auto elem = this->shared_from_this();
//         auto c_elem = std::const_pointer_cast<const ebmlElement>(elem);
//         return new ebmlMap<K, void, H, E, A>::_const_iterator(std::move(c_elem), std::move(this->_data.cbegin()));
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     ebmlMasterElement::_const_iterator* ebmlMap<K, void, H, E, A>::_cend() const {
//         auto elem = this->shared_from_this();
//         auto c_elem = std::const_pointer_cast<const ebmlElement>(elem);
//         return new ebmlMap<K, void, H, E, A>::_const_iterator(std::move(c_elem), std::move(this->_data.cend()));
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     void ebmlMap<K, void, H, E, A>::_addChild(const ebmlElement_sp& elem) {
//         const K& key = elem->as<ebmlPair<const K>>().key();
//
//         if (this->_data.count(key)) {
//             this->_detachChild(this->_data[key]);
//             this->_data[key] = elem;
//         } else {
//             this->_data.insert({key, elem});
//         }
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     void ebmlMap<K, void, H, E, A>::_addChild(ebmlElement_sp&& elem) {
//         const K& key = elem->as<ebmlPair<const K>>().key();
//
//         if (this->_data.count(key)) {
//             this->_detachChild(this->_data[key]);
//             this->_data[key] = std::move(elem);
//         } else {
//             this->_data.insert({key, std::move(elem)});
//         }
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     size_t ebmlMap<K, void, H, E, A>::erase(const K& key) {
//         if (this->_data.count(key)) {
//             this->_detachChild(this->_data[key]);
//         }
//
//         return this->_data.erase(key);
//     }
//
//     template<typename K, typename H, typename E, typename A>
//     size_t ebmlMap<K, void, H, E, A>::count(const K& key) const {
//         return this->_data.count(key);
//     }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::ebmlMap(const ebmlMapClass<K, V, H, E, A, B>* cls, const std::vector<std::pair<K, pair_value_t<V>>>& items)
    : InstMixin<ebmlMapClass<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElement>(cls) {
        this->setData(items);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::ebmlMap(const ebmlMapClass<K, V, H, E, A, B>* cls, std::vector<std::pair<K, pair_value_t<V>>>&& items)
    : InstMixin<ebmlMapClass<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElement>(cls) {
        this->setData(std::move(items));
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::ebmlMap(const ebmlMapClass<K, V, H, E, A, B>* cls, const ebmlMapArgs<K, V, H, E, B>& items)
    : InstMixin<ebmlMapClass<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElement>(cls) {
        this->setData(items);
    }

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    ebmlMap<K, V, H, E, A, B>::ebmlMap(const ebmlMapClass<K, V, H, E, A, B>* cls, ebmlMapArgs<K, V, H, E, B>&& items)
    : InstMixin<ebmlMapClass<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElement>(cls) {
        this->setData(std::move(items));
    }
}
#endif
