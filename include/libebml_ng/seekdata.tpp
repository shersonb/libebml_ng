#ifndef EBML_NG_SEEKDATA_TPP
#define EBML_NG_SEEKDATA_TPP

#include "seekdata.h"
// #include "masterelement/lazyload.h"
#include "repr.h"

namespace ebml {
    template<typename K>
    seekDataWithKey_t<K>& seekData_t::withKeyType() {
        if (auto recast = std::dynamic_pointer_cast<seekDataWithKey_t<K>*>(this)) {
            return *recast;
        }
        throw std::bad_cast();
    }

    template<typename K>
    const seekDataWithKey_t<K>& seekData_t::withKeyType() const {
        if (auto recast = std::dynamic_pointer_cast<const seekDataWithKey_t<K>*>(this)) {
            return *recast;
        }
        throw std::bad_cast();
    }

    template<typename K>
    K& seekData_t::key() {
        return this->withKeyType<K>().key;
    }

    template<typename K>
    const K& seekData_t::key() const {
        return this->withKeyType<K>().key;
    }


    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const parseString& parsed)
    : seekDataWithKey_t<K>(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset, parsed.dataSize, parsed.sizeWidth) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const parseString& parsed, const K& key)
    // : seekDataWithKey_t<K>(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset, parsed.dataSize, parsed.sizeWidth, key) {}
    : seekData_t(parsed), key(key) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const parseString& parsed, K&& key)
    // : seekDataWithKey_t<K>(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset, parsed.dataSize, parsed.sizeWidth, std::move(key)) {}
    : seekData_t(parsed), key(std::move(key)) {}


    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const parseFile& parsed)
    : seekDataWithKey_t<K>(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset - parsed.parentOffset, parsed.dataSize, parsed.sizeWidth) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const parseFile& parsed, const K& key)
    : seekDataWithKey_t<K>(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset - parsed.parentOffset, parsed.dataSize, parsed.sizeWidth, key) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const parseFile& parsed, K&& key)
    : seekDataWithKey_t<K>(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset - parsed.parentOffset, parsed.dataSize, parsed.sizeWidth, std::move(key)) {}


    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlElement& elem)
    : seekData_t(elem) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlElement& elem, const K& key)
    : seekData_t(elem), key(key) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlElement& elem, K&& key)
    : seekData_t(elem), key(std::move(key)) {}


    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const ebmlElement& elem)
    : seekData_t(elem) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const ebmlElement& elem, const K& key)
    : seekData_t(elem), key(key) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(const ebmlElement& elem, K&& key)
    : seekData_t(elem), key(std::move(key)) {}


    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth)
    : seekData_t(ebmlID, ebmlIDWidth, offset, dataSize, sizeWidth) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, const K& key)
    : seekData_t(ebmlID, ebmlIDWidth, offset, dataSize, sizeWidth), key(key) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, K&& key)
    : seekData_t(ebmlID, ebmlIDWidth, offset, dataSize, sizeWidth), key(std::move(key)) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, const K& key, const ebmlElement_sp& ref)
    : seekData_t(ebmlID, ebmlIDWidth, offset, dataSize, sizeWidth, ref), key(key) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, const K& key, const ebmlElement_sp& ref, const ebmlElement_sp& parent)
    : seekData_t(ebmlID, ebmlIDWidth, offset, dataSize, sizeWidth, ref, parent), key(key) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, K&& key, const ebmlElement_sp& ref)
    : seekData_t(ebmlID, ebmlIDWidth, offset, dataSize, sizeWidth, ref), key(std::move(key)) {}

    template<typename K>
    seekDataWithKey_t<K>::seekDataWithKey_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, K&& key, const ebmlElement_sp& ref, const ebmlElement_sp& parent)
    : seekData_t(ebmlID, ebmlIDWidth, offset, dataSize, sizeWidth, ref, parent), key(std::move(key)) {}

    template<typename K>
    std::wstring seekDataWithKey_t<K>::repr() const {
        std::wstring result = L"SeekData(";

        result += L"ebmlID=";
        result += ebml::repr(this->ebmlID);
        result += L", ";

        result += L"ebmlIDWidth=";
        result += ebml::repr(static_cast<unsigned long long>(this->ebmlIDWidth));
        result += L", ";

        result += L"offsetInParent=";
        result += ebml::repr(static_cast<long long>(this->offsetInParent));
        result += L", ";

        result += L"dataSize=";
        result += ebml::repr(static_cast<unsigned long long>(this->dataSize));
        result += L", ";

        result += L"sizeWidth=";
        result += ebml::repr(static_cast<unsigned long long>(this->sizeWidth));
        result += L", ";

        result += L"key=";
        result += ebml::repr(this->key);

        return result + L")";
    }

    // template<typename K, typename H, typename E, typename A>
    // void seekDataWithKey_t<K>::add(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>& map) {
    //     // auto& ptr = map[this->ebmlID];
    //     //
    //     // if (ptr == nullptr) {
    //     //     std::cout << this->key << " Creating new map" << std::endl;
    //     //     auto raw = new seekMap<K, H, E, A>();
    //     //     ptr = std::unique_ptr<seekMapBase>(raw);
    //     //     auto& submap = *raw;
    //     //     submap[this->key] = this;
    //     // } else {
    //     //     std::cout << this->key << " Using existing map" << std::endl;
    //     //     auto& submap = *ptr;
    //     //     submap[this->key] = this;
    //     // }
    //     if (map.count(this->ebmlID)) {
    //         auto& submap = map.at(this->ebmlID);
    //
    //         if (auto recast = dynamic_cast<seekMap<K, H, E, A>*>(submap.get())) {
    //             // (*submap)[this->key] = this;
    //             recast->insert({this->key, this});
    //         // } else {
    //         //     std::cout << "NULL!" << std::endl;
    //         }
    //     } else {
    //         auto submap = new seekMap<K, H, E, A>();
    //         auto submap_sp = std::unique_ptr<seekMapBase>(submap);
    //         (*submap)[this->key] = this;
    //         map.insert({this->ebmlID, std::move(submap_sp)});
    //     }
    // }
    //
    // template<typename K, typename H, typename E, typename A>
    // void seekDataWithKey_t<K>::rm(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>& map) {
    //     if (map.count(this->ebmlID)) {
    //         auto& ptr = map[this->ebmlID];
    //         auto& submap = *ptr;
    //         submap.erase(this->key);
    //     }
    // }



    template<typename K, typename H, typename E, typename A>
    seekMap<K, H, E, A>& seekMapBase::withKeyType() {
        if (auto recast = dynamic_cast<seekMap<K, H, E, A>*>(this)) {
            return *recast;
        }
        throw std::bad_cast();
    }

    template<typename K, typename H, typename E, typename A>
    seekMap<K, H, E, A>::seekMap() {}

    // template<typename K, typename H, typename E, typename A>
    // seekMap<K, H, E, A>::~seekMap() {
    //     // this->_items.clear();
    // }

    template<typename K, typename H, typename E, typename A>
    const seekMap<K, H, E, A>& seekMapBase::withKeyType() const {
        if (auto recast = dynamic_cast<const seekMap<K, H, E, A>*>(this)) {
            return *recast;
        }
        throw std::bad_cast();
    }

    template<typename K, typename H, typename E, typename A>
    size_t seekMapBase::count(const K& t) const{
        return this->withKeyType<K, H, E, A>().count(t);
    }

    template<typename K, typename H, typename E, typename A>
    size_t seekMapBase::erase(const K& t) {
        return this->withKeyType<K, H, E, A>().erase(t);
    }

    template<typename K, typename H, typename E, typename A>
    seekDataWithKey_t<K>*& seekMapBase::operator[](const K& t) {
        return this->withKeyType<K, H, E, A>()[t];
    }

    template<typename K, typename H, typename E, typename A>
    seekDataWithKey_t<K>*& seekMapBase::at(const K& t) {
        return this->withKeyType<K, H, E, A>().at(t);
    }

    template<typename K, typename H, typename E, typename A>
    seekDataWithKey_t<K>* const& seekMapBase::at(const K& t) const {
        return this->withKeyType<K, H, E, A>().at(t);
    }



    template<typename K, typename H, typename E, typename A>
    seekDataWithKey_t<K>*& seekMap<K, H, E, A>::operator[](const K& t) {
        return this->_items[t];
    }

    template<typename K, typename H, typename E, typename A>
    seekDataWithKey_t<K>*& seekMap<K, H, E, A>::at(const K& t) {
        return this->_items.at(t);
    }

    template<typename K, typename H, typename E, typename A>
    seekDataWithKey_t<K>* const& seekMap<K, H, E, A>::at(const K& t) const {
        return this->_items.at(t);
    }

    template<typename K, typename H, typename E, typename A>
    size_t seekMap<K, H, E, A>::count(const K& t) const {
        return this->_items.count(t);
    }

    template<typename K, typename H, typename E, typename A>
    size_t seekMap<K, H, E, A>::erase(const K& t) {
        return this->_items.erase(t);
    }


    template<typename K, typename H, typename E, typename A>
    void seekMap<K, H, E, A>::insert(const std::pair<K, seekDataWithKey_t<K>*>& pair) {
        this->_items.insert(pair);
    }

    template<typename K, typename H, typename E, typename A>
    void seekMap<K, H, E, A>::insert(std::pair<K, seekDataWithKey_t<K>*>&& pair) {
        this->_items.insert(std::move(pair));
    }

    template<typename K, typename H, typename E, typename A>
    std::wstring seekMap<K, H, E, A>::repr() const {
        std::wstring result = L"{";

        if (this->_items.size() == 0) {
            return result + L"}";
        }

        result += L"\n";

        auto iter = this->_items.cbegin();
        auto end = this->_items.cend();

        result += L"    ";
        result += ebml::repr(iter->first);
        result += L": ";
        result += iter->second->repr();

        ++iter;

        while (iter != end) {
            result += L",\n";
            result += L"    ";
            result += ebml::repr(iter->first);
            result += L": ";
            result += iter->second->repr();

            ++iter;
        }

        return result + L"\n}";
    }

    template<typename K, typename H, typename E, typename A>
    void seekMap<K, H, E, A>::add(seekData_t* seekData) {
        if (auto recast = dynamic_cast<seekDataWithKey_t<K>*>(seekData)) {
            _items[recast->key] = recast;
        }
    }

    template<typename K, typename H, typename E, typename A>
    void seekMap<K, H, E, A>::rm(seekData_t* seekData) {
        if (auto recast = dynamic_cast<seekDataWithKey_t<K>*>(seekData)) {
            K& k = recast->key;
            if (_items.count(k)) {
                _items.erase(k);
            }
        }
    }

    template<typename K, typename H, typename E, typename A>
    seekMap<K, H, E, A>* seekHelperByEBMLID<K, H, E, A>::initSeekMap() const {
        return new seekMap<K, H, E, A>();
    }

    template<typename K, typename H, typename E, typename A>
    seekHelperByEBMLID<K, H, E, A>::seekHelperByEBMLID(ebmlID_t ebmlID) : ebmlID(ebmlID) {}

    template<typename K, typename H, typename E, typename A>
    seekData_t* seekHelperByEBMLID<K, H, E, A>::makeSeekData(const parseString& parsed) const {
        auto iter = parsed.begin();

        while (!iter.atEnd()) {
            auto subparsed = *iter;

            if (subparsed.ebmlID == this->ebmlID) {
                K data = subparsed.unpack<K>();
                // std::cout << ebml::pack(ebml::repr(data)) << std::endl;
                // return new seekData_t(parsed);
                // return new seekDataWithKey_t<K>(parsed, subparsed.unpack<K>());
                // return new seekDataWithKey_t<K>(parsed);
                // auto seekData = new seekDataWithKey_t<K>(parsed);
                // return new seekDataWithKey_t<K>(parsed, std::move(data));
                return new seekDataWithKey_t<K>(parsed, data);
                // (*seekData).key = 0;
                // return seekData;
            }

            ++iter;
        }

        return new seekData_t(parsed);
    }

    template<typename K, typename H, typename E, typename A>
    seekData_t* seekHelperByEBMLID<K, H, E, A>::makeSeekData(const parseFile& parsed) const {
        auto iter = parsed.begin();

        while (!iter.atEnd()) {
            auto subparsed = *iter;

            if (subparsed.ebmlID == this->ebmlID) {
                return new seekDataWithKey_t<K>(parsed, subparsed.unpack<K>());
            }

            ++iter;
        }

        return new seekData_t(parsed);
    }

    // template<typename K, typename H, typename E, typename A>
    // seekData_t* seekHelperByEBMLID<K, H, E, A>::makeSeekData(ebmlElement& elem) const {
    //     if (auto recast = dynamic_cast<ebmlLazyLoad*>(&elem)) {
    //         auto& seeksByEBMLID = recast->seeksByEBMLID();
    //
    //         if (seeksByEBMLID.count(this->ebmlID)) {
    //             auto& seeks = seeksByEBMLID.at(this->ebmlID);
    //
    //             if (seeks.size() > 0) {
    //                 auto& pair = *seeks.cbegin();
    //                 auto child = pair.second->cget();
    //
    //                 if (auto recast = ebml_dynamic_pointer_cast<const ebmlDataElement<K>>(std::move(child))) {
    //                     return new seekDataWithKey_t<K>(elem, recast->data);
    //                 }
    //             }
    //         }
    //     }
    //
    //     return new seekData_t(elem);
    // }

    // template<typename K, typename H, typename E, typename A>
    // seekData_t* seekHelperByEBMLID<K, H, E, A>::makeSeekData(const ebmlElement& elem) const {
    //     if (auto recast = dynamic_cast<const ebmlLazyLoad*>(&elem)) {
    //         auto& seeksByEBMLID = recast->seeksByEBMLID();
    //
    //         if (seeksByEBMLID.count(this->ebmlID)) {
    //             auto& seeks = seeksByEBMLID.at(this->ebmlID);
    //
    //             if (seeks.size() > 0) {
    //                 auto& pair = *seeks.cbegin();
    //                 auto child = pair.second->cget();
    //
    //                 if (auto recast = ebml_dynamic_pointer_cast<const ebmlDataElement<K>>(std::move(child))) {
    //                     return new seekDataWithKey_t<K>(elem, recast->data);
    //                 }
    //             }
    //         }
    //     }
    //
    //     return new seekData_t(elem);
    // }
}

#endif
