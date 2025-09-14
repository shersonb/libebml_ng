#ifndef EBML_NG_SCHEMA_H
#define EBML_NG_SCHEMA_H

#include <string>
#include <unordered_map>
#include <vector>
#include "ptrs.h"
#include "ebmlID_t.h"

namespace ebml {
    struct element_t {
        std::wstring name;
        std::vector<std::string> path;
        ebmlID_t id;
        std::string type;
        std::string range;                                  // TODO: Implement std::variant
        std::string def;                                    // TODO: Implement std::any
        std::unordered_map<std::string, std::string> documentation;
        std::unordered_map<std::string, std::string> usage;
        mutable ebmlElementType* elemtype;
    };

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
        const ebmlElementType* _rootcls;
        std::unordered_map<std::wstring, ebmlID_t> _ebmlIDMap;
        std::unordered_map<ebmlID_t, element_t> _elementTypes;

    protected:
        virtual ebmlDocument* _new(const ioBase_sp&) const;
        virtual ebmlDocument* _new(ioBase_sp&&) const;

    public:
        ebmlSchema(const std::string&, unsigned long, const ebmlElementType*, const range_t& maxIDLength, const range_t& maxSizeLength);

        // Open existing document
        template<typename T=int>
        ebmlDocument_sp open(const std::string&, bool write=false) const;

        // Creating new document.
        template<typename T=int>
        ebmlDocument_sp create(const std::string&, size_t initSize) const;

        template<typename T=int>
        ebmlDocument_sp create(const std::string&, size_t initSize, unsigned long maxIDLength, unsigned long maxSizeLength) const;

        const ebmlElementType* const& rootcls() const;
        const std::string& docType() const;
        const range_t& maxIDLength() const;
        const range_t& maxSizeLength() const;
        unsigned long docTypeVersion() const;

        const ebmlElementType& getElementType(ebmlID_t ebmlID) const;
        const ebmlElementType& getElementType(const std::wstring& name) const;
    };

    extern template ebmlDocument_sp ebmlSchema::open<int>(const std::string&, bool) const;
    extern template ebmlDocument_sp ebmlSchema::create<int>(const std::string&, size_t) const;
    extern template ebmlDocument_sp ebmlSchema::create<int>(const std::string&, size_t, unsigned long, unsigned long) const;
    extern template ebmlDocument_sp ebmlSchema::open<FILE*>(const std::string&, bool) const;
    extern template ebmlDocument_sp ebmlSchema::create<FILE*>(const std::string&, size_t) const;
    extern template ebmlDocument_sp ebmlSchema::create<FILE*>(const std::string&, size_t, unsigned long, unsigned long) const;
}

#endif
