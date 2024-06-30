#ifndef EBML_NG_SCHEMA_H
#define EBML_NG_SCHEMA_H

#include <string>
#include "ptrs.h"

namespace ebml {
    struct range_t {
        unsigned long min = 0;
        long max = -1;
        unsigned long def = 0;
        range_t(unsigned long);
        range_t(unsigned long, long);
        range_t(unsigned long, long, unsigned long);
    };

    class ebmlSchema {
    private:
        std::string _doctype;
        unsigned long _doctypeversion;
        range_t _maxIDLength;
        range_t _maxSizeLength;
        const ebmlElementClass* _rootcls;

    protected:
        virtual ebmlDocument* _new(const ioBase_sp&) const;
        virtual ebmlDocument* _new(ioBase_sp&&) const;

    public:
        ebmlSchema(const std::string&, unsigned long, const ebmlElementClass*, const range_t& maxIDLength, const range_t& maxSizeLength);

        // Open existing document
        template<typename T=int>
        ebmlDocument_sp open(const std::string&, bool write=false) const;

        // Creating new document.
        template<typename T=int>
        ebmlDocument_sp create(const std::string&, size_t initSize) const;

        template<typename T=int>
        ebmlDocument_sp create(const std::string&, size_t initSize, unsigned long maxIDLength, unsigned long maxSizeLength) const;

        const ebmlElementClass* const& rootcls() const;
        const std::string& docType() const;
        const range_t& maxIDLength() const;
        const range_t& maxSizeLength() const;
        unsigned long docTypeVersion() const;
    };

    extern template ebmlDocument_sp ebmlSchema::open<int>(const std::string&, bool) const;
    extern template ebmlDocument_sp ebmlSchema::create<int>(const std::string&, size_t) const;
    extern template ebmlDocument_sp ebmlSchema::create<int>(const std::string&, size_t, unsigned long, unsigned long) const;
    extern template ebmlDocument_sp ebmlSchema::open<FILE*>(const std::string&, bool) const;
    extern template ebmlDocument_sp ebmlSchema::create<FILE*>(const std::string&, size_t) const;
    extern template ebmlDocument_sp ebmlSchema::create<FILE*>(const std::string&, size_t, unsigned long, unsigned long) const;
}

#endif
