#ifndef EBML_NG_LAZYLOADMASTERELEMENT_CPP
#define EBML_NG_LAZYLOADMASTERELEMENT_CPP

#include "libebml_ng/masterelement/lazyload.h"
#include "libebml_ng/document.h"
#include "libebml_ng/vint.h"
#include "libebml_ng/void.h"

namespace ebml {
    ebmlLazyLoadClass::ebmlLazyLoadClass(ebmlID_t ebmlID, const std::wstring& name, const occurSpec_t& recursive) : ebmlMasterElementClass(ebmlID, name) {
        if (recursive.max != 0) {
            this->_childClasses.add({this, recursive.min, recursive.max});
        }
    }

    ebmlLazyLoadClass::ebmlLazyLoadClass(const char* ebmlID, const std::wstring& name, const occurSpec_t& recursive) : ebmlLazyLoadClass(unpackVint(ebmlID), name, recursive) {}

    ebmlLazyLoadClass::ebmlLazyLoadClass(ebmlID_t ebmlID, const std::wstring& name, const childClassSpecArg_l& childClasses, const occurSpec_t& recursive)
    : ebmlMasterElementClass(ebmlID, name) {
        this->_childClasses = childClasses;

        if (recursive.max != 0) {
            this->_childClasses.add({this, recursive.min, recursive.max});
        }
    }

    ebmlLazyLoadClass::ebmlLazyLoadClass(const char* ebmlID, const std::wstring& name, const childClassSpecArg_l& childClasses, const occurSpec_t& recursive)
    : ebmlLazyLoadClass(unpackVint(ebmlID), name, childClasses, recursive) {}

    ebmlElement_sp ebmlLazyLoadClass::operator()(ioBase* file, off_t offset, vintWidth_t sizeWidth, size_t dataSize) const {
        // auto elem = new ebmlLazyLoad(this, file, offset, sizeWidth, dataSize);
        auto elem = this->_new()->ptr<ebmlLazyLoad>();
        auto elem_sp = ebmlElement_sp(elem);
        elem->_initNew(file, offset, sizeWidth, dataSize);
        file->truncate(elem->dataOffsetInFile() + dataSize);
        return elem_sp;
    }

    void ebmlLazyLoad::_handleParseFile(const parseFile& parsed) {
        off_t childOffset = parsed.offset - parsed.parentOffset;
        const ebmlElementClass* childClass = this->cls()->childClasses()[parsed.ebmlID];
        seekData_t* seekData = childClass->makeSeekData(parsed);
        auto seekData_up = std::unique_ptr<seekData_t>(seekData);
        this->_handleSeekData(std::move(seekData_up));
    }

    void ebmlLazyLoad::_handleSeekData(std::unique_ptr<seekData_t>&& ptr) {
        auto seekData = ptr.get();
        std::cout << "ebmlID: " << seekData->ebmlID << std::endl;

        this->_children_by_offset[seekData->offsetInParent] = std::move(ptr);
        this->_children_by_ebmlID[seekData->ebmlID][seekData->offsetInParent] = seekData;
    }

    ebmlElement* ebmlLazyLoadClass::_new() const {
        return new ebmlLazyLoad(this);
    }

    ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls) : ebmlMasterElement(cls) {}
    //
    // // Read existing element in file
    // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase*);
    // ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp&);
    // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&&);

    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase* file, off_t offset) ; ebmlLazyLoad(cls) {
    // }

    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp& file, off_t offset)
    // : ebmlLazyLoad(cls, file.get(), offset) {
    //     this->_file = file;
    //     this->_file_raw = file.get();
    // }
    //
    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&& file, off_t offset)
    // : ebmlLazyLoad(cls, file.get(), offset) {
    //     this->_file_raw = file.get();
    //     this->_file = std::move(file);
    // }

    // // Create new element in file
    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase* file, vintWidth_t sizeWidth, size_t dataSize) : ebmlMasterElement(cls) {
    //
    // }

    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp& file, vintWidth_t sizeWidth, size_t dataSize)
    // : ebmlLazyLoad(cls, file, file->tell(), sizeWidth, dataSize) {}
    //
    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&& file, vintWidth_t sizeWidth, size_t dataSize)
    // : ebmlLazyLoad(cls, std::move(file), file->tell(), sizeWidth, dataSize) {}

    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp& file, off_t offset, vintWidth_t sizeWidth, size_t dataSize)
    // : ebmlMasterElement(cls), _file(file), _dataSize(dataSize) {
    //     this->_offsetInParent = offset;
    //     this->_sizeWidth = sizeWidth;
    //
    //     if ((dataSize < UNKNOWN) and (dataSize >= ((1LL << (7*sizeWidth)) - 1))) {
    //         std::string errmsg = "size ";
    //         errmsg += std::to_string(dataSize);
    //         errmsg += " too large for sizeWidth ";
    //         errmsg += std::to_string(sizeWidth);
    //         throw std::invalid_argument(errmsg);
    //     }
    //
    //     this->_file_raw = file.get();
    //     this->_file = file;
    // }

    // ebmlLazyLoad::ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&& file, off_t offset, vintWidth_t sizeWidth, size_t dataSize)
    // : ebmlMasterElement(cls), _file(file), _dataSize(dataSize) {
    //     this->_offsetInParent = offset;
    //     this->_sizeWidth = sizeWidth;
    //
    //     if ((dataSize < UNKNOWN) and (dataSize >= ((1LL << (7*sizeWidth)) - 1))) {
    //         std::string errmsg = "size ";
    //         errmsg += std::to_string(dataSize);
    //         errmsg += " too large for sizeWidth ";
    //         errmsg += std::to_string(sizeWidth);
    //         throw std::invalid_argument(errmsg);
    //     }
    //
    //     this->_file_raw = file.get();
    //     this->_file = std::move(file);
    // }

    const ioBase_sp& ebmlLazyLoad::file() {
        auto doc = this->document();

        if (doc != nullptr) {
            return doc->file();
        }

        throw std::runtime_error("element does not have a document");
    }

    void ebmlLazyLoad::_initNew(ioBase* file, off_t offset, vintWidth_t sizeWidth, size_t dataSize) {
        if ((dataSize < UNKNOWN) and (dataSize >= ((1LL << (7*sizeWidth)) - 1))) {
            std::string errmsg = "size ";
            errmsg += std::to_string(dataSize);
            errmsg += " too large for sizeWidth ";
            errmsg += std::to_string(sizeWidth);
            throw std::invalid_argument(errmsg);
        }

        this->_offsetInParent = offset;
        this->_sizeWidth = sizeWidth;
        this->_dataSize = dataSize;

        char buffer[25]; // Room enough for ebmlID, size, and VOID element
        vintWidth_t ebmlIDWidth = widthAsVint(this->cls()->ebmlID);
        unsigned char headSize = ebmlIDWidth + this->_sizeWidth;

        packVint(this->_cls->ebmlID, ebmlIDWidth, buffer);
        packVint(this->_dataSize, this->_sizeWidth, buffer + ebmlIDWidth);

        unsigned char voidWidth = 0;

        if (this->_dataSize > 0) {
            voidWidth = Void.writeVoid(buffer + headSize, this->_dataSize);
        }

        file->write(buffer, this->offsetInFile(), headSize + voidWidth);
    }

    size_t ebmlLazyLoad::dataSize() const {
        return this->_dataSize;
    }
    // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&&, vintWidth_t, size_t);
    // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase*, off_t, vintWidth_t, size_t);
    // ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp&, off_t, vintWidth_t, size_t);
    // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&&, off_t, vintWidth_t, size_t);

    // TODO
    std::wstring ebmlLazyLoad::minirepr() const {
        return L"";
    }

    // Do nothing
    void ebmlLazyLoad::_addChild(const ebmlElement_sp&) {}
    void ebmlLazyLoad::_addChild(ebmlElement_sp&&) {}

    // TODO
    ebmlMasterElement::_iterator* ebmlLazyLoad::_begin() {return nullptr;}
    ebmlMasterElement::_iterator* ebmlLazyLoad::_end() {return nullptr;}
    ebmlMasterElement::_const_iterator* ebmlLazyLoad::_cbegin() const {return nullptr;}
    ebmlMasterElement::_const_iterator* ebmlLazyLoad::_cend() const {return nullptr;}

    INST_AS_MEMBERS(ebmlLazyLoadClass, ebmlLazyLoad)
}

#endif
