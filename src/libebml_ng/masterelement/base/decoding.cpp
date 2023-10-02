#ifndef EBML_NG_MASTERELEMENT_DECODE_CPP
#define EBML_NG_MASTERELEMENT_DECODE_CPP

#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/exceptions.h"

namespace ebml {
    // From string
    void ebmlMasterElement::_decode(const parseString& parsed) {
        try {
            if (parsed.dataSize > 0) {
                parseString::iterator iter = parsed.begin();
                this->_decodeChildren(iter);
            }
        } catch (ebmlDecodeError& e) {
            if (e.cls != nullptr) {
                e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }
    }

    void ebmlMasterElement::_cdecode(const parseString& parsed) {
        try {
            if (parsed.dataSize > 0) {
                parseString::iterator iter = parsed.begin();
                this->_cdecodeChildren(iter);
            }
        } catch (ebmlDecodeError& e) {
            if (e.cls != nullptr) {
                e.offset += parsed.ebmlIDWidth + parsed.sizeWidth;
            }
            throw;
        }
    }

    void ebmlMasterElement::_decodeChildren(parseString::iterator& iter) {
        parseString parsed;
        ebmlElement_sp elem;
        this->_clear();

        while (!iter.atEnd()) {
            parsed = *iter;
            ++iter;

            if (parsed.ebmlIDWidth == 0) {
                break;
            }

            if (parsed.ebmlID == Void.ebmlID) {
                continue;
            }

            try {
                elem = this->_decodeChild(parsed);
            } catch (ebmlDecodeError& e) {
                if (e.cls != nullptr) {
                    e.offset += parsed.offset;
                }
                throw;
            }

            this->_attachChild(elem);
            this->_addChild(std::move(elem));
        }
    }

    void ebmlMasterElement::_cdecodeChildren(parseString::iterator& iter) {
        parseString parsed;
        ebmlElement_sp elem;
        this->_clear();

        while (!iter.atEnd()) {
            parsed = *iter;
            ++iter;

            if (parsed.ebmlIDWidth == 0) {
                break;
            }

            if (parsed.ebmlID == Void.ebmlID) {
                continue;
            }

            try {
                elem = this->_cdecodeChild(parsed);
            } catch (ebmlDecodeError& e) {
                if (e.cls != nullptr) {
                    e.offset += parsed.offset;
                }
                throw;
            }

            this->_attachChild(elem);
            this->_addChild(std::move(elem));
        }
    }

    ebmlElement_sp ebmlMasterElement::_decodeChild(const parseString& parsed) {
        const ebmlElementClass* childcls;

        try {
            childcls = this->cls()->childClasses()[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            // e.cls = this->cls();
            throw;
        }

        return childcls->decode(parsed);
    }

    ebmlElement_sp ebmlMasterElement::_cdecodeChild(const parseString& parsed) {
        const ebmlElementClass* childcls;

        try {
            childcls = this->cls()->childClasses()[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            // e.cls = this->cls();
            throw;
        }

        return childcls->_cdecode(parsed);
    }

    // From file
    void ebmlMasterElement::_decode(const parseFile& parsed) {
        if (parsed.dataSize > 0) {
            parseFile::iterator iter = parsed.begin();
            this->_scanChildren(iter);
        }
    }

    void ebmlMasterElement::_cdecode(const parseFile& parsed) {
        if (parsed.dataSize > 0) {
            parseFile::iterator iter = parsed.begin();
            this->_cscanChildren(iter);
        }
    }

    void ebmlMasterElement::_scanChildren(parseFile::iterator& iter) {
        while (!iter.atEnd()) {
            parseFile parsed = *iter;
            this->_handleSeekData(parsed);
            ++iter;
        }
    }

    void ebmlMasterElement::_cscanChildren(parseFile::iterator& iter) {
        while (!iter.atEnd()) {
            parseFile parsed = *iter;
            this->_chandleSeekData(parsed);
            ++iter;
        }
    }

    void ebmlMasterElement::_handleSeekData(const parseFile& parsed) {
        auto elem = this->_decodeChild(parsed);
        this->_addChild(elem);
    }

    void ebmlMasterElement::_chandleSeekData(const parseFile& parsed) {
        auto elem = this->_cdecodeChild(parsed);
        this->_addChild(elem);
    }

    ebmlElement_sp ebmlMasterElement::_decodeChild(const parseFile& parsed) {
        const ebmlElementClass* childcls;

        try {
            childcls = this->cls()->childClasses()[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            e.cls = this->cls();
            throw;
        }

        return childcls->decode(parsed);
    }

    ebmlElement_sp ebmlMasterElement::_cdecodeChild(const parseFile& parsed) {
        const ebmlElementClass* childcls;

        try {
            childcls = this->cls()->childClasses()[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.offset = parsed.offset;
            e.cls = this->cls();
            throw;
        }

        return childcls->_cdecode(parsed);
    }
}
#endif
