#ifndef EBML_NG_SEEKDATA_CPP
#define EBML_NG_SEEKDATA_CPP

#include "libebml_ng/seekdata.tpp"

namespace ebml {
    seekData_t::seekData_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize, nullptr, nullptr) {}

    seekData_t::seekData_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, const ebmlElement_sp& ref)
    : seekData_t(ebmlID, offset, sizeWidth, dataSize, ref, nullptr) {}

    seekData_t::seekData_t(ebmlID_t ebmlID, off_t offset, vintWidth_t sizeWidth, size_t dataSize, const ebmlElement_sp& ref, const ebmlElement_sp& parent)
    : ebmlID(ebmlID), offsetInParent(offset), sizeWidth(sizeWidth), dataSize(dataSize), ref(ref), parent(parent) {}

    // seekData_t::seekData_t(const seekData_t&);
    // seekData_t::seekData_t(seekData_t&&);
    // seekData_t& seekData_t::operator=(const seekData_t&);
    // seekData_t& seekData_t::operator=(seekData_t&&);

    // Do nothing by default
    void seekData_t::add(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&) {}
    void seekData_t::rm(std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>&) {}

    template class seekMap<unsigned long long>;
}

#endif
