#ifndef EBML_NG_MASTERELEMENT_PAIR_H
#define EBML_NG_MASTERELEMENT_PAIR_H

#include <unordered_map>
#include <vector>
#include <exception>

// #include "base.h"
#include "base/childSlot_t.h"
#include "../ptrs.h"
#include "../ebmlDataElement.h"
#include "../ebmlElement.h"

#define IS_EBML_TYPE(V) std::is_class<V>::value && std::is_base_of<ebmlElement, V>::value

namespace ebml {
    template<typename K, typename V>
    class ebmlPair;

    template<typename K, typename V>
    using ebmlPair_sp = ebml::ptr<ebmlPair<K, V>>;

    template<typename K, typename V>
    using c_ebmlPair_sp = ebml::ptr<const ebmlPair<K, V>>;

    template<typename K>
    using pair_key_class_t = ebmlDataElementClass<K>;

    template<typename K>
    using pair_key_inst_t = ebmlDataElement<K>;

    template<typename V>
    using pair_value_class_t = std::conditional_t<IS_EBML_TYPE(V), ebmlElementClass, ebmlDataElementClass<V>>;

    template<typename V>
    using pair_value_inst_t = std::conditional_t<IS_EBML_TYPE(V), V, ebmlDataElement<V>>;

    template<typename V>
    using pair_value_inst_sp = ebml::ptr<pair_value_inst_t<V>>;

    template<typename V>
    using pair_value_t = std::conditional_t<IS_EBML_TYPE(V), ebml::ptr<V>, V>;

    template<typename V>
    using pair_value_slot_t = std::conditional_t<IS_EBML_TYPE(V), childSlot_t<V>, V&>;

    template<typename V>
#if RAW
    using pair_const_value_t = std::conditional_t<IS_EBML_TYPE(V), const V*, const V&>;
#else
    using pair_const_value_t = std::conditional_t<IS_EBML_TYPE(V), ebml::ptr<const V>, const V&>;
#endif

    template<
        typename K,
        typename V,
        typename H = std::hash<K>,
        typename E = std::equal_to<K>,
        typename A = std::allocator<std::pair<const K, ebml::ptr<ebmlPair<const K, V>>>>,
        typename B = std::allocator<std::pair<const K, pair_value_t<V>>>
    >
    class ebmlMap;


    template<typename K, typename V>
    class ebmlPairClass : public ClsMixin<ebmlPairClass<K, V>, ebmlPair<K, V>, ebmlMasterElementClass> {
    public:
        ebmlPairClass(const char*, const std::wstring&, const pair_key_class_t<K>* keycls, const pair_value_class_t<V>* valcls);
        ebmlPairClass(ebmlID_t, const std::wstring&, const pair_key_class_t<K>* keycls, const pair_value_class_t<V>* valcls);

        // ebmlElement_sp operator()(const K&, const pair_value_t<V>&) const;
        // ebmlElement_sp operator()(K&&, pair_value_t<V>&&) const;

        // template<typename... Args>
        // ebmlElement_sp operator()(const K&, Args...) const;
        //
        // template<typename... Args>
        // ebmlElement_sp operator()(K&&, Args...) const;
        //
        const pair_key_class_t<K>& keycls() const;
        const pair_value_class_t<V>& valcls() const;

    protected:
        // ebmlElement* _new() const;
        childClassSpec_t _valueSpec;

    private:
        // const ebmlDataElementClass<K>* _keycls;
        const ebmlDataElementClass<K>* _keycls;
        const pair_value_class_t<V>* _valcls;
        friend class ebmlPair<K, V>;
    };

    template<typename K, typename V>
    class ebmlPair : public InstMixin<ebmlPairClass<K, V>, ebmlPair<K, V>, ebmlMasterElement> {
        friend class ClsMixin<ebmlPairClass<K, V>, ebmlPair<K, V>, ebmlMasterElementClass>;
        friend class InstMixin<ebmlPairClass<K, V>, ebmlPair<K, V>, ebmlMasterElement>;

    protected:
        ebml::ptr<ebmlDataElement<K>> _key;
        ebml::ptr<pair_value_inst_t<V>> _value;
        // ebml::ptr<ebmlDataElementClass<K>> _key;
        // ebml::ptr<pair_value_inst_t<V>> _value;

    protected:
        ebmlPair(const ebmlPairClass<K, V>*);
        ebmlPair(const ebmlPairClass<K, V>*, const K&, const pair_value_t<V>&);
        ebmlPair(const ebmlPairClass<K, V>*, const K&, pair_value_t<V>&&);
        ebmlPair(const ebmlPairClass<K, V>*, K&&, const pair_value_t<V>&);
        ebmlPair(const ebmlPairClass<K, V>*, K&&, pair_value_t<V>&&);
        ebmlPair(const ebmlPair<K, V>&);

        void _clear() override;
        void _setData(const K&, const pair_value_t<V>&);
        void _setData(const K&, pair_value_t<V>&&);
        void _setData(K&&, const pair_value_t<V>&);
        void _setData(K&&, pair_value_t<V>&&);

    public:
        std::wstring minirepr() const override;
        // const ebmlPairClass<K, V>* cls() const override;

        // Element access
        K& key();
        pair_value_slot_t<V> value();

        // Const element access
        const K& key() const;
        pair_const_value_t<V> value() const;

        // Iteration
    protected:
        class _iterator : public ebmlMasterElement::_iterator {
        private:
            ebmlPair_sp<K, V> _elem;
            unsigned char _pos;

        protected:
            _iterator(const ebmlPair_sp<K, V>& elem, unsigned char pos);
            _iterator(ebmlPair_sp<K, V>&& elem, unsigned char pos);

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy() const;
            ebmlElement& operator*() const;
            ebmlMasterElement::_iterator& operator++();
            ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
            bool operator==(const ebmlMasterElement::_iterator&) const;
            bool operator!=(const ebmlMasterElement::_iterator&) const;

            friend class ebmlPair<K, V>;
            friend class ebmlMasterElement::iterator;
        };

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;


        class _const_iterator : public ebmlMasterElement::_const_iterator {
        private:
            c_ebmlPair_sp<K, V> _elem;
            unsigned char _pos;

        protected:
            _const_iterator(const c_ebmlPair_sp<K, V>& elem, unsigned char pos);
            _const_iterator(c_ebmlPair_sp<K, V>&& elem, unsigned char pos);

        public:
            _const_iterator();
            virtual ~_const_iterator();

            ebmlMasterElement::_const_iterator* copy() const;
            const ebmlElement& operator*() const;
            ebmlMasterElement::_const_iterator& operator++();
            ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
            bool operator==(const ebmlMasterElement::_const_iterator&) const;
            bool operator!=(const ebmlMasterElement::_const_iterator&) const;

            friend class ebmlPair<K, V>;
            friend class ebmlMasterElement::const_iterator;
        };

        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;

        friend class ebmlPairClass<K, V>;

        template<typename Kp, typename Vp, typename H, typename E, typename A, typename B>
        // template<typename H, typename E, typename A, typename B>
        friend class ebmlMap;
    };

    // extern template class childSlot_t<ebmlList>;
    extern template class ebmlPairClass<std::string, unsigned long long>;
    extern template class ebmlPair<std::string, unsigned long long>;
    // extern template class ebmlPairClass<std::string, ebmlList>;
    // extern template class ebmlPair<std::string, ebmlList>;
}
#endif
