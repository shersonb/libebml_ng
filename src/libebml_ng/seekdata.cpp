#ifndef EBML_NG_SEEKDATA_CPP
#define EBML_NG_SEEKDATA_CPP

#include "libebml_ng/seekdata.h"
#include "libebml_ng/ebmlElement.h"
#include "libebml_ng/seekdata.tpp"
#include "libebml_ng/masterelement/lazyload/ebmlLazyLoad.h"
#include "libebml_ng/flexptr.h"
#include "libebml_ng/vint.h"

namespace ebml {
    seekData_t::seekData_t(const parseString& parsed)
    : seekData_t(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset, parsed.dataSize, parsed.sizeWidth) {}

    seekData_t::seekData_t(const parseFile& parsed)
    : seekData_t(parsed.ebmlID, parsed.ebmlIDWidth, parsed.offset - parsed.parentOffset, parsed.dataSize, parsed.sizeWidth) {}

    seekData_t::seekData_t(ebmlElement& elem)
    : seekData_t(elem.ebmlID(), widthAsVint(elem.ebmlID()), elem.offsetInParent(), elem.dataSize(), elem.sizeWidth()) {
        ref = wptr(elem.sp());
        parent = wptr(elem.parent());
    }

    // seekData_t::~seekData_t() {
    //     std::cout << "~seekData_t()" << std::endl;
    // }

    seekData_t::seekData_t(const ebmlElement& elem)
    : seekData_t(elem.ebmlID(), widthAsVint(elem.ebmlID()), elem.offsetInParent(), elem.dataSize(), elem.sizeWidth()) {
        auto ptr = elem.sp();
        ref = wptr(std::move(ptr));
        parent = wptr(elem.parent());
    }

    seekData_t::seekData_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth)
    : ebmlID(ebmlID), ebmlIDWidth(ebmlIDWidth), offsetInParent(offset), dataSize(dataSize), sizeWidth(sizeWidth) {}

    seekData_t::seekData_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, ebmlElement_wp&& ref)
    : ebmlID(ebmlID), ebmlIDWidth(ebmlIDWidth), offsetInParent(offset), dataSize(dataSize), sizeWidth(sizeWidth), ref(std::move(ref)) {}

    seekData_t::seekData_t(ebmlID_t ebmlID, vintWidth_t ebmlIDWidth, off_t offset, size_t dataSize, vintWidth_t sizeWidth, ebmlElement_wp&& ref, ebmlMasterElement_wp&& parent)
    : ebmlID(ebmlID), ebmlIDWidth(ebmlIDWidth), offsetInParent(offset), dataSize(dataSize), sizeWidth(sizeWidth), ref(std::move(ref)), parent(std::move(parent)) {}

    vintWidth_t seekData_t::headSize() const {
        return this->ebmlIDWidth + this->sizeWidth;
    }

    size_t seekData_t::outerSize() const {
        return this->ebmlIDWidth + this->sizeWidth + this->dataSize;
    }

    off_t seekData_t::endOffset() const {
        return offsetInParent + outerSize();
    }

    ebml::ptr<ebmlElement> seekData_t::get() {
        auto elem = ref.get();

        if (elem != nullptr) {
            return elem;
        }

        auto parent_sp = parent.get();
        auto& _parent = parent_sp->as<ebmlLazyLoad>();
        return _parent._read_elem(*this);
    }

    ebml::ptr<const ebmlElement> seekData_t::cget() {
        auto elem = ref.cget();

        if (elem != nullptr) {
            return elem;
        }

        auto parent_sp = parent.get();
        auto& _parent = parent_sp->as<const ebmlLazyLoad>();
        return _parent._read_elem(*this);
    }

    std::wstring seekData_t::repr() const {
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

        return result + L")";
    }
    // seekData_t::seekData_t(const seekData_t&);
    // seekData_t::seekData_t(seekData_t&&);
    // seekData_t& seekData_t::operator=(const seekData_t&);
    // seekData_t& seekData_t::operator=(seekData_t&&);

    // Do nothing by default
    // void seekData_t::add(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&) {}
    // void seekData_t::rm(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&) {}
    seekData_t::~seekData_t() {}

    seekHelper_t::seekHelper_t() {}

    seekMapBase* seekHelper_t::initSeekMap() const {
        return nullptr;
    }

    seekData_t* seekHelper_t::makeSeekData(const parseString& parsed) const {
        return new seekData_t(parsed);
    }

    seekData_t* seekHelper_t::makeSeekData(const parseFile& parsed) const {
        return new seekData_t(parsed);
    }

    seekData_t* seekHelper_t::makeSeekData(ebmlElement& elem) const {
        return new seekData_t(elem);
    }

    seekData_t* seekHelper_t::makeSeekData(const ebmlElement& elem) const {
        return new seekData_t(elem);
    }

    seekHelper_t seekHelper;

    seekMapBase::seekMapBase() {}
    seekMapBase::~seekMapBase() {}

    std::wstring seekMapBase::repr() const {
        return L"{}";
    }

    template seekMap<unsigned long long>& seekMapBase::withKeyType();
    template const seekMap<unsigned long long>& seekMapBase::withKeyType() const;

    template seekMap<long long>& seekMapBase::withKeyType();
    template const seekMap<long long>& seekMapBase::withKeyType() const;

    template seekMap<double>& seekMapBase::withKeyType();
    template const seekMap<double>& seekMapBase::withKeyType() const;

    template seekMap<std::string>& seekMapBase::withKeyType();
    template const seekMap<std::string>& seekMapBase::withKeyType() const;

    template seekMap<std::wstring>& seekMapBase::withKeyType();
    template const seekMap<std::wstring>& seekMapBase::withKeyType() const;


    template class seekMap<unsigned long long>;
    template class seekMap<long long>;
    template class seekMap<double>;
    template class seekMap<std::string>;
    template class seekMap<std::wstring>;
    // template class seekMap<timepoint_t>;

    // template class seekHelperByEBMLID<unsigned long long>;
    // template class seekHelperByEBMLID<long long>;
    // template class seekHelperByEBMLID<double>;
    // template class seekHelperByEBMLID<std::string>;
    // template class seekHelperByEBMLID<std::wstring>;
    // template class seekHelperByEBMLID<timepoint_t>;
}

#endif
