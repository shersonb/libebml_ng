#ifndef EBML_NG_MASTERELEMENTCLASS_H
#define EBML_NG_MASTERELEMENTCLASS_H

#include "../../ebmlElementType.h"
#include "../../ebmlElementType.tpp"
#include "childTypeSpec.h"
#include "sizetree_t.h"
#include <vector>

namespace ebml {
    class ebmlMasterElement;

    typedef std::unordered_map<ebmlID_t, unsigned long> occur_d;

    /**
     * @defgroup master_element_type_definitions Master Element Types
     * @brief Classes that define the structure and properties of EBML master elements.
     *
     * This group contains the base classes that are used as a foundation for all
     * hierarchical EBML element types.
     *
     * @ingroup type_definitions
     */

    /**
     * @defgroup master_element_type_templates Master Element Templates
     * @brief Template classes for creating EBML master element types.
     *
     * This group contains template classes that provide the blueprints for
     * creating master element types that have specialized behavior, such as
     * maps or lazy-loading structures.
     *
     * @ingroup master_element_type_definitions
     */

    /**
     * @defgroup master_element_type_implementations Master Element Implementations
     * @brief Concrete classes that hold and manage hierarchical EBML element data.
     *
     * This group contains the functional classes that are instantiated from master
     * element types. These classes are responsible for managing child elements
     * and handling the hierarchical structure of the EBML data.
     *
     * @ingroup master_element_type_definitions
     */

    /**
     * @ingroup type_definitions
     * @ingroup master_element_type_definitions
     * @class ebmlMasterElementType
     * @brief Represents the base type for all EBML master elements.
     *
     * This class provides the fundamental structure and functionality for EBML
     * elements that can contain other elements. It inherits from the CRTP base
     * to enable type‐safe instance creation and decoding.
     */
    class ebmlMasterElementType : public ebmlTypeCRTP<ebmlMasterElementType, ebmlMasterElement> {
        friend class ebmlMasterElement;

    protected:
        childTypeSpec_t _childTypes;

    public:
        ebmlMasterElementType(const char*, const std::wstring&, const childTypeSpecArg_l &);
        ebmlMasterElementType(ebmlID_t, const std::wstring&, const childTypeSpecArg_l &);
        ebmlMasterElementType(const char*, const std::wstring&, const childTypeSpecArg_l &, const seekHelper_t* seekHelper);
        ebmlMasterElementType(ebmlID_t, const std::wstring&, const childTypeSpecArg_l &, const seekHelper_t* seekHelper);

    protected:
        ebmlMasterElementType(const char*, const std::wstring&);
        ebmlMasterElementType(ebmlID_t, const std::wstring&);
        ebmlMasterElementType(const char*, const std::wstring&, const seekHelper_t* seekHelper);
        ebmlMasterElementType(ebmlID_t, const std::wstring&, const seekHelper_t* seekHelper);

        inline ebmlElement* _decodeChild(const parseString&) const;
        inline ebmlElement* _decodeChild(const parseFile&) const;
        inline ebmlElement* _cdecodeChild(const parseString&) const;
        inline ebmlElement* _cdecodeChild(const parseFile&) const;

    public:
        /**
         * @brief Gets the child type specification.
         *
         * @return A constant reference to the childTypeSpec_t.
         */
        inline const childTypeSpec_t & childTypes() const {
            return _childTypes;
        };
    };

    inline ebmlElement* ebmlMasterElementType::_decodeChild(const parseString& parsed) const {
        const ebmlElementType * childcls;

        try {
            childcls = this->_childTypes[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            throw;
        }

        return childcls->_decode_nocheck(parsed);
    }

    inline ebmlElement* ebmlMasterElementType::_cdecodeChild(const parseString& parsed) const {
        const ebmlElementType * childcls;

        try {
            childcls = this->_childTypes[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            throw;
        }

        return childcls->_decode_nocheck(parsed);
    }

    inline ebmlElement* ebmlMasterElementType::_decodeChild(const parseFile& parsed) const {
        const ebmlElementType * childcls;
        // std::cout << "E " << parsed.offset << " " << parsed.dataOffset() << " " << parsed.dataSize << std::endl;
        std::cout << typeof(this) << "::_decodeChild(const parseFile&) const" << std::endl;

        try {
            childcls = this->_childTypes[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.erroroffset = parsed.offset;
            throw;
        }

        return childcls->_decode_nocheck(parsed);
    }

    inline ebmlElement* ebmlMasterElementType::_cdecodeChild(const parseFile& parsed) const {
        const ebmlElementType * childcls;

        try {
            childcls = this->_childTypes[parsed.ebmlID];
        } catch (ebmlNoChildMatch& e) {
            e.offset = parsed.offset;
            throw;
        }

        // return childcls->mcdecode(parsed);
        return childcls->_decode_nocheck(parsed);
    }

    extern template class ebmlTypeCRTP<ebmlMasterElementType, ebmlMasterElement, ebmlElementType>;
}
#endif
