#ifndef EBML_NG_MASTERELEMENT_DECODE_CPP
#define EBML_NG_MASTERELEMENT_DECODE_CPP

#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/exceptions.h"
#include "libebml_ng/void.h"
#include "libebml_ng/struct/unicode.h"
#include "libebml_ng/ebmlElementClass.tpp"

namespace ebml {
    // void ebmlMasterElement::_init(const parseString& parsed) {
    //     try {
    //         if (parsed.dataSize > 0) {
    //             parseString::iterator iter = parsed.begin();
    //             _decodeChildren(iter);
    //         }
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls != nullptr) {
    //             e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //         throw;
    //     }
    // }
    //
    // void ebmlMasterElement::_cinit(const parseString& parsed) {
    //     try {
    //         if (parsed.dataSize > 0) {
    //             parseString::iterator iter = parsed.begin();
    //             _cdecodeChildren(iter);
    //         }
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls != nullptr) {
    //             e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //         throw;
    //     }
    // }

    // void ebmlMasterElement::_init(const parseFile& parsed) {
    //     try {
    //         if (parsed.dataSize > 0) {
    //             parseFile::iterator iter = parsed.begin();
    //             _scanChildren(iter);
    //         }
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls != nullptr) {
    //             e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //         throw;
    //     }
    // }
    //
    // void ebmlMasterElement::_cinit(const parseFile& parsed) {
    //     try {
    //         if (parsed.dataSize > 0) {
    //             parseFile::iterator iter = parsed.begin();
    //             _cscanChildren(iter);
    //         }
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls != nullptr) {
    //             e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //         throw;
    //     }
    // }

    // ebmlElement* ebmlMasterElement::_decodeChild(const parseString& parsed) const {
    //     return cls()._decodeChild(parsed);
    // }
    //
    // ebmlElement* ebmlMasterElement::_cdecodeChild(const parseString& parsed) const {
    //     return cls()._cdecodeChild(parsed);
    // }
    //
    // ebmlElement* ebmlMasterElement::_decodeChild(const parseFile& parsed) const {
    //     return cls()._decodeChild(parsed);
    // }
    //
    // ebmlElement* ebmlMasterElement::_cdecodeChild(const parseFile& parsed) const {
    //     return cls()._cdecodeChild(parsed);
    // }

    // ebmlElement_sp ebmlMasterElementClass::_decode(const parseString& parsed) const {
    //     try {
    //         auto elem = this->_new()->ptr<ebmlMasterElement>();
    //         auto elem_sp = ebmlElement_sp(elem);
    //
    //         if (parsed.dataSize > 0) {
    //             parseString::iterator iter = parsed.begin();
    //             elem->_decodeChildren(iter);
    //         }
    //
    //         return elem_sp;
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls != nullptr) {
    //             e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //         throw;
    //     }
    // }
    //
    // ebmlElement_sp ebmlMasterElementClass::_cdecode(const parseString& parsed) const {
    //     try {
    //         auto elem = this->_new()->ptr<ebmlMasterElement>();
    //         auto elem_sp = ebmlElement_sp(elem);
    //
    //         if (parsed.dataSize > 0) {
    //             parseString::iterator iter = parsed.begin();
    //             elem->_cdecodeChildren(iter);
    //         }
    //
    //         return elem_sp;
    //     } catch (ebmlDecodeError& e) {
    //         if (e.cls != nullptr) {
    //             e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
    //         }
    //         throw;
    //     }
    // }

    // ebmlElement* ebmlMasterElementClass::_decodeChild(const parseString& parsed) const {
    //     const ebmlElementClass* childcls;
    //
    //     try {
    //         childcls = this->_childClasses[parsed.ebmlID];
    //     } catch (ebmlNoChildMatch& e) {
    //         e.erroroffset = parsed.offset;
    //         throw;
    //     }
    //
    //     return childcls->_decode_nocheck(parsed);
    // }
    //
    // ebmlElement* ebmlMasterElementClass::_cdecodeChild(const parseString& parsed) const {
    //     const ebmlElementClass* childcls;
    //
    //     try {
    //         childcls = this->_childClasses[parsed.ebmlID];
    //     } catch (ebmlNoChildMatch& e) {
    //         e.erroroffset = parsed.offset;
    //         throw;
    //     }
    //
    //     return childcls->_decode_nocheck(parsed);
    // }

    // From file
    // ebmlElement_sp ebmlMasterElementClass::_decode(const parseFile& parsed) const {
    //     auto elem = this->_new()->ptr<ebmlMasterElement>();
    //     auto elem_sp = ebmlElement_sp(elem);
    //
    //     if (parsed.dataSize > 0) {
    //         parseFile::iterator iter = parsed.begin();
    //         elem->_scanChildren(iter);
    //     }
    //
    //     return elem_sp;
    // }
    //
    // ebmlElement_sp ebmlMasterElementClass::_cdecode(const parseFile& parsed) const {
    //     auto elem = this->_new()->ptr<ebmlMasterElement>();
    //     auto elem_sp = ebmlElement_sp(elem);
    //
    //     if (parsed.dataSize > 0) {
    //         parseFile::iterator iter = parsed.begin();
    //         elem->_cscanChildren(iter);
    //     }
    //
    //     return elem_sp;
    // }

    // ebmlElement* ebmlMasterElementClass::_decodeChild(const parseFile& parsed) const {
    //     const ebmlElementClass* childcls;
    //
    //     try {
    //         childcls = this->_childClasses[parsed.ebmlID];
    //     } catch (ebmlNoChildMatch& e) {
    //         e.erroroffset = parsed.offset;
    //         throw;
    //     }
    //
    //     return childcls->_decode_nocheck(parsed);
    // }
    //
    // ebmlElement* ebmlMasterElementClass::_cdecodeChild(const parseFile& parsed) const {
    //     const ebmlElementClass* childcls;
    //
    //     try {
    //         childcls = this->_childClasses[parsed.ebmlID];
    //     } catch (ebmlNoChildMatch& e) {
    //         e.offset = parsed.offset;
    //         throw;
    //     }
    //
    //     // return childcls->mcdecode(parsed);
    //     return childcls->_decode_nocheck(parsed);
    // }

    // void ebmlMasterElement::_decodeChildren(parseString::iterator& iter) {
    //     parseString parsed;
    //     ebmlElement_sp elem_sp;
    //     ebmlElement* elem;
    //     _clear();
    //     auto& c = cls();
    //
    //     while (!iter.atEnd()) {
    //         parsed = *iter;
    //         ++iter;
    //
    //         if (parsed.ebmlIDWidth == 0) {
    //             break;
    //         }
    //
    //         if (parsed.ebmlID == Void.ebmlID) {
    //             continue;
    //         }
    //
    //         try {
    //             elem = c._decodeChild(parsed);
    //             elem_sp = elem->sp();
    //         } catch (ebmlDecodeError& e) {
    //             if (e.cls != nullptr) {
    //                 e.offset += parsed.offset;
    //             }
    //             throw;
    //         }
    //
    //         this->_attachChild(*elem);
    //         this->_addChild(std::move(elem_sp));
    //     }
    // }
    //
    // void ebmlMasterElement::_cdecodeChildren(parseString::iterator& iter) {
    //     parseString parsed;
    //     ebmlElement* elem;
    //     this->_clear();
    //     auto& c = cls();
    //
    //     while (!iter.atEnd()) {
    //         parsed = *iter;
    //         ++iter;
    //
    //         if (parsed.ebmlIDWidth == 0) {
    //             break;
    //         }
    //
    //         if (parsed.ebmlID == Void.ebmlID) {
    //             continue;
    //         }
    //
    //         try {
    //             elem = c._cdecodeChild(parsed);
    //         } catch (ebmlDecodeError& e) {
    //             if (e.cls != nullptr) {
    //                 e.offset += parsed.offset;
    //             }
    //             throw;
    //         }
    //
    //         auto elem_sp = elem->sp();
    //         this->_attachChild(*elem);
    //         this->_addChild(std::move(elem_sp));
    //     }
    // }
    //
    // void ebmlMasterElement::_scanChildren(parseFile::iterator& iter) {
    //     while (!iter.atEnd()) {
    //         parseFile parsed = *iter;
    //         this->_handleParseFile(parsed);
    //         ++iter;
    //     }
    // }
    //
    // void ebmlMasterElement::_cscanChildren(parseFile::iterator& iter) {
    //     while (!iter.atEnd()) {
    //         parseFile parsed = *iter;
    //         this->_chandleParseFile(parsed);
    //         ++iter;
    //     }
    // }
    //
    // void ebmlMasterElement::_handleParseFile(const parseFile& parsed) {
    //     auto elem = cls()._decodeChild(parsed);
    //     _addChild(elem->sp());
    // }
    //
    // void ebmlMasterElement::_chandleParseFile(const parseFile& parsed) {
    //     auto elem = cls()._cdecodeChild(parsed);
    //     _addChild(elem->sp());
    // }
}
#endif
