#ifndef EBML_NG_DOCUMENT_CPP
#define EBML_NG_DOCUMENT_CPP

#include "libebml_ng/ebmlDocument.h"
#include "libebml_ng/ebmlElement.h"

namespace ebml {
    ebmlDocument::ebmlDocument(const ebmlSchema* schema)
    : _schema(schema) {}

    ebmlDocument::ebmlDocument(const ebmlSchema* schema, const ioBase_sp& file)
    : _schema(schema), _file(file) {
        this->_blksize = this->_file->stat().st_blksize;
    }

    ebmlDocument::ebmlDocument(const ebmlSchema* schema, ioBase_sp&& file)
    : _schema(schema), _file(std::move(file)) {
        this->_blksize = this->_file->stat().st_blksize;
    }

    void ebmlDocument::setHead(ebmlElement_sp&& head) {
        auto this_sp = this->shared_from_this();
        head->_document = this_sp;
        this->_head = std::move(head);
    }

    void ebmlDocument::setRoot(ebmlElement_sp&& root) {
        root->_document = this->shared_from_this();
        this->_root = std::move(root);
    }

    ioBase& ebmlDocument::file() {
        if (_file != nullptr) {
            return *_file;
        }

        throw std::runtime_error("document does not have file object");
    }

    size_t ebmlDocument::blksize() const {
        return this->_blksize;
    }

    const ebmlElement_sp& ebmlDocument::root() const {
        return _root;
    }

    const ebmlElement_sp& ebmlDocument::head() const {
        return _head;
    }
    const ebmlSchema* ebmlDocument::schema() const {
        return this->_schema;
    }

    std::unique_lock<std::shared_mutex> ebmlDocument::wlock() const {
        return std::unique_lock<std::shared_mutex>(this->_lock);
    }

    std::shared_lock<std::shared_mutex> ebmlDocument::rlock() const {
        return std::shared_lock<std::shared_mutex>(this->_lock);
    }

    std::shared_mutex& ebmlDocument::mutex() const {
        return this->_lock;
    }
}
#endif
