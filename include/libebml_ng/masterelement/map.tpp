#ifndef EBML_NG_MASTERELEMENT_MAP_TPP
#define EBML_NG_MASTERELEMENT_MAP_TPP

#include "map.h"
#include "../base.h"
#include "../basictypes.h"
#include "../struct/unicode.h"
#include "../template.tpp"

namespace ebml {
    template<typename T>
    ebmlPairClass<T>::ebmlPairClass(const char* ebmlID, const std::wstring& name, const ebmlElementClass* keycls, const ebmlElementClass* valcls)
    : ebmlPairClass(unpackVint(ebmlID), name, keycls, valcls) {}

    template<typename T>
    ebmlPairClass<T>::ebmlPairClass(const char* ebmlID, const std::wstring& name, const ebmlDataElementClass<T>* keycls, const ebmlElementClass* valcls)
    : ebmlPairClass(unpackVint(ebmlID), name, keycls, valcls) {}

    template<typename T>
    ebmlPairClass<T>::ebmlPairClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlElementClass* keycls, const ebmlElementClass* valcls)
    : ebmlMasterElementClass(ebmlID, name), _keycls(dynamic_cast<const ebmlDataElementClass<T>*>(keycls)), _valcls(valcls) {
        if (this->_keycls == nullptr) {
            throw std::invalid_argument("cannot convert to const ebmlDataElementClass<T>*");
        }
        this->_childClasses = {{keycls, 1, 1}, {valcls, 1, 1}};
        this->_valueSpec = {{valcls, 1, 1}};
        // this->_childClasses.add({keycls, 1, 1});
        // this->_childClasses.add({valcls, 1, 1});
        // this->_valueSpec.add({valcls, 1, 1});
    }

    template<typename T>
    ebmlPairClass<T>::ebmlPairClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlDataElementClass<T>* keycls, const ebmlElementClass* valcls)
    : ebmlMasterElementClass(ebmlID, name), _keycls(keycls), _valcls(valcls) {
        this->_childClasses = {{keycls, 1, 1}, {valcls, 1, 1}};
        this->_valueSpec = {{valcls, 1, 1}};
        // this->_childClasses.add({keycls, 1, 1});
        // this->_childClasses.add({valcls, 1, 1});
        // this->_valueSpec.add({valcls, 1, 1});
    }

    template<typename T>
    ebmlElement_sp ebmlPairClass<T>::operator()(const ebmlElement_sp& key, const ebmlElement_sp& value) const {
        auto elem = new ebmlPair<T>(this);
        auto elem_sp = ebmlElement_sp(elem);
        elem->_validate(key, value);
        elem->_setData(key, value);
        return elem_sp;
    }

    template<typename T>
    ebmlElement_sp ebmlPairClass<T>::operator()(ebmlElement_sp&& key, ebmlElement_sp&& value) const {
        auto elem = new ebmlPair<T>(this);
        auto elem_sp = ebmlElement_sp(elem);
        elem->_validate(key, value);
        elem->_setData(std::move(key), std::move(value));
        return elem_sp;
    }

    template<typename T>
    ebmlElement_sp ebmlPairClass<T>::operator()(const T& key, const ebmlElement_sp& value) const {
        auto elem = new ebmlPair<T>(this);
        auto elem_sp = ebmlElement_sp(elem);
        elem->_validate(value);
        elem->_setData(key, value);
        return elem_sp;
    }

    template<typename T>
    ebmlElement_sp ebmlPairClass<T>::operator()(const T& key, ebmlElement_sp&& value) const {
        auto elem = new ebmlPair<T>(this);
        auto elem_sp = ebmlElement_sp(elem);
        elem->_validate(value);
        elem->_setData(key, std::move(value));
        return elem_sp;
    }

    template<typename T>
    ebmlElement_sp ebmlPairClass<T>::operator()(T&& key, ebmlElement_sp&& value) const {
        auto elem = new ebmlPair<T>(this);
        auto elem_sp = ebmlElement_sp(elem);
        elem->_validate(value);
        elem->_setData(std::move(key), std::move(value));
        return elem_sp;
    }

    template<typename T>
    ebmlElement* ebmlPairClass<T>::_new() const {
        return new ebmlPair<T>(this);
    }

    template<typename T>
    const ebmlDataElementClass<T>* ebmlPairClass<T>::keycls() const {
        return dynamic_cast<const ebmlDataElementClass<T>*>(this->_keycls);
    }

    template<typename T>
    const ebmlElementClass* ebmlPairClass<T>::valcls() const {
        return this->_valcls;
    }

    template<typename T>
    ebmlPair<T>::ebmlPair(const ebmlPairClass<T>* cls) : ebmlMasterElement(cls) {}

    template<typename T>
    void ebmlPair<T>::_validate(const ebmlElement_sp& key, const ebmlElement_sp& value) {
        auto c = this->cls();

        if (key->cls() != c->_keycls) {
            std::string errmsg = "ebmlPair(): cannot assign '";
            errmsg += pack(key->cls()->name);
            errmsg += "' element to key slot (expected '";
            errmsg += pack(c->_keycls->name);
            errmsg += "' element)";
            throw std::invalid_argument(errmsg);
        }

        if (value->cls() != c->_valcls) {
            std::string errmsg = "ebmlPair(): cannot assign '";
            errmsg += pack(value->cls()->name);
            errmsg += "' element to value slot (expected '";
            errmsg += pack(c->_valcls->name);
            errmsg += "' element)";
            throw std::invalid_argument(errmsg);
        }

        this->_attachChild(key);
        this->_attachChild(value);
    }

    template<typename T>
    void ebmlPair<T>::_validate(const ebmlElement_sp& value) {
        auto c = this->cls();

        if (value->cls() != c->_valcls) {
            std::string errmsg = "ebmlPair(): cannot assign '";
            errmsg += pack(value->cls()->name);
            errmsg += "' element to value slot (expected '";
            errmsg += pack(c->_valcls->name);
            errmsg += "' element)";
            throw std::invalid_argument(errmsg);
        }

        this->_attachChild(value);
    }

    template<typename T>
    void ebmlPair<T>::_setData(const ebmlElement_sp& key, const ebmlElement_sp& value) {
        this->_key = key;
        this->_value = value;
    }

    template<typename T>
    void ebmlPair<T>::_setData(ebmlElement_sp&& key, ebmlElement_sp&& value) {
        this->_key = std::move(key);
        this->_value = std::move(value);
    }

    template<typename T>
    void ebmlPair<T>::_setData(const T& key, const ebmlElement_sp& value) {
        auto c = this->cls();
        auto keyelem = (*c->_keycls)(key);
        this->_key = std::move(keyelem);
        this->_value = value;
    }

    template<typename T>
    void ebmlPair<T>::_setData(const T& key, ebmlElement_sp&& value) {
        auto c = this->cls();
        auto keyelem = (*c->_keycls)(key);
        this->_key = std::move(keyelem);
        this->_value = std::move(value);
    }

    template<typename T>
    void ebmlPair<T>::_setData(T&& key, ebmlElement_sp&& value) {
        auto c = this->cls();
        auto keyelem = (*c->_keycls)(std::move(key));
        this->_key = std::move(keyelem);
        this->_value = std::move(value);
    }

    template<typename T>
    void ebmlPair<T>::_clear() {
        if (this->_key != nullptr) {
            this->_detachChild(this->_key);
            this->_key = nullptr;
        }

        if (this->_value != nullptr) {
            this->_detachChild(this->_value);
            this->_value = nullptr;
        }
    }

    template<typename T>
    std::wstring ebmlPair<T>::minirepr() const {
        std::wstring result = L"(";
        result += this->_key->minirepr();
        result += L", ";
        result += this->_value->minirepr();
        result += L")";
        return result;
    }

    template<typename T>
    const ebmlPairClass<T>* ebmlPair<T>::cls() const {
        return this->_cls->ptr<ebmlPairClass<T>>();
    }

    // Element access
    template<typename T>
    T& ebmlPair<T>::key() {
        return data<T>(this->_key);
    }

    template<typename T>
    const T& ebmlPair<T>::key() const {
        return data<T>(this->_key);
    }

    template<typename T>
    const ebmlElement_sp& ebmlPair<T>::keyElem() {
        return this->_key;
    }

    template<typename T>
    childSlot_t ebmlPair<T>::value() {
        return {this, this->cls()->_valueSpec, this->_value, false};
    }

#if RAW
    template<typename T>
    const ebmlElement* ebmlPair<T>::value() const {
        return this->_value.get();
    }

    template<typename T>
    const ebmlElement* ebmlPair<T>::keyElem() const {
        return this->_key.get();
    }
#else
    template<typename T>
    c_ebmlElement_sp ebmlPair<T>::value() const {
        return std::const_pointer_cast<const ebmlElement>(this->_value);
    }

    template<typename T>
    c_ebmlElement_sp ebmlPair<T>::keyElem() const {
        return std::const_pointer_cast<const ebmlElement>(this->_key);
    }
#endif

    template<typename T>
    ebmlPair<T>::_iterator::_iterator(const ebmlPair_sp<T>& elem, unsigned char pos) {
        this->_elem = elem;
        this->_pos = pos;
    }

    template<typename T>
    ebmlPair<T>::_iterator::_iterator(ebmlPair_sp<T>&& elem, unsigned char pos) {
        this->_elem = std::move(elem);
        this->_pos = pos;
    }

    template<typename T>
    ebmlPair<T>::_iterator::_iterator() {}

    template<typename T>
    ebmlPair<T>::_iterator::~_iterator() {}

    template<typename T>
    ebmlMasterElement::_iterator* ebmlPair<T>::_iterator::copy() const {
        return new ebmlPair<T>::_iterator(this->_elem, this->_pos);
    }

    template<typename T>
    const ebmlElement_sp& ebmlPair<T>::_iterator::operator*() const {
        if (this->_pos == 0) {
            return this->_elem->_key;
        }
        return this->_elem->_value;

    }

    template<typename T>
    ebmlMasterElement::_iterator& ebmlPair<T>::_iterator::operator++() {
        ++this->_pos;
        return *this;
    }

    template<typename T>
    ebmlMasterElement::_iterator& ebmlPair<T>::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlPair<T>::_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_pos = recast->_pos;
        } else {
            throw std::bad_alloc();
        }

        return *this;
    }

    template<typename T>
    bool ebmlPair<T>::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlPair<T>::_iterator*>(&other)) {
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

    template<typename T>
    bool ebmlPair<T>::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
        return not (*this == other);
    }

    template<typename T>
    ebmlMasterElement::_iterator* ebmlPair<T>::_begin() {
        auto elem = this->shared_from_this();
        auto recast = std::dynamic_pointer_cast<ebmlPair<T>>(elem);
        return new ebmlPair<T>::_iterator(std::move(recast), 0);
    }

    template<typename T>
    ebmlMasterElement::_iterator* ebmlPair<T>::_end() {
        auto elem = this->shared_from_this();
        auto recast = std::dynamic_pointer_cast<ebmlPair<T>>(elem);
        return new ebmlPair<T>::_iterator(std::move(recast), 2);
    }

    template<typename T>
    ebmlPair<T>::_const_iterator::_const_iterator(const c_ebmlPair_sp<T>& elem, unsigned char pos) {
        this->_elem = elem;
        this->_pos = pos;
    }

    template<typename T>
    ebmlPair<T>::_const_iterator::_const_iterator(c_ebmlPair_sp<T>&& elem, unsigned char pos) {
        this->_elem = std::move(elem);
        this->_pos = pos;
    }

    template<typename T>
    ebmlPair<T>::_const_iterator::_const_iterator() {}

    template<typename T>
    ebmlPair<T>::_const_iterator::~_const_iterator() {}

    template<typename T>
    ebmlMasterElement::_const_iterator* ebmlPair<T>::_const_iterator::copy() const {
        return new ebmlPair<T>::_const_iterator(this->_elem, this->_pos);
    }

#if RAW
    template<typename T>
    const ebmlElement* ebmlPair<T>::_const_iterator::operator*() const {
        if (this->_pos == 0) {
            return this->_elem->_key.get();
        }
        return this->_elem->_value.get();
    }
#else
    template<typename T>
    c_ebmlElement_sp ebmlPair<T>::_const_iterator::operator*() const {
        if (this->_pos == 0) {
            return std::const_pointer_cast<const ebmlElement>(this->_elem->_key);
        }
        return std::const_pointer_cast<const ebmlElement>(this->_elem->_value);
    }
#endif
    template<typename T>
    ebmlMasterElement::_const_iterator& ebmlPair<T>::_const_iterator::operator++() {
        ++this->_pos;
        return *this;
    }

    template<typename T>
    ebmlMasterElement::_const_iterator& ebmlPair<T>::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlPair<T>::_const_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_pos = recast->_pos;
        } else {
            throw std::bad_alloc();
        }

        return *this;
    }

    template<typename T>
    bool ebmlPair<T>::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlPair<T>::_const_iterator*>(&other)) {
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

    template<typename T>
    bool ebmlPair<T>::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
        return not (*this == other);
    }

    template<typename T>
    ebmlMasterElement::_const_iterator* ebmlPair<T>::_cbegin() const {
        auto elem = this->shared_from_this();
        auto recast = std::dynamic_pointer_cast<const ebmlPair<T>>(elem);
        return new ebmlPair<T>::_const_iterator(std::move(recast), 0);
    }

    template<typename T>
    ebmlMasterElement::_const_iterator* ebmlPair<T>::_cend() const {
        auto elem = this->shared_from_this();
        auto recast = std::dynamic_pointer_cast<const ebmlPair<T>>(elem);
        return new ebmlPair<T>::_const_iterator(std::move(recast), 2);
    }

    template<typename T>
    void ebmlPair<T>::_addChild(const ebmlElement_sp& elem) {
        auto childcls = elem->cls();
        auto _cls = this->cls();

        if (childcls == _cls->_keycls) {
            this->_key = elem;
        } else if (childcls == _cls->_valcls) {
            this->_value = elem;
        }
    }

    template<typename T>
    void ebmlPair<T>::_addChild(ebmlElement_sp&& elem) {
        auto childcls = elem->cls();
        auto _cls = this->cls();

        if (childcls == _cls->_keycls) {
            this->_key = std::move(elem);
            elem = nullptr;
        } else if (childcls == _cls->_valcls) {
            this->_value = std::move(elem);
        }
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMapClass<K, H, E, A>::ebmlMapClass(const char* ebmlID, const std::wstring& name, const ebmlElementClass* paircls)
    : ebmlMapClass(unpackVint(ebmlID), name, paircls) {}

    template<typename K, typename H, typename E, typename A>
    ebmlMapClass<K, H, E, A>::ebmlMapClass(const char* ebmlID, const std::wstring& name, const ebmlPairClass<const K>* paircls)
    : ebmlMapClass(unpackVint(ebmlID), name, paircls) {}

    template<typename K, typename H, typename E, typename A>
    ebmlMapClass<K, H, E, A>::ebmlMapClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlElementClass* paircls)
    : ebmlMasterElementClass(ebmlID, name), _paircls(paircls->ptr<ebmlPairClass<const K>>()) {
        this->_childClasses.add({this->_paircls, 0, -1});
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMapClass<K, H, E, A>::ebmlMapClass(ebmlID_t ebmlID, const std::wstring& name, const ebmlPairClass<const K>* paircls)
    : ebmlMasterElementClass(ebmlID, name), _paircls(paircls) {
        this->_childClasses.add({this->_paircls, 0, -1});
    }

    template<typename K, typename H, typename E, typename A>
    const ebmlPairClass<const K>* ebmlMapClass<K, H, E, A>::paircls() const {
        return this->_paircls;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()() const {
        return ebmlElement_sp(this->_new());
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(std::initializer_list<ebmlElement_sp> items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::list<ebmlElement_sp> items_l = items;
        elem->_validateData(items_l);
        elem->_setData(std::move(items_l));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(std::initializer_list<std::pair<ebmlElement_sp, ebmlElement_sp>> items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::list<std::pair<ebmlElement_sp, ebmlElement_sp>> items_l = items;
        std::list<ebmlElement_sp> prepared = elem->_prepareData(std::move(items_l));
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(std::initializer_list<std::pair<K, ebmlElement_sp>> items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::list<std::pair<K, ebmlElement_sp>> items_l = items;
        std::list<ebmlElement_sp> prepared = elem->_prepareData(std::move(items_l));
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(const std::list<ebmlElement_sp>& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        elem->_validateData(items);
        elem->_setData(items);
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(std::list<ebmlElement_sp>&& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        elem->_validateData(items);
        elem->_setData(std::move(items));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(const std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::list<ebmlElement_sp> prepared = elem->_prepareData(items);
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::list<ebmlElement_sp> prepared = elem->_prepareData(std::move(items));
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(const std::list<std::pair<K, ebmlElement_sp>>& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::list<ebmlElement_sp> prepared = elem->_prepareData(items);
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(std::list<std::pair<K, ebmlElement_sp>>&& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::list<ebmlElement_sp> prepared = elem->_prepareData(std::move(items));
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(const std::unordered_map<K, ebmlElement_sp, H, E, A>& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::unordered_map<K, ebmlElement_sp> prepared = elem->_prepareData(items);
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement_sp ebmlMapClass<K, H, E, A>::operator()(std::unordered_map<K, ebmlElement_sp, H, E, A>&& items) const {
        auto elem = new ebmlMap<K, H, E, A>(this);
        auto elem_sp = ebmlElement_sp(elem);
        std::unordered_map<K, ebmlElement_sp> prepared = elem->_prepareData(std::move(items));
        elem->_setData(std::move(prepared));
        return elem_sp;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlElement* ebmlMapClass<K, H, E, A>::_new() const {
        return new ebmlMap<K, H, E, A>(this);
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::ebmlMap(const ebmlMapClass<K, H, E, A>* cls) : ebmlMasterElement(cls) {}

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_validateData(const std::list<ebmlElement_sp>& items) {
        auto c = this->cls()->paircls();
        auto start = items.cbegin();
        auto end = items.cend();
        auto iter = start;

        while (iter != end) {
            try {
                auto& item = *iter;

                if (item->cls() != c) {
                    std::string errmsg = "ebmlMap(): expected '";
                    errmsg += pack(c->name);
                    errmsg += "' object, got '";
                    errmsg += pack(item->cls()->name);
                    errmsg += "' instead";
                    throw std::invalid_argument(errmsg);
                }

                this->_attachChild(item);
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    auto& item = *iter;
                    this->_detachChild(item);
                }

                throw;
            }
        }
    }

    // Initialize from list
    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_setData(const std::list<ebmlElement_sp>& items) {
        for (auto& item : items) {
            const K& key = item->ref<ebmlPair<const K>>().key();
            this->_data[key] = item;
        }
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_setData(std::list<ebmlElement_sp>&& items) {
        for (auto& item : items) {
            const K& key = item->ref<ebmlPair<const K>>().key();
            this->_data[key] = std::move(item);
        }
    }

    template<typename K, typename H, typename E, typename A>
    std::list<ebmlElement_sp> ebmlMap<K, H, E, A>::_prepareData(const std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>& items) {
        std::list<ebmlElement_sp> pairlist;
        auto iter = items.cbegin();
        auto end = items.cend();
        auto& c = *this->cls()->paircls();

        while (iter != end) {
            auto& item = *iter;
            auto pair = c(item.first, item.second);
            pairlist.push_back(std::move(pair));
            ++iter;
        }

        return pairlist;
    }

    template<typename K, typename H, typename E, typename A>
    std::list<ebmlElement_sp> ebmlMap<K, H, E, A>::_prepareData(std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&& items) {
        std::list<ebmlElement_sp> pairlist;
        auto start = items.begin();
        auto iter = start;
        auto end = items.end();
        auto& c = *this->cls()->paircls();

        while (iter != end) {
            try {
                auto& item = *iter;
                auto pair = c(std::move(item.first), std::move(item.second));
                pairlist.push_back(std::move(pair));
            } catch (std::exception& e) {
                auto pairiter = pairlist.end();

                while (iter != start) {
                    --iter;
                    --pairiter;
                    auto& pair = (*pairiter)->ref<ebmlPair<const K>>();
                    *iter = {std::move(pair._key), std::move(pair._value)};
                }

                throw;
            }
            ++iter;
        }

        items.clear();
        return pairlist;
    }

    template<typename K, typename H, typename E, typename A>
    std::list<ebmlElement_sp> ebmlMap<K, H, E, A>::_prepareData(const std::list<std::pair<K, ebmlElement_sp>>& items) {
        std::list<ebmlElement_sp> pairlist;
        auto iter = items.cbegin();
        auto end = items.cend();
        auto& c = *this->cls()->paircls();
        auto& k = *c.keycls();

        while (iter != end) {
            auto& item = *iter;
            auto key = k(item.first);
            auto pair = c(std::move(key), item.second);
            pairlist.push_back(std::move(pair));
            ++iter;
        }

        return pairlist;
    }

    template<typename K, typename H, typename E, typename A>
    std::list<ebmlElement_sp> ebmlMap<K, H, E, A>::_prepareData(std::list<std::pair<K, ebmlElement_sp>>&& items) {
        std::list<ebmlElement_sp> pairlist;
        auto start = items.begin();
        auto iter = start;
        auto end = items.end();
        auto& c = *this->cls()->paircls();
        auto& k = *c.keycls();

        while (iter != end) {
            try {
                auto& item = *iter;
                auto key = k(std::move(item.first));
                auto pair = c(std::move(key), std::move(item.second));
                pairlist.push_back(std::move(pair));
            } catch (std::exception& e) {
                auto pairiter = pairlist.end();

                while (iter != start) {
                    --iter;
                    --pairiter;
                    auto& pair = (*pairiter)->ref<ebmlPair<const K>>();
                    *iter = {pair.key(), std::move(pair._value)};
                }

                throw;
            }
            ++iter;
        }

        items.clear();
        return pairlist;
    }

    template<typename K, typename H, typename E, typename A>
    std::unordered_map<K, ebmlElement_sp, H, E, A> ebmlMap<K, H, E, A>::_prepareData(const std::unordered_map<K, ebmlElement_sp, H, E, A>& items) {
        std::unordered_map<K, ebmlElement_sp, H, E, A> map;
        auto iter = items.cbegin();
        auto end = items.cend();
        auto& c = *this->cls()->paircls();
        auto& k = *c.keycls();
        auto& v = *c.valcls();

        while (iter != end) {
            auto& item = *iter;

            if (item.second->cls() != &v) {
                std::string errmsg = "ebmlMap(): expected '";
                errmsg += pack(v.name);
                errmsg += "' object, got '";
                errmsg += pack(item.second->cls()->name);
                errmsg += "' instead";
                throw std::invalid_argument(errmsg);
            }

            auto key = k(item.first);
            auto pair = c(std::move(key), item.second);
            this->_attachChild(pair);
            map.insert({item.first, std::move(pair)});
            ++iter;
        }

        return map;
    }

    template<typename K, typename H, typename E, typename A>
    std::unordered_map<K, ebmlElement_sp, H, E, A> ebmlMap<K, H, E, A>::_prepareData(std::unordered_map<K, ebmlElement_sp, H, E, A>&& items) {
        std::unordered_map<K, ebmlElement_sp, H, E, A> map;
        auto iter = items.cbegin();
        auto end = items.cend();
        auto& c = *this->cls()->paircls();
        auto& k = *c.keycls();
        auto& v = *c.valcls();

        while (iter != end) {
            try {
                auto& item = *iter;

                if (item.second->cls() != &v) {
                    std::string errmsg = "ebmlMap(): expected '";
                    errmsg += pack(v.name);
                    errmsg += "' object, got '";
                    errmsg += pack(item.second->cls()->name);
                    errmsg += "' instead";
                    throw std::invalid_argument(errmsg);
                }

                auto key = k(item.first);
                auto pair = c(std::move(key), item.second);
                this->_attachChild(pair);
                map.insert({item.first, std::move(pair)});
                ++iter;
            } catch (std::exception& e) {
                for (auto& p : map) {
                    items[p.first] = std::move(p.second->template ptr<ebmlPair<const K>>()->value());
                }
                throw;
            }
        }

        items.clear();
        return map;
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_setData(const std::unordered_map<K, ebmlElement_sp, H, E, A>& items) {
        this->_data = items;
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_setData(std::unordered_map<K, ebmlElement_sp, H, E, A>&& items) {
        this->_data = std::move(items);
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(std::initializer_list<ebmlElement_sp> items) {
        std::list<ebmlElement_sp> items_l = items;
        this->_validateData(items);
        this->_clear();
        this->_setData(std::move(items_l));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(std::initializer_list<std::pair<ebmlElement_sp, ebmlElement_sp>> items) {
        std::list<std::pair<ebmlElement_sp, ebmlElement_sp>> items_l = items;
        std::list<ebmlElement_sp> prepared = this->_prepareData(std::move(items_l));
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(std::initializer_list<std::pair<K, ebmlElement_sp>> items) {
        std::list<std::pair<K, ebmlElement_sp>> items_l = items;
        std::list<ebmlElement_sp> prepared = this->_prepareData(std::move(items_l));
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(const std::list<ebmlElement_sp>& items) {
        this->_validateData(items);
        this->_clear();
        this->_setData(items);
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(std::list<ebmlElement_sp>&& items) {
        this->_validateData(items);
        this->_clear();
        this->_setData(std::move(items));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(const std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>& items) {
        std::list<ebmlElement_sp> prepared = this->_prepareData(items);
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&& items) {
        std::list<ebmlElement_sp> prepared = this->_prepareData(std::move(items));
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(const std::list<std::pair<K, ebmlElement_sp>>& items) {
        std::list<ebmlElement_sp> prepared = this->_prepareData(items);
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(std::list<std::pair<K, ebmlElement_sp>>&& items) {
        std::list<ebmlElement_sp> prepared = this->_prepareData(std::move(items));
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(const std::unordered_map<K, ebmlElement_sp, H, E, A>& items) {
        std::unordered_map<K, ebmlElement_sp> prepared = this->_prepareData(items);
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::setData(std::unordered_map<K, ebmlElement_sp, H, E, A>&& items) {
        std::unordered_map<K, ebmlElement_sp> prepared = this->_prepareData(std::move(items));
        this->_clear();
        this->_setData(std::move(prepared));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_clear() {
        auto iter = this->_data.cbegin();
        auto end = this->_data.cbegin();

        while (iter != end) {
            const auto& p = *iter;
            this->_detachChild(p.second);
            ++iter;
        }
        this->_data.clear();
    }

    template<typename K, typename H, typename E, typename A>
    std::wstring ebmlMap<K, H, E, A>::minirepr() const {
        std::wstring result = L"{";

        auto iter = this->_data.cbegin();
        auto end = this->_data.cend();

        if (iter != end) {
            const auto& p = *iter;
            const auto& elem = p.second->template ref<ebmlPair<const K>>();

            result += elem.keyElem()->minirepr();
            result += L": ";
            result += elem.value()->repr();

            ++iter;
        }

        while (iter != end) {
            const auto& p = *iter;
            const auto& elem = p.second->template ref<ebmlPair<const K>>();

            result += L", ";
            result += elem.keyElem()->minirepr();
            result += L": ";
            result += elem.value()->repr();

            ++iter;
        }

        result += L"}";
        return result;
    }

    // Element access
    template<typename K, typename H, typename E, typename A>
    childSlot_t ebmlMap<K, H, E, A>::operator[](const K& key) {
        ebmlElement_sp& ebmlpair = this->_data[key];

        if (ebmlpair == nullptr) {
            const auto& paircls = *this->cls()->paircls();
            const auto& valcls = *paircls.valcls();
            auto elem = paircls(key, valcls());
            this->_attachChild(elem);
            ebmlpair = std::move(elem);
        }

        return ebmlpair->ref<ebmlPair<const K>>().value();
    }

    template<typename K, typename H, typename E, typename A>
    childSlot_t ebmlMap<K, H, E, A>::at(const K& key) {
        ebmlElement_sp& ebmlpair = this->_data.at(key);

        if (ebmlpair == nullptr) {
            const auto& paircls = *this->cls()->paircls();
            const auto& valcls = *paircls.valcls();
            auto elem = paircls(key, valcls());
            this->_attachChild(elem);
            ebmlpair = std::move(elem);
        }

        return ebmlpair->ref<ebmlPair<const K>>().value();
    }

    template<typename K, typename H, typename E, typename A>
    const ebmlMapClass<K, H, E, A>* ebmlMap<K, H, E, A>::cls() const {
        return this->_cls->ptr<ebmlMapClass<K, H, E, A>>();
    }

    template<typename K, typename H, typename E, typename A>
    std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, H, E, A>::insert(const ebmlElement_sp& pair) {
        if (pair->cls() != this->cls()->paircls()) {
            std::string errmsg = "insert(): expected '";
            errmsg += pack(this->cls()->paircls()->name);
            errmsg += "' object, got '";
            errmsg += pack(pair->cls()->name);
            errmsg += "' instead";
            throw std::invalid_argument(errmsg);
        }

        const K& key = pair->ref<ebmlPair<const K>>().key();

        auto p = this->_data.insert({key, pair});

        if (p.second) {
            this->_attachChild(pair);
        }

        auto iter = new ebmlMap<K, H, E, A>::_iterator(this->shared_from_this(), p.first);
        return {ebmlMasterElement::make_iter(iter), p.second};
    }

    template<typename K, typename H, typename E, typename A>
    std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, H, E, A>::insert(ebmlElement_sp&& pair) {
        if (pair->cls() != this->cls()->paircls()) {
            std::string errmsg = "insert(): expected '";
            errmsg += pack(this->cls()->paircls()->name);
            errmsg += "' object, got '";
            errmsg += pack(pair->cls()->name);
            errmsg += "' instead";
            throw std::invalid_argument(errmsg);
        }

        const K& key = pair->ref<ebmlPair<const K>>().key();

        auto p = this->_data.insert({key, std::move(pair)});

        if (p.second) {
            this->_attachChild(this->_data.at(key));
        }

        auto iter = new ebmlMap<K, H, E, A>::_iterator(this->shared_from_this(), p.first);
        return {ebmlMasterElement::make_iter(iter), p.second};
    }

    template<typename K, typename H, typename E, typename A>
    std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, H, E, A>::insert(const std::pair<ebmlElement_sp, ebmlElement_sp>& pair) {
        auto epair = (*this->cls()->paircls())(pair.first, pair.second);
        return this->insert(std::move(epair));
    }

    template<typename K, typename H, typename E, typename A>
    std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, H, E, A>::insert(std::pair<ebmlElement_sp, ebmlElement_sp>&& pair) {
        auto epair = (*this->cls()->paircls())(std::move(pair.first), std::move(pair.second));
        return this->insert(std::move(epair));
    }

    template<typename K, typename H, typename E, typename A>
    std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, H, E, A>::insert(const std::pair<K, ebmlElement_sp>& pair) {
        auto key = (*this->cls()->paircls()->keycls())(pair.first);
        auto epair = (*this->cls()->paircls())(std::move(key), pair.second);
        return this->insert(std::move(epair));
    }

    template<typename K, typename H, typename E, typename A>
    std::pair<ebmlMasterElement::iterator, bool> ebmlMap<K, H, E, A>::insert(std::pair<K, ebmlElement_sp>&& pair) {
        auto key = (*this->cls()->paircls()->keycls())(std::move(pair.first));
        auto epair = (*this->cls()->paircls())(std::move(key), std::move(pair.second));
        return this->insert(std::move(epair));
    }

    // Const element access
#if RAW
    template<typename K, typename H, typename E, typename A>
    const ebmlElement* ebmlMap<K, H, E, A>::at(const K& key) const {
        const ebmlElement_sp& ebmlpair = this->_data.at(key);
        return ebmlpair->ref<const ebmlPair<const K>>().value();
    }

    template<typename K, typename H, typename E, typename A>
    const ebmlElement* ebmlMap<K, H, E, A>::keyAt(const K& key) const {
        const ebmlElement_sp& ebmlpair = this->_data.at(key);
        return ebmlpair->ref<const ebmlPair<const K>>().keyElem();
    }
#else
    template<typename K, typename H, typename E, typename A>
    c_ebmlElement_sp ebmlMap<K, H, E, A>::at(const K& key) const {
        const ebmlElement_sp& ebmlpair = this->_data.at(key);
        return ebmlpair->ref<const ebmlPair<const K>>().value();
    }

    template<typename K, typename H, typename E, typename A>
    c_ebmlElement_sp ebmlMap<K, H, E, A>::keyAt(const K& key) const {
        const ebmlElement_sp& ebmlpair = this->_data.at(key);
        return ebmlpair->ref<const ebmlPair<const K>>().keyElem();
    }
#endif

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_iterator::_iterator(const ebmlElement_sp& elem, const typename std::unordered_map<K, ebmlElement_sp, H, E, A>::iterator& iter)
    : _elem(elem), _iter(iter) {}

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_iterator::_iterator(ebmlElement_sp&& elem, typename std::unordered_map<K, ebmlElement_sp, H, E, A>::iterator&& iter)
    : _elem(std::move(elem)), _iter(std::move(iter)) {}

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_iterator::_iterator() {}

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_iterator::~_iterator() {}

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_iterator* ebmlMap<K, H, E, A>::_iterator::copy() const {
        return new ebmlMap<K, H, E, A>::_iterator(this->_elem, this->_iter);
    }

    template<typename K, typename H, typename E, typename A>
    const ebmlElement_sp& ebmlMap<K, H, E, A>::_iterator::operator*() const {
        return (*this->_iter).second;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_iterator& ebmlMap<K, H, E, A>::_iterator::operator++() {
        ++this->_iter;
        return *this;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_iterator& ebmlMap<K, H, E, A>::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlMap<K, H, E, A>::_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_iter = recast->_iter;
        } else {
            throw std::invalid_argument("invalid iterator type");
        }

        return *this;
    }

    template<typename K, typename H, typename E, typename A>
    bool ebmlMap<K, H, E, A>::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlMap<K, H, E, A>::_iterator*>(&other)) {
            return ((this->_elem == recast->_elem)
                and (this->_iter == recast->_iter));
        }

        return false;
    }

    template<typename K, typename H, typename E, typename A>
    bool ebmlMap<K, H, E, A>::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
        return not (*this == other);
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_iterator* ebmlMap<K, H, E, A>::_begin() {
        auto elem = this->shared_from_this();
        return new ebmlMap<K, H, E, A>::_iterator(std::move(elem), std::move(this->_data.begin()));
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_iterator* ebmlMap<K, H, E, A>::_end() {
        auto elem = this->shared_from_this();
        return new ebmlMap<K, H, E, A>::_iterator(std::move(elem), std::move(this->_data.end()));
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_const_iterator::_const_iterator(const c_ebmlElement_sp& elem, const typename std::unordered_map<K, ebmlElement_sp, H, E, A>::const_iterator& iter)
    : _elem(elem), _iter(iter) {}

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_const_iterator::_const_iterator(c_ebmlElement_sp&& elem, typename std::unordered_map<K, ebmlElement_sp, H, E, A>::const_iterator&& iter)
    : _elem(std::move(elem)), _iter(std::move(iter)) {}

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_const_iterator::_const_iterator() {}

    template<typename K, typename H, typename E, typename A>
    ebmlMap<K, H, E, A>::_const_iterator::~_const_iterator() {}

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_const_iterator* ebmlMap<K, H, E, A>::_const_iterator::copy() const {
        return new ebmlMap<K, H, E, A>::_const_iterator(this->_elem, this->_iter);
    }

#if RAW
    template<typename K, typename H, typename E, typename A>
    const ebmlElement* ebmlMap<K, H, E, A>::_const_iterator::operator*() const {
        return (*this->_iter).second.get();
    }
#else
    template<typename K, typename H, typename E, typename A>
    c_ebmlElement_sp ebmlMap<K, H, E, A>::_const_iterator::operator*() const {
        return std::const_pointer_cast<const ebmlElement>((*this->_iter).second);
    }
#endif

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_const_iterator& ebmlMap<K, H, E, A>::_const_iterator::operator++() {
        ++this->_iter;
        return *this;
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_const_iterator& ebmlMap<K, H, E, A>::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
        if (auto recast = dynamic_cast<const ebmlMap<K, H, E, A>::_const_iterator*>(&other)) {
            this->_elem = recast->_elem;
            this->_iter = recast->_iter;
        } else {
            throw std::invalid_argument("invalid iterator type");
        }

        return *this;
    }

    template<typename K, typename H, typename E, typename A>
    bool ebmlMap<K, H, E, A>::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
        if (auto recast = dynamic_cast<const ebmlMap<K, H, E, A>::_const_iterator*>(&other)) {
            return ((this->_elem == recast->_elem)
                and (this->_iter == recast->_iter));
        }

        return false;
    }

    template<typename K, typename H, typename E, typename A>
    bool ebmlMap<K, H, E, A>::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
        return not (*this == other);
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_const_iterator* ebmlMap<K, H, E, A>::_cbegin() const {
        auto elem = this->shared_from_this();
        auto c_elem = std::const_pointer_cast<const ebmlElement>(elem);
        return new ebmlMap<K, H, E, A>::_const_iterator(std::move(c_elem), std::move(this->_data.cbegin()));
    }

    template<typename K, typename H, typename E, typename A>
    ebmlMasterElement::_const_iterator* ebmlMap<K, H, E, A>::_cend() const {
        auto elem = this->shared_from_this();
        auto c_elem = std::const_pointer_cast<const ebmlElement>(elem);
        return new ebmlMap<K, H, E, A>::_const_iterator(std::move(c_elem), std::move(this->_data.cend()));
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_addChild(const ebmlElement_sp& elem) {
        auto recast = dynamic_cast<ebmlPair<const K>*>(elem.get());
        const K& key = recast->key();

        if (this->_data.count(key)) {
            this->_detachChild(this->_data[key]);
            this->_data[key] = elem;
        } else {
            this->_data.insert({key, elem});
        }
    }

    template<typename K, typename H, typename E, typename A>
    void ebmlMap<K, H, E, A>::_addChild(ebmlElement_sp&& elem) {
        auto recast = dynamic_cast<ebmlPair<const K>*>(elem.get());
        const K& key = recast->key();

        if (this->_data.count(key)) {
            this->_detachChild(this->_data[key]);
            this->_data[key] = std::move(elem);
        } else {
            this->_data.insert({key, std::move(elem)});
        }
    }
}
#endif
