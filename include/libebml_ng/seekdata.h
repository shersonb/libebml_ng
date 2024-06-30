#ifndef EBML_NG_SEEKDATA_H
#define EBML_NG_SEEKDATA_H

#include "ptrs.h"
#include "ebmlElement.h"
// #include "masterelement/ebmlMasterElement.h"
#include "ebmlID_t.h"
#include "parsing/io.h"
#include <sys/types.h>
#include "struct/datetime.h"

#include <unordered_map>
#define SEEKMAPDEFAULTS typename K, typename H=std::hash<K>, typename E=std::equal_to<K>, typename A=std::allocator<std::pair<const K, seekDataWithKey_t<K>*>>
namespace ebml {
    class seekMapBase;
    class seekData_t;

    // template<typename K, typename H=std::hash<K>, typename E=std::equal_to<K>>
    // class seekDataWithKey_t;

    template<typename K>
    class seekDataWithKey_t;

    template<SEEKMAPDEFAULTS>
    class seekMap;

    class seekData_t {
    public:
        ebmlID_t ebmlID;
        vintWidth_t ebmlIDWidth;
        off_t offsetInParent;
        size_t dataSize;
        vintWidth_t sizeWidth;
        ebmlElement_fp ref;
        ebmlMasterElement_fp parent;

        seekData_t(const parseString&);
        seekData_t(const parseFile&);
        seekData_t(ebmlElement&);
        seekData_t(const ebmlElement&);
        seekData_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t);
        seekData_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, ebmlElement_wp&&);
        seekData_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, ebmlElement_wp&&, ebmlMasterElement_wp&&);
        seekData_t(seekData_t&&) = default;
        seekData_t& operator=(seekData_t&&) = default;

        vintWidth_t headSize() const;
        size_t outerSize() const;
        off_t endOffset() const;

        ebmlElement_sp get();
        c_ebmlElement_sp cget();

        // template<typename K, typename H=std::hash<K>, typename E=std::equal_to<K>>
        template<typename K>
        seekDataWithKey_t<K>& withKeyType();

        // template<typename K, typename H=std::hash<K>, typename E=std::equal_to<K>>
        template<typename K>
        const seekDataWithKey_t<K>& withKeyType() const;

        // template<typename K, typename H=std::hash<K>, typename E=std::equal_to<K>>
        template<typename K>
        K& key();

        // template<typename K, typename H=std::hash<K>, typename E=std::equal_to<K>>
        template<typename K>
        const K& key() const;

        virtual std::wstring repr() const;

        // virtual void add(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&);
        // virtual void rm(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&);
        virtual ~seekData_t();
    };

    // template<typename K, typename H, typename E>
    template<typename K>
    class seekDataWithKey_t : public seekData_t {
    public:
        K key;
        // void add(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&);
        // void rm(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&);

        seekDataWithKey_t(const parseString&);
        seekDataWithKey_t(const parseString&, const K&);
        seekDataWithKey_t(const parseString&, K&&);
        seekDataWithKey_t(const parseFile&);
        seekDataWithKey_t(const parseFile&, const K&);
        seekDataWithKey_t(const parseFile&, K&&);
        seekDataWithKey_t(ebmlElement&);
        seekDataWithKey_t(ebmlElement&, const K&);
        seekDataWithKey_t(ebmlElement&, K&&);
        seekDataWithKey_t(const ebmlElement&);
        seekDataWithKey_t(const ebmlElement&, const K&);
        seekDataWithKey_t(const ebmlElement&, K&&);
        seekDataWithKey_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t);
        seekDataWithKey_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, const K&);
        seekDataWithKey_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, K&&);
        seekDataWithKey_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, const K&, const ebmlElement_sp&);
        seekDataWithKey_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, const K&, const ebmlElement_sp&, const ebmlElement_sp&);
        seekDataWithKey_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, K&&, const ebmlElement_sp&);
        seekDataWithKey_t(ebmlID_t, vintWidth_t, off_t, size_t, vintWidth_t, K&&, const ebmlElement_sp&, const ebmlElement_sp&);

        std::wstring repr() const;
    };

    class seekMapBase {
    public:
        seekMapBase();
        virtual ~seekMapBase();
        virtual std::wstring repr() const;

        // template<typename K, typename H=std::hash<K>, typename E=std::equal_to<K>>
        template<SEEKMAPDEFAULTS>
        seekMap<K, H, E, A>& withKeyType();

        template<SEEKMAPDEFAULTS>
        const seekMap<K, H, E, A>& withKeyType() const;

        template<SEEKMAPDEFAULTS>
        seekDataWithKey_t<K>*& operator[](const K&);

        template<SEEKMAPDEFAULTS>
        seekDataWithKey_t<K>*& at(const K&);

        template<SEEKMAPDEFAULTS>
        seekDataWithKey_t<K>* const& at(const K&) const;

        template<SEEKMAPDEFAULTS>
        size_t count(const K&) const;

        template<SEEKMAPDEFAULTS>
        size_t erase(const K&);

        virtual void add(seekData_t*) = 0;
        virtual void rm(seekData_t*) = 0;
    };

    template<typename K, typename H, typename E, typename A>
    class seekMap : public seekMapBase {
    protected:
        // std::unordered_map<K, seekDataWithKey_t<K, H, E>*, H, E> _items;
        std::unordered_map<K, seekDataWithKey_t<K>*, H, E, A> _items;

    public:
        seekMap();
        // ~seekMap();
        seekDataWithKey_t<K>*& operator[](const K&);
        seekDataWithKey_t<K>*& at(const K&);
        seekDataWithKey_t<K>* const& at(const K&) const;
        void insert(const std::pair<K, seekDataWithKey_t<K>*>&);
        void insert(std::pair<K, seekDataWithKey_t<K>*>&&);
        size_t count(const K&) const;
        size_t erase(const K&);
        std::wstring repr() const;

        void add(seekData_t*) override;
        void rm(seekData_t*) override;
    };

    class seekHelper_t {
    public:
        seekHelper_t();
        seekMapBase* initSeekMap() const;
        virtual seekData_t* makeSeekData(const parseString& parsed) const;
        virtual seekData_t* makeSeekData(const parseFile& parsed) const;
        virtual seekData_t* makeSeekData(ebmlElement& elem) const;
        virtual seekData_t* makeSeekData(const ebmlElement& elem) const;
    };

    extern seekHelper_t seekHelper;

    template<SEEKMAPDEFAULTS>
    class seekHelperByEBMLID : public seekHelper_t {
    public:
        ebmlID_t ebmlID;
        seekHelperByEBMLID(ebmlID_t ebmlID);
        seekMap<K, H, E, A>* initSeekMap() const;
        seekData_t* makeSeekData(const parseString& parsed) const;
        seekData_t* makeSeekData(const parseFile& parsed) const;
        seekData_t* makeSeekData(ebmlElement& elem) const = 0;
        seekData_t* makeSeekData(const ebmlElement& elem) const = 0;
    };

    extern template seekMap<unsigned long long>& seekMapBase::withKeyType();
    // extern template const seekMap<unsigned long long>& seekMapBase::withKeyType() const;
    //
    extern template seekMap<long long>& seekMapBase::withKeyType();
    // extern template const seekMap<long long>& seekMapBase::withKeyType() const;
    //
    extern template seekMap<double>& seekMapBase::withKeyType();
    // extern template const seekMap<double>& seekMapBase::withKeyType() const;
    //
    extern template seekMap<std::string>& seekMapBase::withKeyType();
    // extern template const seekMap<std::string>& seekMapBase::withKeyType() const;
    //
    extern template seekMap<std::wstring>& seekMapBase::withKeyType();
    // extern template const seekMap<std::wstring>& seekMapBase::withKeyType() const;
    //
    // extern template class seekMap<unsigned long long>;
    // extern template class seekMap<long long>;
    // extern template class seekMap<double>;
    // extern template class seekMap<std::string>;
    // extern template class seekMap<std::wstring>;
    //
    // extern template class seekHelperByEBMLID<unsigned long long>;
    // extern template class seekHelperByEBMLID<long long>;
    // extern template class seekHelperByEBMLID<double>;
    // extern template class seekHelperByEBMLID<std::string>;
    // extern template class seekHelperByEBMLID<std::wstring>;
    // extern template class seekHelperByEBMLID<timepoint_t>;
}

#endif
