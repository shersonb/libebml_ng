#ifndef EBML_NG_MASTERELEMENT_PAIR_TPP
#define EBML_NG_MASTERELEMENT_PAIR_TPP

#include "ebmlPair.h"
// #include "../ebmlElement.h"
// #include "../basictypes.h"
#include "../struct/unicode.h"
#include "../ebmlDataElement.tpp"

namespace ebml {
    // template<typename K, typename V>
    // ebmlPairType<K, V>::ebmlPairType(const char* ebmlID, const std::wstring& name, const ebmlElementClass* keycls, const ebmlElementClass* valcls)
    // : ebmlPairType(unpackVint(ebmlID), name, keycls, valcls) {}

    template<typename K, typename V>
    ebmlPairType<K, V>::ebmlPairType(const char* ebmlID, const std::wstring& name, const pair_key_class_t<K>* keycls, const pair_value_class_t<V>* valcls)
    : ebmlPairType(unpackVint(ebmlID), name, keycls, valcls) {}

    // template<typename K, typename V>
    // ebmlPairType<K, V>::ebmlPairType(ebmlID_t ebmlID, const std::wstring& name, const ebmlElementClass* keycls, const ebmlElementClass* valcls)
    // : ClsMixin<ebmlPairType<K, V>, ebmlPair<K, V>, ebmlMasterElementClass>(ebmlID, name), _keycls(keycls->ptr<pair_key_class_t<K>>()), _valcls(valcls->ptr<pair_value_class_t<V>>()) {
    //     this->_childTypes = {{keycls, 1, 1}, {valcls, 1, 1}};
    // }

    template<typename K, typename V>
    ebmlPairType<K, V>::ebmlPairType(ebmlID_t ebmlID, const std::wstring& name, const pair_key_class_t<K>* keycls, const pair_value_class_t<V>* valcls)
      : ebmlTypeCRTP<ebmlPairType<K, V>, ebmlPair<K, V>, ebmlMasterElementType>(ebmlID, name), _keycls(keycls), _valcls(valcls) {
        this->_childTypes = {{keycls, 1, 1}, {valcls, 1, 1}};
    }

    // template<typename K, typename V>
    // ebmlElement_sp ebmlPairType<K, V>::operator()(const K& key, const pair_value_t<V>& value) const {
    //     auto elem = new ebmlPair<K, V>(this);
    //     auto elem_sp = ebmlElement_sp(elem);
    //     elem->_setData(key, value);
    //     return elem_sp;
    // }

    // template<typename K, typename V>
    // ebmlElement_sp ebmlPairType<K, V>::operator()(K&& key, pair_value_t<V>&& value) const {
    //     auto elem = new ebmlPair<K, V>(this);
    //     auto elem_sp = ebmlElement_sp(elem);
    //     elem->_setData(std::move(key), std::move(value));
    //     return elem_sp;
    // }

    // template<typename K, typename V>
    // ebmlElement* ebmlPairType<K, V>::_new() const {
    //     return new ebmlPair<K, V>(this);
    // }

    template<typename K, typename V>
    const pair_key_class_t<K>& ebmlPairType<K, V>::keycls() const {
        return *this->_keycls;
    }

    template<typename K, typename V>
    const pair_value_class_t<V>& ebmlPairType<K, V>::valcls() const {
        return *this->_valcls;
    }

    template<typename K, typename V>
    ebmlPair<K, V>::ebmlPair(const ebmlPairType<K, V>* cls) : ebmlElementCRTP<ebmlPairType<K, V>, ebmlPair<K, V>, ebmlMasterElement>(cls) {}

    template<typename K, typename V>
    ebmlPair<K, V>::ebmlPair(const ebmlPairType<K, V>* cls, const K& key, const pair_value_t<V>& value) : ebmlPair(cls) {
        _setData(key, value);
    }

    template<typename K, typename V>
    ebmlPair<K, V>::ebmlPair(const ebmlPairType<K, V>* cls, const K& key, pair_value_t<V>&& value) : ebmlPair(cls) {
        _setData(key, std::move(value));
    }

    template<typename K, typename V>
    ebmlPair<K, V>::ebmlPair(const ebmlPairType<K, V>* cls, K&& key, const pair_value_t<V>& value) : ebmlPair(cls) {
        _setData(std::move(key), value);
    }

    template<typename K, typename V>
    ebmlPair<K, V>::ebmlPair(const ebmlPairType<K, V>* cls, K&& key, pair_value_t<V>&& value) : ebmlPair(cls) {
        _setData(std::move(key), std::move(value));
    }

    template<typename K, typename V>
    ebmlPair<K, V>::ebmlPair(const ebmlPair<K, V>& orig) : ebmlPair(&orig.cls()) {
        _key = orig._key->clone();
        _value = orig._value->clone();
        this->_attachChild(_key);
        this->_attachChild(_value);
    }

    template<typename K, typename V>
    void ebmlPair<K, V>::_setData(const K& key, const pair_value_t<V>& value) {
        auto& c = this->cls();

        auto keyelem = c.keycls()(key);
        this->_key = std::move(keyelem);

        if constexpr (std::is_base_of<ebmlElement, V>::value) {
            this->_value = value;
        } else {
            auto valueelem = c.valcls()(value);
            this->_value = std::move(valueelem);
        }
    }

    template<typename K, typename V>
    void ebmlPair<K, V>::_setData(const K& key, pair_value_t<V>&& value) {
        auto& c = this->cls();
        auto keyelem = c.keycls()(key);
        this->_key = std::move(keyelem);

        if constexpr (std::is_base_of<ebmlElement, V>::value) {
            this->_value = std::move(value);
        } else {
            auto valueelem = c.valcls()(std::move(value));
            this->_value = std::move(valueelem);
        }
    }

    template<typename K, typename V>
    void ebmlPair<K, V>::_setData(K&& key, const pair_value_t<V>& value) {
        auto& c = this->cls();
        auto keyelem = c.keycls()(std::move(key));
        this->_key = std::move(keyelem);

        if constexpr (std::is_base_of<ebmlElement, V>::value) {
            this->_value = value;
        } else {
            auto valueelem = c.valcls()(value);
            this->_value = std::move(valueelem);
        }
    }

    template<typename K, typename V>
    void ebmlPair<K, V>::_setData(K&& key, pair_value_t<V>&& value) {
        auto& c = this->cls();
        auto keyelem = c.keycls()(std::move(key));
        this->_key = std::move(keyelem);

        if constexpr (std::is_base_of<ebmlElement, V>::value) {
            this->_value = std::move(value);
        } else {
            auto valueelem = c.valcls()(std::move(value));
            this->_value = std::move(valueelem);
        }
    }

    template<typename K, typename V>
    void ebmlPair<K, V>::_clear() {
        if (this->_key != nullptr) {
            this->_detachChild(this->_key);
            this->_key = nullptr;
        }

        if (this->_value != nullptr) {
            this->_detachChild(this->_value);
            this->_value = nullptr;
        }
    }

    template<typename K, typename V>
    std::wstring ebmlPair<K, V>::minirepr() const {
        std::wstring result = L"(";
        result += _key->minirepr();
        result += L", ";
        result += _value->minirepr();
        result += L")";
        return result;
    }

    // Element access
    template<typename K, typename V>
    K& ebmlPair<K, V>::key() {
        return _key->data;
    }

    template<typename K, typename V>
    const K& ebmlPair<K, V>::key() const {
        return _key->data;
    }

    template<typename K, typename V>
    pair_value_slot_t<V> ebmlPair<K, V>::value() {
        if constexpr (std::is_base_of<ebmlElement, V>::value) {
            return {this, this->cls()._valueSpec, _value, false};
        } else {
            return _value->data;
        }
    }

    template<typename K, typename V>
    pair_const_value_t<V> ebmlPair<K, V>::value() const {
        if constexpr (std::is_base_of<ebmlElement, V>::value) {
#if RAW
            return dynamic_cast<V*>(_value.get());
#else
            return ebml_const_pointer_cast<const V>(_value);
#endif
        } else {
            return _value->data;
        }
    }


    template<typename K, typename V>
    ebmlPair<K, V>::_iterator::_iterator(const ebmlPair_sp<K, V>& elem, unsigned char pos) {
        this->_elem = elem;
        this->_pos = pos;
    }

    template<typename K, typename V>
    ebmlPair<K, V>::_iterator::_iterator(ebmlPair_sp<K, V>&& elem, unsigned char pos) {
        this->_elem = std::move(elem);
        this->_pos = pos;
    }

    template<typename K, typename V>
    ebmlPair<K, V>::_iterator::_iterator() {}

    template<typename K, typename V>
    ebmlPair<K, V>::_iterator::~_iterator() {}

    template<typename K, typename V>
    ebmlMasterElement::_iterator* ebmlPair<K, V>::_iterator::copy() const {
        return new ebmlPair<K, V>::_iterator(this->_elem, this->_pos);
    }

    template<typename K, typename V>
    ebmlElement& ebmlPair<K, V>::_iterator::operator*() const {
        if (this->_pos == 0) {
            return *this->_elem->_key;
        }

        return *this->_elem->_value;
    }

    template<typename K, typename V>
    ebmlMasterElement::_iterator& ebmlPair<K, V>::_iterator::operator++() {
        ++this->_pos;
        return *this;
    }

    template<typename K, typename V>
    ebmlMasterElement::_iterator& ebmlPair<K, V>::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlPair<K, V>::_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_pos = recast->_pos;
        } else {
            throw std::bad_alloc();
        }

        return *this;
    }

    template<typename K, typename V>
    bool ebmlPair<K, V>::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlPair<K, V>::_iterator*>(&other)) {
            if (this->_elem != recast->_elem) {
                return false;
            }

            if ((this->_pos > 1) and (recast->_pos > 1)) {
                return true;
            }

            return this->_pos == recast->_pos;
        }

        return false;
    }

    template<typename K, typename V>
    bool ebmlPair<K, V>::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
        return not (*this == other);
    }

    template<typename K, typename V>
    ebmlMasterElement::_iterator* ebmlPair<K, V>::_begin() {
        return new ebmlPair<K, V>::_iterator(this->sp(), 0);
    }

    template<typename K, typename V>
    ebmlMasterElement::_iterator* ebmlPair<K, V>::_end() {
        return new ebmlPair<K, V>::_iterator(this->sp(), 2);
    }

    template<typename K, typename V>
    ebmlPair<K, V>::_const_iterator::_const_iterator(const c_ebmlPair_sp<K, V>& elem, unsigned char pos) {
        this->_elem = elem;
        this->_pos = pos;
    }

    template<typename K, typename V>
    ebmlPair<K, V>::_const_iterator::_const_iterator(c_ebmlPair_sp<K, V>&& elem, unsigned char pos) {
        this->_elem = std::move(elem);
        this->_pos = pos;
    }

    template<typename K, typename V>
    ebmlPair<K, V>::_const_iterator::_const_iterator() {}

    template<typename K, typename V>
    ebmlPair<K, V>::_const_iterator::~_const_iterator() {}

    template<typename K, typename V>
    ebmlMasterElement::_const_iterator* ebmlPair<K, V>::_const_iterator::copy() const {
        return new ebmlPair<K, V>::_const_iterator(this->_elem, this->_pos);
    }

    template<typename K, typename V>
    const ebmlElement& ebmlPair<K, V>::_const_iterator::operator*() const {
        if (this->_pos == 0) {
            return *_elem->_key;
        }
        return *_elem->_value;
    }

    template<typename K, typename V>
    ebmlMasterElement::_const_iterator& ebmlPair<K, V>::_const_iterator::operator++() {
        ++this->_pos;
        return *this;
    }

    template<typename K, typename V>
    ebmlMasterElement::_const_iterator& ebmlPair<K, V>::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlPair<K, V>::_const_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_pos = recast->_pos;
        } else {
            throw std::bad_alloc();
        }

        return *this;
    }

    template<typename K, typename V>
    bool ebmlPair<K, V>::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlPair<K, V>::_const_iterator*>(&other)) {
            if (this->_elem != recast->_elem) {
                return false;
            }

            if ((this->_pos > 1) and (recast->_pos > 1)) {
                return true;
            }

            return this->_pos == recast->_pos;
        }

        return false;
    }

    template<typename K, typename V>
    bool ebmlPair<K, V>::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
        return not (*this == other);
    }

    template<typename K, typename V>
    ebmlMasterElement::_const_iterator* ebmlPair<K, V>::_cbegin() const {
        // auto elem = this->shared_from_this();
        // auto recast = std::dynamic_pointer_cast<const ebmlPair<K, V>>(elem);
        // return new ebmlPair<K, V>::_const_iterator(std::move(recast), 0);
        return new ebmlPair<K, V>::_const_iterator(this->sp(), 0);
    }

    template<typename K, typename V>
    ebmlMasterElement::_const_iterator* ebmlPair<K, V>::_cend() const {
        // auto elem = this->shared_from_this();
        // auto recast = std::dynamic_pointer_cast<const ebmlPair<K, V>>(elem);
        // return new ebmlPair<K, V>::_const_iterator(std::move(recast), 2);
        return new ebmlPair<K, V>::_const_iterator(this->sp(), 2);
    }

    template<typename K, typename V>
    void ebmlPair<K, V>::_addChild(const ebmlElement_sp& elem) {
        auto& childcls = elem->cls();
        auto& _cls = this->cls();

        if (&childcls == _cls._keycls) {
            this->_key = elem;
        } else if (&childcls == _cls._valcls) {
            this->_value = elem;
        }
    }

    template<typename K, typename V>
    void ebmlPair<K, V>::_addChild(ebmlElement_sp&& elem) {
        auto& childcls = elem->cls();
        auto& _cls = this->cls();

        if (&childcls == _cls._keycls) {
            this->_key = std::move(elem);
            elem = nullptr;
        } else if (&childcls == _cls._valcls) {
            this->_value = std::move(elem);
        }
    }
}
#endif
