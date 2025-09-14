#ifndef EBML_NG_MASTERELEMENT_MAP_H
#define EBML_NG_MASTERELEMENT_MAP_H

#include <unordered_map>
#include <vector>
#include <exception>

// #include "base.h"
#include "base/childSlot_t.h"
#include "ebmlPair.h"
#include "../ebmlDataElement.h"
#include "../ebmlElement.h"

#include "ebmlList.h"

namespace ebml {
    /**
     * @ingroup type_definitions
     * @ingroup type_templates
     * @ingroup master_element_type_definitions
     * @ingroup master_element_type_templates
     * @class ebmlMapType
     * @brief A template class representing a map-like EBML element type.
     *
     * This template defines an EBML element that stores key-value pairs.
     * It is specialized with a key type `K` and a value type `V` to create a concrete element type,
     * providing a structured way to store mapping information within an EBML file.
     */
    template<
        typename K,
        typename V,
        typename H=std::hash<K>,
        typename E = std::equal_to<K>,
        typename A = std::allocator<std::pair<const K, ebml::ptr<ebmlPair<const K, V>>>>,
        typename B = std::allocator<std::pair<const K, pair_value_t<V>>>
    >
    class ebmlMapType : public ebmlTypeCRTP<ebmlMapType<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElementType> {
    public:
        ebmlMapType(const char*, const std::wstring&, const ebmlPairType<const K, V>* paircls);
        ebmlMapType(ebmlID_t, const std::wstring&, const ebmlPairType<const K, V>* paircls);
        const ebmlPairType<const K, V>& paircls() const;

    private:
        const ebmlPairType<const K, V>* _paircls;
        friend class ebmlMap<K, V, H, E, A>;
    };

    template<
        typename K,
        typename V,
        typename H = std::hash<K>,
        typename E = std::equal_to<K>,
        typename A = std::allocator<std::pair<const K, ebml::ptr<ebmlPair<const K, V>>>>
    >
    using ebmlMapData = std::unordered_map<K, ebml::ptr<ebmlPair<const K, V>>, H, E, A>;

    template<
        typename K,
        typename V,
        typename H = std::hash<K>,
        typename E = std::equal_to<K>,
        typename B = std::allocator<std::pair<const K, pair_value_t<V>>>
    >
    using ebmlMapArgs = std::unordered_map<K, pair_value_t<V>, H, E, B>;

    template<typename K, typename V, typename H, typename E, typename A, typename B>
    class ebmlMap : public ebmlElementCRTP<ebmlMapType<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElement> {
        friend class ebmlTypeCRTP<ebmlMapType<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElementType>;
        friend class ebmlElementCRTP<ebmlMapType<K, V, H, E, A, B>, ebmlMap<K, V, H, E, A, B>, ebmlMasterElement>;

    private:
        ebmlMapData<K, V, H, E, A> _data;

    protected:
        ebmlMap(const ebmlMapType<K, V, H, E, A, B>*);
        ebmlMap(const ebmlMapType<K, V, H, E, A, B>*, const std::vector<std::pair<K, pair_value_t<V>>>&);
        ebmlMap(const ebmlMapType<K, V, H, E, A, B>*, std::vector<std::pair<K, pair_value_t<V>>>&&);
        ebmlMap(const ebmlMapType<K, V, H, E, A, B>*, const ebmlMapArgs<K, V, H, E, B>&);
        ebmlMap(const ebmlMapType<K, V, H, E, A, B>*, ebmlMapArgs<K, V, H, E, B>&&);
        ebmlMap(const ebmlMap<K, V, H, E, A, B>&);
        void _clear() override;

        std::vector<ebml::ptr<ebmlPair<const K, V>>> _prepareData(const std::vector<std::pair<K, pair_value_t<V>>>&);
        std::vector<ebml::ptr<ebmlPair<const K, V>>> _prepareData(std::vector<std::pair<K, pair_value_t<V>>>&&);
        ebmlMapData<K, V, H, E, A> _prepareData(const ebmlMapArgs<K, V, H, E, B>&);
        ebmlMapData<K, V, H, E, A> _prepareData(ebmlMapArgs<K, V, H, E, B>&&);

        void _setData(std::vector<ebml::ptr<ebml::ebmlPair<const K, V>>>&& items);

    public:
        // void setData(std::initializer_list<std::pair<K, pair_value_t<V>>>);
        void setData(const std::vector<std::pair<K, pair_value_t<V>>>&);
        void setData(std::vector<std::pair<K, pair_value_t<V>>>&&);
        void setData(const ebmlMapArgs<K, V, H, E, B>&);
        void setData(ebmlMapArgs<K, V, H, E, B>&&);

        std::wstring minirepr() const override;

        // Element access
        pair_value_slot_t<V> operator[](const K&);
        pair_value_slot_t<V> at(const K&);

        std::pair<ebmlMasterElement::iterator, bool> insert(const std::pair<K, pair_value_t<V>>& value);
        std::pair<ebmlMasterElement::iterator, bool> insert(std::pair<K, pair_value_t<V>>&& value);

        size_t erase(const K&);
        size_t count(const K&) const;

        // Const element access
        pair_const_value_t<V> at(const K&) const;

        // Iteration
    protected:
        class _iterator : public ebmlMasterElement::_iterator {
        private:
            ebml::ptr<ebmlMap<K, V, H, E, A, B>> _elem;
            typename ebmlMapData<K, V, H, E, A>::iterator _iter;
            mutable ebml::ptr<ebmlElement> _current;

        protected:
            _iterator(const ebml::ptr<ebmlMap<K, V, H, E, A, B>>& elem, const typename ebmlMapData<K, V, H, E, A>::iterator& iter);
            _iterator(ebml::ptr<ebmlMap<K, V, H, E, A, B>>&& elem, typename ebmlMapData<K, V, H, E, A>::iterator&& iter);

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy() const;

            ebmlElement& operator*() const;
            ebmlMasterElement::_iterator& operator++();
            ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
            bool operator==(const ebmlMasterElement::_iterator&) const;
            bool operator!=(const ebmlMasterElement::_iterator&) const;
            friend class ebmlMap<K, V, H, E, A>;
            friend class ebmlMasterElement::iterator;
        };

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;

        class _const_iterator : public ebmlMasterElement::_const_iterator {
        private:
            ebml::ptr<const ebmlMap<K, V, H, E, A, B>> _elem;
            typename ebmlMapData<K, V, H, E, A>::const_iterator _iter;

        protected:
            _const_iterator(const ebml::ptr<const ebmlMap<K, V, H, E, A, B>>& elem, const typename ebmlMapData<K, V, H, E, A>::const_iterator& iter);
            _const_iterator(ebml::ptr<const ebmlMap<K, V, H, E, A, B>>&& elem, typename ebmlMapData<K, V, H, E, A>::const_iterator&& iter);

        public:
            _const_iterator();
            virtual ~_const_iterator();

            ebmlMasterElement::_const_iterator* copy() const;
            const ebmlElement& operator*() const;
            ebmlMasterElement::_const_iterator& operator++();
            ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
            bool operator==(const ebmlMasterElement::_const_iterator&) const;
            bool operator!=(const ebmlMasterElement::_const_iterator&) const;

            friend class ebmlMap<K, V, H, E, A, B>;
            friend class ebmlMasterElement::const_iterator;
        };

        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;

        friend class ebmlMapType<K, V, H, E, A, B>;
    };
}

#define EXTERN_MAP_TYPE(KEYTYPE) \
    extern template class ebml::ebmlPairType<const KEYTYPE>; \
    extern template class ebml::ebmlPair<const KEYTYPE>; \
    extern template class ebml::ebmlMapType<KEYTYPE>; \
    extern template class ebml::ebmlMap<KEYTYPE>

#define EXTERN_DATAMAP_TYPE(KEYTYPE, VALTYPE) \
    extern template class ebml::ebmlPairType<const KEYTYPE, VALTYPE>; \
    extern template class ebml::ebmlPair<const KEYTYPE, VALTYPE>; \
    extern template class ebml::ebmlMapType<KEYTYPE, VALTYPE>; \
    extern template class ebml::ebmlMap<KEYTYPE, VALTYPE>

#define INST_MAP_TYPE(KEYTYPE) \
    template class ebml::ebmlPairType<const KEYTYPE>; \
    template class ebml::ebmlPair<const KEYTYPE>; \
    template class ebml::ebmlMapType<KEYTYPE>; \
    template class ebml::ebmlMap<KEYTYPE>

#define INST_DATAMAP_TYPE(KEYTYPE, VALTYPE) \
    template class ebml::ebmlPairType<const KEYTYPE, VALTYPE>; \
    template class ebml::ebmlPair<const KEYTYPE, VALTYPE>; \
    template class ebml::ebmlMapType<KEYTYPE, VALTYPE>; \
    template class ebml::ebmlMap<KEYTYPE, VALTYPE>

#define EXTERN_MAP_TYPE_EXT(KEYTYPE, ...) \
    extern template class ebml::ebmlPairType<const KEYTYPE>; \
    extern template class ebml::ebmlPair<const KEYTYPE>; \
    extern template class ebml::ebmlMapType<KEYTYPE, ##__VA_ARGS__>; \
    extern template class ebml::ebmlMap<KEYTYPE, ##__VA_ARGS__>

#define INST_MAP_TYPE_EXT(KEYTYPE, ...) \
    template class ebml::ebmlPairType<const KEYTYPE>; \
    template class ebml::ebmlPair<const KEYTYPE>; \
    template class ebml::ebmlMapType<KEYTYPE, ##__VA_ARGS__>; \
    template class ebml::ebmlMap<KEYTYPE, ##__VA_ARGS__>

#endif
