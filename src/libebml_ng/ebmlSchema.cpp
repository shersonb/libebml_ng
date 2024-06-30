#ifndef EBML_NG_SCHEMA_CPP
#define EBML_NG_SCHEMA_CPP

#include "libebml_ng/ebmlSchema.h"
#include "libebml_ng/ebmlHead.h"
#include "libebml_ng/exceptions.h"
#include "libebml_ng/ebmlDocument.h"
#include "libebml_ng/masterelement/ebmlList.h"
#include "libebml_ng/masterelement/lazyload/ebmlLazyLoad.h"
#include "libebml_ng/ebmlElementClass.tpp"

namespace ebml {
    range_t::range_t(unsigned long min) : min(min), max(min), def(min) {}
    range_t::range_t(unsigned long min, long max) : min(min), max(max), def(max) {}
    range_t::range_t(unsigned long min, long max, unsigned long def) : min(min), max(max), def(def) {}

    ebmlSchema::ebmlSchema(const std::string& docType, unsigned long docTypeVersion, const ebmlElementClass* rootcls, const range_t& maxIDLength, const range_t& maxSizeLength)
    : _doctype(docType), _doctypeversion(docTypeVersion), _maxIDLength(maxIDLength), _maxSizeLength(maxSizeLength), _rootcls(rootcls) {}

    ebmlDocument* ebmlSchema::_new(const ioBase_sp& file) const {
        return new ebmlDocument(this, file);
    }

    ebmlDocument* ebmlSchema::_new(ioBase_sp&& file) const {
        return new ebmlDocument(this, std::move(file));
    }

    template<typename T>
    ebmlDocument_sp ebmlSchema::open(const std::string& filename, bool write) const {
        auto mode = std::ios::in | std::ios::binary;

        if (write) {
            mode |= std::ios::out;
        }

        ioBase_sp file_sp = io<T>::open(filename, mode);
        auto& file = *file_sp;

        off_t offset = 0;
        auto head = EBMLHead.decode(file, offset);
        const ebmlElement_sp& doctype = head->as<ebmlMultiSlot>()["docType"];
        // auto& doctype = data<std::string>(doctype);

        if (data<std::string>(doctype) != this->_doctype) {
            std::string errmsg = "expected ";
            errmsg += this->_doctype;
            errmsg += ", got ";
            errmsg += data<std::string>(doctype);
            errmsg += " instead";
            throw ebmlException(errmsg);
        }

        auto root = this->_rootcls->decode(file, offset);

        ebmlDocument* doc = this->_new(std::move(file_sp));
        ebmlDocument_sp doc_sp = ebmlDocument_sp(doc);

        doc->setHead(std::move(head));
        doc->setRoot(std::move(root));

        return doc_sp;
    }

    template<typename T>
    ebmlDocument_sp ebmlSchema::create(const std::string& filename, size_t initSize, unsigned long maxIDLength, unsigned long maxSizeLength) const {
        static auto mode = std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc;
        ioBase_sp file_sp = io<T>::open(filename, mode);
        auto& file = *file_sp;

        auto head = EBMLHead(slotArg_d({
            {"ebmlVersion", EBMLVersion(1)},
            {"ebmlReadVersion", EBMLReadVersion(1)},
            {"ebmlMaxIDLength", EBMLMaxIDLength(maxIDLength)},
            {"ebmlMaxSizeLength", EBMLMaxSizeLength(maxSizeLength)},
            {"docType", DocType(this->_doctype)},
            {"docTypeVersion", DocTypeVersion(this->_doctypeversion)},
            {"docTypeReadVersion", DocTypeReadVersion(this->_doctypeversion)}
        }));

        auto ebmlheadsize = head->encode(file, 0);
        auto root = this->_rootcls->as<const ebmlLazyLoadClass>()(file, static_cast<off_t>(ebmlheadsize), static_cast<vintWidth_t>(8), static_cast<size_t>(initSize));

        ebmlDocument* doc = _new(std::move(file_sp));
        ebmlDocument_sp doc_sp = ebmlDocument_sp(doc);

        doc->setHead(std::move(head));
        doc->setRoot(std::move(root));
        return doc_sp;
    }

    template<typename T>
    ebmlDocument_sp ebmlSchema::create(const std::string& filename, size_t initSize) const {
        return this->create(filename, initSize, this->_maxIDLength.def, this->_maxSizeLength.def);
    }

    template ebmlDocument_sp ebmlSchema::open<int>(const std::string&, bool) const;
    template ebmlDocument_sp ebmlSchema::create<int>(const std::string&, size_t) const;
    template ebmlDocument_sp ebmlSchema::create<int>(const std::string&, size_t, unsigned long, unsigned long) const;
    template ebmlDocument_sp ebmlSchema::open<FILE*>(const std::string&, bool) const;
    template ebmlDocument_sp ebmlSchema::create<FILE*>(const std::string&, size_t) const;
    template ebmlDocument_sp ebmlSchema::create<FILE*>(const std::string&, size_t, unsigned long, unsigned long) const;
}
#endif
