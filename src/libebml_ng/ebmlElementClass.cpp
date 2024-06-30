#ifndef EBML_NG_ELEMENTCLS_CPP
#define EBML_NG_ELEMENTCLS_CPP

// #include <memory>
// #include <stdio.h>
#include <sstream>
#include <iomanip>

#include "libebml_ng/forwards.h"
#include "libebml_ng/ebmlElementClass.tpp"
#include "libebml_ng/ebmlElement.h"
#include "libebml_ng/struct.h"
#include "libebml_ng/vint.h"
#include "libebml_ng/masterelement/base/ebmlMasterElement.h"
// #include "libebml_ng/masterelement/list.h"
// #include "libebml_ng/masterelement/lazyload.h"
// #include "libebml_ng/masterelement/multislot.h"

#include "libebml_ng/seekdata.h"

#include "libebml_ng/struct/binary.h"
#include "libebml_ng/repr.h"

namespace ebml {
    // ebmlElement_sp ebmlElementClass::operator()() const {
    //     return ebmlElement_sp(this->_new());
    // }
    //
    // ebmlElementClass::ebmlElementClass(const char* _ebmlID, const std::wstring& _name) : ebmlElementClass(unpackVint(_ebmlID), _name) {}
    //
    // ebmlElementClass::ebmlElementClass(const char* _ebmlID, const std::wstring& _name, const seekHelper_t* seekHelper)
    // : ebmlElementClass(unpackVint(_ebmlID), _name, seekHelper) {}
    //
    // ebmlElementClass::ebmlElementClass(ebmlID_t _ebmlID, const std::wstring& _name) : ebmlID(_ebmlID), name(_name) {}
    // ebmlElementClass::ebmlElementClass(ebmlID_t _ebmlID, const std::wstring& _name, const seekHelper_t* seekHelper)
    // : ebmlID(_ebmlID), name(_name), _seekHelper(seekHelper) {}
    //
    // ebmlElement* ebmlElementClass::_decode(const parseString& parsed) const {
    //     if (parsed.ebmlID != ebmlID) {
    //         throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
    //     }
    //
    //     ebmlElement* elem;
    //
    //     try {
    //         elem = _decode_nocheck(parsed);
    //
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls == nullptr) {
    //             e.cls = this;
    //             e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //
    //         throw;
    //     }
    //
    //     return elem;
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const parseString& parsed) const {
    //     return _decode(parsed)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(const parseFile& parsed) const {
    //     if (parsed.ebmlID != this->ebmlID) {
    //         throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
    //     }
    //
    //     return _decode_nocheck(parsed);
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const parseFile& parsed) const {
    //     return _decode(parsed)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(const char* data, size_t size) const {
    //     auto parsed = parseString(data, size);
    //
    //     if (size > parsed.outerSize()) {
    //         throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
    //     }
    //
    //     return _decode(parsed);
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const char* data, size_t size) const {
    //     return _decode(data, size)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(const std::string& data) const {
    //     return _decode(data.data(), data.size());
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const std::string& data) const {
    //     return _decode(data)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(const ioBase_sp& file) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&): nullptr detected.");
    //     }
    //
    //     return _decode(*file);
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const ioBase_sp& file) const {
    //     return _decode(file)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(ioBase& file) const {
    //     auto parsed = parseFile(file);
    //     // auto elem = this->decode(parsed);
    //     auto elem = _decode(parsed);
    //     file.seek(parsed.endOffset());
    //     return elem;
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(ioBase& file) const {
    //     return _decode(file)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(const ioBase_sp& file, off_t& offset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _decode(*file, offset);
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const ioBase_sp& file, off_t& offset) const {
    //     return _decode(file, offset)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(ioBase& file, off_t& offset) const {
    //     auto parsed = parseFile(file, offset);
    //     auto elem = _decode(parsed);
    //     offset = parsed.endOffset();
    //     return elem;
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(ioBase& file, off_t& offset) const {
    //     return _decode(file, offset)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _decode(*file, offset, endOffset);
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     return _decode(file, offset, endOffset)->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_decode(ioBase& file, off_t offset, off_t& endOffset) const {
    //     auto parsed = parseFile(file, offset);
    //     auto elem = _decode(parsed);
    //     endOffset = parsed.endOffset();
    //     return elem;
    // }
    //
    // ebml::ptr<ebmlElement> ebmlElementClass::decode(ioBase& file, off_t offset, off_t& endOffset) const {
    //     return _decode(file, offset, endOffset)->sp();
    // }
    //
    // // CONST DECODING
    //
    // ebmlElement* ebmlElementClass::_cdecode(const parseString& parsed) const {
    //     if (parsed.ebmlID != ebmlID) {
    //         throw ebmlNoMatch("EBML ID does not match.", this, 0, 0, 0);
    //     }
    //
    //     ebmlElement* elem;
    //
    //     try {
    //         elem = _cdecode_nocheck(parsed);
    //
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls == nullptr) {
    //             e.cls = this;
    //             e.headSize = parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //
    //         throw;
    //     }
    //
    //     return elem;
    // }
    //
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const parseString& parsed) const {
    //     const auto elem = _cdecode(parsed);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(const parseFile& parsed) const {
    //     if (parsed.ebmlID != this->ebmlID) {
    //         throw ebmlNoMatch("EBML ID does not match.", this, parsed.offset);
    //     }
    //
    //     return _cdecode_nocheck(parsed);
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const parseFile& parsed) const {
    //     const auto elem = _cdecode(parsed);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(const char* data, size_t size) const {
    //     auto parsed = parseString(data, size);
    //
    //     if (size > parsed.outerSize()) {
    //         throw ebmlDataContinues("Data continues past expected end.", this, 0, parsed.ebmlIDWidth + parsed.sizeWidth, parsed.dataSize);
    //     }
    //
    //     return _cdecode(parsed);
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const char* data, size_t size) const {
    //     const auto elem = _cdecode(data, size);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(const std::string& data) const {
    //     return _cdecode(data.data(), data.size());
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const std::string& data) const {
    //     const auto elem = _cdecode(data);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(const ioBase_sp& file) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file);
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const ioBase_sp& file) const {
    //     const auto elem = _cdecode(file);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(ioBase& file) const {
    //     auto parsed = parseFile(file);
    //     // auto elem = this->decode(parsed);
    //     auto elem = _cdecode(parsed);
    //     file.seek(parsed.endOffset());
    //     return elem;
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(ioBase& file) const {
    //     const auto elem = _cdecode(file);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(const ioBase_sp& file, off_t& offset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file, offset);
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const ioBase_sp& file, off_t& offset) const {
    //     const auto elem = _cdecode(file, offset);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(ioBase& file, off_t& offset) const {
    //     auto parsed = parseFile(file, offset);
    //     auto elem = _cdecode(parsed);
    //     offset = parsed.endOffset();
    //     return elem;
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(ioBase& file, off_t& offset) const {
    //     const auto elem = _cdecode(file, offset);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     if (file == nullptr) {
    //         throw std::runtime_error("ebmlElementClass::decode(const ioBase_sp&, off_t&): nullptr detected.");
    //     }
    //
    //     return _cdecode(*file, offset, endOffset);
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(const ioBase_sp& file, off_t offset, off_t& endOffset) const {
    //     const auto elem = _cdecode(file, offset, endOffset);
    //     return elem->sp();
    // }
    //
    // ebmlElement* ebmlElementClass::_cdecode(ioBase& file, off_t offset, off_t& endOffset) const {
    //     auto parsed = parseFile(file, offset);
    //     auto elem = _cdecode(parsed);
    //     endOffset = parsed.endOffset();
    //     return elem;
    // }
    //
    // ebml::ptr<const ebmlElement> ebmlElementClass::cdecode(ioBase& file, off_t offset, off_t& endOffset) const {
    //     const auto elem = _cdecode(file, offset, endOffset);
    //     return elem->sp();
    // }

    // MAKE SEEK DATA

    seekMapBase* ebmlElementClass::initSeekMap() const {
        return _seekHelper->initSeekMap();
    }

    seekData_t* ebmlElementClass::makeSeekData(const parseString& parsed) const {
        return _seekHelper->makeSeekData(parsed);
    }

    seekData_t* ebmlElementClass::makeSeekData(const parseString& parsed, const ebmlMasterElement_wp& parent) const {
        auto data = makeSeekData(parsed);
        data->parent = parent;
        return data;
    }

    seekData_t* ebmlElementClass::makeSeekData(const parseString& parsed, ebmlMasterElement_wp&& parent) const {
        auto data = makeSeekData(parsed);
        data->parent = std::move(parent);
        return data;
    }

    seekData_t* ebmlElementClass::makeSeekData(const parseFile& parsed) const {
        return _seekHelper->makeSeekData(parsed);
    }

    seekData_t* ebmlElementClass::makeSeekData(const parseFile& parsed, const ebmlMasterElement_wp& parent) const {
        auto data = makeSeekData(parsed);
        data->parent = parent;
        return data;
    }

    seekData_t* ebmlElementClass::makeSeekData(const parseFile& parsed, ebmlMasterElement_wp&& parent) const {
        auto data = makeSeekData(parsed);
        data->parent = std::move(parent);
        return data;
    }

    std::wstring ebmlElementClass::repr() const {
        std::wstringstream ss;

        char ebmlID_chars[8];
        unsigned char ebmlIDWidth;

        ebmlIDWidth = packVint(this->ebmlID, ebmlID_chars);

        ss << L"<EBML Element type: ";
        ss << this->name;

        ss << L", ebmlID=0x";

        for (unsigned char k = 0; k < ebmlIDWidth; k++) {
            ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<unsigned char>(ebmlID_chars[k]);
        }

        ss << L">";
        return ss.str();
    }

    std::wstring ebmlElementClass::minirepr() const {
        return std::wstring();
    }
}

#endif
