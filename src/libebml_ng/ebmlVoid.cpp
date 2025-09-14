#ifndef EBML_NG_VOID_CPP
#define EBML_NG_VOID_CPP

#include <cstring>
#include <sstream>
#include <iomanip>

#include "ebmlVoid.h"
#include "vint.h"
#include "ebmlElementType.tpp"
#include "ebmlElement.tpp"

namespace ebml {
    ebmlVoidType::ebmlVoidType() : ebmlTypeCRTP<ebmlVoidType, ebmlVoid>("\xec", L"EBMLVoid") {}

    unsigned char ebmlVoidType::writeVoid(char* dest, size_t outerSize) const {
        if (outerSize < 2) {
            throw std::invalid_argument("cannot write void with outerSize < 2");
        }

        if (outerSize == UNKNOWN) {
            dest[0] = '\xec';
            packVint(UNKNOWN, 8, dest + 1);
            return 9;
        }

        unsigned char k = 1;

        while (k < 9) {
            if ((outerSize - 1 - k) < ((1LL << (7*k)) - 1)) {
                dest[0] = '\xec';
                packVint(outerSize - 1 - k, k, dest + 1);
                return k + 1;
            }
            ++k;
        }

        throw std::runtime_error("unable to write Void element for unknown reason");
    }

    unsigned char ebmlVoidType::writeVoid(ioBase& file, off_t offset, size_t outerSize) const {
        char buffer[9];
        auto size = writeVoid(buffer, outerSize);
        file.write(buffer, offset, size);
        return size;
    }

    ebmlVoid::ebmlVoid(const ebmlVoidType* cls)
      : ebmlElementCRTP<ebmlVoidType, ebmlVoid>(cls), voidsize(0) {}
    ebmlVoid::ebmlVoid(const ebmlVoidType* cls, const parseString& parsed) : ebmlVoid(cls, parsed.dataSize) {}
    ebmlVoid::ebmlVoid(const ebmlVoidType* cls, const parseFile& parsed) : ebmlVoid(cls, parsed.dataSize) {
        parsed.seek(parsed.dataSize);
    }
    ebmlVoid::ebmlVoid(const ebmlVoidType* cls, size_t size)
      : ebmlElementCRTP<ebmlVoidType, ebmlVoid>(cls), voidsize(size) {}
    ebmlVoid::ebmlVoid(const ebmlVoid& orig)
      : ebmlElementCRTP<ebmlVoidType, ebmlVoid>(&orig.cls()), voidsize(orig.voidsize) {}

    size_t ebmlVoid::dataSize() const {
        return this->voidsize;
    }

    size_t ebmlVoid::_encode(char* dest) const {
        memset(dest, 0, this->voidsize);
        return this->voidsize;
    }

    size_t ebmlVoid::encode(ioBase* file) const {
        char buffer[16];
        auto headSize = this->_encode_head(buffer, this->voidsize);

        file->write(buffer, headSize);

        if (file->seekable()) {
            file->seek(this->voidsize, SEEK_CUR);
        } else {
            char zeros[4096];
            memset(zeros, 0, 4096);

            for (size_t k = 0; k < this->voidsize; k += 4096) {
                file->write(zeros, std::min(static_cast<size_t>(4096), static_cast<size_t>(this->voidsize - k)));
            }
        }

        return this->voidsize;
    }

    void ebmlVoid::_clonedata(const ebmlElement* orig) {
        const ebmlVoid* orig_typed = dynamic_cast<const ebmlVoid*>(orig);
        if (orig_typed) {
            this->voidsize = orig_typed->voidsize;
        }
    }

    std::wstring ebmlVoid::minirepr() const {
        return std::to_wstring(this->voidsize);
    }
    std::wstring ebmlVoid::repr() const {
        auto mrepr = this->minirepr();

        std::wstringstream ss;

        char ebmlID_chars[8];
        unsigned char ebmlIDWidth;

        ebmlIDWidth = packVint(this->ebmlID(), ebmlID_chars);

        ss << L"<";
        ss << this->cls().name;

        ss << L" object: ebmlID=0x";

        for (unsigned char k = 0; k < ebmlIDWidth; k++) {
            ss << std::hex << std::setw(2) << std::setfill(L'0') << static_cast<unsigned char>(ebmlID_chars[k]);
        }

        if (mrepr.size() > 0) {
            ss << L", voidsize=";
            ss << mrepr;
        }

        ss << L">";
        return ss.str();
    }

    // ebmlVoidType Void = ebmlVoidType();
    ebmlVoidType Void;
}
#endif
