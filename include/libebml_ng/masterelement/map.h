#ifndef EBML_NG_MASTERELEMENT_MAP_H
#define EBML_NG_MASTERELEMENT_MAP_H

#include "libebml_ng/base.h"
#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/template.h"
#include <unordered_map>
#include <vector>
#include <exception>

namespace ebml {
    template<typename T>
    class ebmlPair;

    template<typename T>
    using ebmlPair_sp = std::shared_ptr<ebmlPair<T>>;

    template<typename T>
    using c_ebmlPair_sp = std::shared_ptr<const ebmlPair<T>>;

    template<typename T>
    class ebmlPairClass : public ebmlMasterElementClass {
    public:
        ebmlPairClass(const char*, const std::wstring&, const ebmlElementClass* keycls, const ebmlElementClass* valcls);
        ebmlPairClass(const char*, const std::wstring&, const ebmlDataElementClass<T>* keycls, const ebmlElementClass* valcls);

        ebmlPairClass(ebmlID_t, const std::wstring&, const ebmlElementClass* keycls, const ebmlElementClass* valcls);
        ebmlPairClass(ebmlID_t, const std::wstring&, const ebmlDataElementClass<T>* keycls, const ebmlElementClass* valcls);

        ebmlElement_sp operator()(const ebmlElement_sp&, const ebmlElement_sp&) const;
        ebmlElement_sp operator()(ebmlElement_sp&&, ebmlElement_sp&&) const;
        ebmlElement_sp operator()(const T&, const ebmlElement_sp&) const;
        ebmlElement_sp operator()(const T&, ebmlElement_sp&&) const;
        ebmlElement_sp operator()(T&&, ebmlElement_sp&&) const;
        const ebmlDataElementClass<T>* keycls() const;
        const ebmlElementClass* valcls() const;

    protected:
        ebmlElement* _new() const;
        childClassSpec_t _valueSpec;

    private:
        const ebmlDataElementClass<T>* _keycls;
        const ebmlElementClass* _valcls;
        friend class ebmlPair<T>;
    };

    template<typename K, typename H = std::hash<K>, typename E = std::equal_to<K>, typename A = std::allocator<std::pair<const K, ebmlElement_sp>>>
    class ebmlMap;

    template<typename T>
    class ebmlPair : public ebmlMasterElement {
    protected:
        ebmlElement_sp _key;
        ebmlElement_sp _value;

    private:
        void _validate(const ebmlElement_sp& key, const ebmlElement_sp& value);
        void _validate(const ebmlElement_sp& value);

    protected:
        ebmlPair(const ebmlPairClass<T>*);
        // ebmlPair(const ebmlPairClass<T>*, const ebmlElement_sp&, const ebmlElement_sp&, ebmlElement_sp&, std::exception_ptr&);
        // ebmlPair(const ebmlPairClass<T>*, ebmlElement_sp&&, ebmlElement_sp&&, ebmlElement_sp&, std::exception_ptr&);
        // ebmlPair(const ebmlPairClass<T>*, const T&, const ebmlElement_sp&, ebmlElement_sp&, std::exception_ptr&);
        // ebmlPair(const ebmlPairClass<T>*, const T&, ebmlElement_sp&&, ebmlElement_sp&, std::exception_ptr&);
        // ebmlPair(const ebmlPairClass<T>*, T&&, ebmlElement_sp&&, ebmlElement_sp&, std::exception_ptr&);
        void _clear() override;
        void _setData(const ebmlElement_sp&, const ebmlElement_sp&);
        void _setData(ebmlElement_sp&&, ebmlElement_sp&&);
        void _setData(const T&, const ebmlElement_sp&);
        void _setData(const T&, ebmlElement_sp&&);
        void _setData(T&&, ebmlElement_sp&&);

        // void _validateKey(const ebmlElement_sp&);
        // void _validateValue(const ebmlElement_sp&);

        void _setKey(const ebmlElement_sp&);
        void _setKey(ebmlElement_sp&&);
        void _setKey(const T&);
        void _setKey(T&&);

        void _setValue(const ebmlElement_sp&);
        void _setValue(ebmlElement_sp&&);

    public:
        std::wstring minirepr() const override;
        const ebmlPairClass<T>* cls() const;

        // Element access
        T& key();
        const ebmlElement_sp& keyElem();

        childSlot_t value();

        // Const element access
        const T& key() const;
#if RAW
        const ebmlElement* keyElem() const;
        const ebmlElement* value() const;
#else
        c_ebmlElement_sp keyElem() const;
        c_ebmlElement_sp value() const;
#endif

        // Iteration
    protected:
        class _iterator : public ebmlMasterElement::_iterator {
        private:
            ebmlPair_sp<T> _elem;
            unsigned char _pos;

        protected:
            _iterator(const ebmlPair_sp<T>& elem, unsigned char pos);
            _iterator(ebmlPair_sp<T>&& elem, unsigned char pos);

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy() const;

            const ebmlElement_sp& operator*() const;
            ebmlMasterElement::_iterator& operator++();
            ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
            bool operator==(const ebmlMasterElement::_iterator&) const;
            bool operator!=(const ebmlMasterElement::_iterator&) const;
            friend class ebmlPair<T>;
            friend class ebmlMasterElement::iterator;
        };

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;


        class _const_iterator : public ebmlMasterElement::_const_iterator {
        private:
            std::shared_ptr<const ebmlPair<T>> _elem;
            unsigned char _pos;

        protected:
            _const_iterator(const c_ebmlPair_sp<T>& elem, unsigned char pos);
            _const_iterator(c_ebmlPair_sp<T>&& elem, unsigned char pos);

        public:
            _const_iterator();
            virtual ~_const_iterator();

            ebmlMasterElement::_const_iterator* copy() const;
#if RAW
            const ebmlElement* operator*() const;
#else
            c_ebmlElement_sp operator*() const;
#endif
            ebmlMasterElement::_const_iterator& operator++();
            ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
            bool operator==(const ebmlMasterElement::_const_iterator&) const;
            bool operator!=(const ebmlMasterElement::_const_iterator&) const;

            friend class ebmlPair<T>;
            friend class ebmlMasterElement::const_iterator;
        };

        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;

        friend class ebmlPairClass<T>;

        template<typename S, typename U, typename... Args>
        friend std::shared_ptr<U> new_sp(Args... args);

        template<typename K, typename H, typename E, typename A>
        friend class ebmlMap;
    };

    template<typename K, typename H = std::hash<K>, typename E = std::equal_to<K>, typename A = std::allocator<std::pair<const K, ebmlElement_sp>>>
    class ebmlMapClass : public ebmlMasterElementClass {
    public:
        ebmlMapClass(const char*, const std::wstring&, const ebmlElementClass* paircls);
        ebmlMapClass(const char*, const std::wstring&, const ebmlPairClass<const K>* paircls);

        ebmlMapClass(ebmlID_t, const std::wstring&, const ebmlElementClass* paircls);
        ebmlMapClass(ebmlID_t, const std::wstring&, const ebmlPairClass<const K>* paircls);

        ebmlElement_sp operator()() const;

        // Create element from initializer_list.
        ebmlElement_sp operator()(std::initializer_list<ebmlElement_sp>) const;
        ebmlElement_sp operator()(std::initializer_list<std::pair<ebmlElement_sp, ebmlElement_sp>>) const;
        ebmlElement_sp operator()(std::initializer_list<std::pair<K, ebmlElement_sp>>) const;

        ebmlElement_sp operator()(const std::list<ebmlElement_sp>&) const;
        ebmlElement_sp operator()(const std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&) const;
        ebmlElement_sp operator()(const std::list<std::pair<K, ebmlElement_sp>>&) const;

        ebmlElement_sp operator()(std::list<ebmlElement_sp>&&) const;
        ebmlElement_sp operator()(std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&&) const;
        ebmlElement_sp operator()(std::list<std::pair<K, ebmlElement_sp>>&&) const;

        // TODO:
        // ebmlElement_sp operator()(const std::vector<ebmlElement_sp>&) const;
        // ebmlElement_sp operator()(const std::vector<std::pair<ebmlElement_sp, ebmlElement_sp>>&) const;
        // ebmlElement_sp operator()(const std::vector<std::pair<K, ebmlElement_sp>>&) const;
        //
        // ebmlElement_sp operator()(std::vector<ebmlElement_sp>&&) const;
        // ebmlElement_sp operator()(std::vector<std::pair<ebmlElement_sp, ebmlElement_sp>>&&) const;
        // ebmlElement_sp operator()(std::vector<std::pair<K, ebmlElement_sp>>&&) const;

        ebmlElement_sp operator()(const std::unordered_map<K, ebmlElement_sp, H, E, A>&) const;
        ebmlElement_sp operator()(std::unordered_map<K, ebmlElement_sp, H, E, A>&&) const;

        const ebmlPairClass<const K>* paircls() const;

    protected:
        ebmlElement* _new() const;

    private:
        const ebmlPairClass<const K>* _paircls;
        friend class ebmlMap<K, H, E, A>;
    };

    template<typename K, typename H, typename E, typename A>
    class ebmlMap : public ebmlMasterElement {
    private:
        std::unordered_map<K, ebmlElement_sp, H, E, A> _data;

    protected:
        ebmlMap(const ebmlMapClass<K, H, E, A>*);

        void _clear() override;

        void _validateData(const std::list<ebmlElement_sp>&);
        std::list<ebmlElement_sp> _prepareData(const std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&);
        std::list<ebmlElement_sp> _prepareData(const std::list<std::pair<K, ebmlElement_sp>>&);

        std::list<ebmlElement_sp> _prepareData(std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&&);
        std::list<ebmlElement_sp> _prepareData(std::list<std::pair<K, ebmlElement_sp>>&&);

        std::unordered_map<K, ebmlElement_sp, H, E, A> _prepareData(const std::unordered_map<K, ebmlElement_sp, H, E, A>&);
        std::unordered_map<K, ebmlElement_sp, H, E, A> _prepareData(std::unordered_map<K, ebmlElement_sp, H, E, A>&&);

        void _setData(const std::list<ebmlElement_sp>&);
        void _setData(std::list<ebmlElement_sp>&&);

        // TODO: Support vector

        // Initialize from unordered_map
        void _setData(const std::unordered_map<K, ebmlElement_sp, H, E, A>&);
        void _setData(std::unordered_map<K, ebmlElement_sp, H, E, A>&&);

    public:
        void setData(std::initializer_list<ebmlElement_sp>);
        void setData(std::initializer_list<std::pair<ebmlElement_sp, ebmlElement_sp>>);
        void setData(std::initializer_list<std::pair<K, ebmlElement_sp>>);

        void setData(const std::list<ebmlElement_sp>&);
        void setData(const std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&);
        void setData(const std::list<std::pair<K, ebmlElement_sp>>&);

        void setData(std::list<ebmlElement_sp>&&);
        void setData(std::list<std::pair<ebmlElement_sp, ebmlElement_sp>>&&);
        void setData(std::list<std::pair<K, ebmlElement_sp>>&&);

        // Initialize from unordered_map
        void setData(const std::unordered_map<K, ebmlElement_sp, H, E, A>&);
        void setData(std::unordered_map<K, ebmlElement_sp, H, E, A>&&);

        std::wstring minirepr() const override;
        const ebmlMapClass<K, H, E, A>* cls() const override;

        // Element access
        childSlot_t operator[](const K&);
        childSlot_t at(const K&);
        // childSlot_t at(const K&);

        // For inserting ebmlPair<K>
        std::pair<ebmlMasterElement::iterator, bool> insert(const ebmlElement_sp& value);
        std::pair<ebmlMasterElement::iterator, bool> insert(ebmlElement_sp&& value);

        // For inserting {ebmlDataElement<K>, ebmlElement}
        std::pair<ebmlMasterElement::iterator, bool> insert(const std::pair<ebmlElement_sp, ebmlElement_sp>& value);
        std::pair<ebmlMasterElement::iterator, bool> insert(std::pair<ebmlElement_sp, ebmlElement_sp>&& value);

        // For inserting {Key, ebmlElement}
        std::pair<ebmlMasterElement::iterator, bool> insert(const std::pair<K, ebmlElement_sp>& value);
        std::pair<ebmlMasterElement::iterator, bool> insert(std::pair<K, ebmlElement_sp>&& value);

        // Const element access
#if RAW
        const ebmlElement* at(const K&) const;
        const ebmlElement* keyAt(const K&) const;
#else
        c_ebmlElement_sp at(const K&) const;
        c_ebmlElement_sp keyAt(const K&) const;
#endif

        // Iteration
    protected:
        class _iterator : public ebmlMasterElement::_iterator {
        private:
            ebmlElement_sp _elem;
            typename std::unordered_map<K, ebmlElement_sp, H, E, A>::iterator _iter;

        protected:
            _iterator(const ebmlElement_sp& elem, const typename std::unordered_map<K, ebmlElement_sp, H, E, A>::iterator& iter);
            _iterator(ebmlElement_sp&& elem, typename std::unordered_map<K, ebmlElement_sp, H, E, A>::iterator&& iter);

        public:
            _iterator();
            virtual ~_iterator();

            ebmlMasterElement::_iterator* copy() const;

            const ebmlElement_sp& operator*() const;
            ebmlMasterElement::_iterator& operator++();
            ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
            bool operator==(const ebmlMasterElement::_iterator&) const;
            bool operator!=(const ebmlMasterElement::_iterator&) const;
            friend class ebmlMap<K, H, E, A>;
            friend class ebmlMasterElement::iterator;
        };

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;

        class _const_iterator : public ebmlMasterElement::_const_iterator {
        private:
            c_ebmlElement_sp _elem;
            typename std::unordered_map<K, ebmlElement_sp, H, E, A>::const_iterator _iter;

        protected:
            _const_iterator(const c_ebmlElement_sp& elem, const typename std::unordered_map<K, ebmlElement_sp, H, E, A>::const_iterator& iter);
            _const_iterator(c_ebmlElement_sp&& elem, typename std::unordered_map<K, ebmlElement_sp, H, E, A>::const_iterator&& iter);

        public:
            _const_iterator();
            virtual ~_const_iterator();

            ebmlMasterElement::_const_iterator* copy() const;
#if RAW
            const ebmlElement* operator*() const;
#else
            c_ebmlElement_sp operator*() const;
#endif
            ebmlMasterElement::_const_iterator& operator++();
            ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
            bool operator==(const ebmlMasterElement::_const_iterator&) const;
            bool operator!=(const ebmlMasterElement::_const_iterator&) const;

            friend class ebmlMap<K, H, E, A>;
            friend class ebmlMasterElement::const_iterator;
        };

        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;

        friend class ebmlMapClass<K, H, E, A>;

        template<typename T, typename U, typename... Args>
        friend std::shared_ptr<U> new_sp(Args... args);
    };
}
#endif
