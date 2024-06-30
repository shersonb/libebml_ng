#ifndef EBML_NG_DOCUMENT_H
#define EBML_NG_DOCUMENT_H

#include <shared_mutex>
#include <mutex>

#include "ptrs.h"

namespace ebml {
    class ebmlDocument : public std::enable_shared_from_this<ebmlDocument> {
    private:
        const ebmlSchema* _schema;
        ioBase_sp _file;
        ebmlElement_sp _head;
        ebmlElement_sp _root;
        mutable std::shared_mutex _lock;
        size_t _blksize = 0;

    protected:
        ebmlDocument(const ebmlSchema*);
        ebmlDocument(const ebmlSchema*, const ioBase_sp&);
        ebmlDocument(const ebmlSchema*, ioBase_sp&&);

        void setHead(const ebmlElement_sp&) {};
        void setHead(ebmlElement_sp&&);

        void setRoot(const ebmlElement_sp&) {};
        void setRoot(ebmlElement_sp&&);

    public:
        ioBase& file();
        size_t blksize() const;
        const ebmlElement_sp& root() const;

        template<typename T>
        ebml::ptr<T> root() const {
            return std::dynamic_pointer_cast<T>(this->_root);
        }

        const ebmlElement_sp& head() const;
        const ebmlSchema* schema() const;
        std::unique_lock<std::shared_mutex> wlock() const;
        std::shared_lock<std::shared_mutex> rlock() const;
        std::shared_mutex& mutex() const;

        friend class ebmlSchema;
    };
}

#endif
