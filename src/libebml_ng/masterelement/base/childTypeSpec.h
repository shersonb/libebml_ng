#ifndef EBML_NG_CHILDCLASSSPEC_H
#define EBML_NG_CHILDCLASSSPEC_H

// #include "../ebmlID_t.h"
// #include <cstddef>
#include "../../ebmlElementType.h"
// #include "ebmlMasterElementClass.h"
// #include "../element.h"
// #include "../exceptions.h"
//
#include <unordered_map>
#include <unordered_set>
// #include <deque>
#include <list>
// #include <vector>
// #include <optional>

namespace ebml {
    /**
     * @brief Structure representing a single child type specification argument.
     *
     * This structure holds the pointer to an `ebml::ebmlElementType` along with the minimum
     * and maximum allowed occurrences for that child element.
     *
     * Instances of this class, as well as lists thereof (through `std::list` or `std::initializer_list`)
     * can be used to initialize `ebml::childTypeSpec_t` objects.
     */
    struct childTypeSpecArg_t {
        childTypeSpecArg_t();
        childTypeSpecArg_t(const ebmlElementType *, unsigned long min=0, long max=-1);
        const ebmlElementType * cls;
        unsigned long min; // Minimum number of occurrences required.
        long max; // Maximum number of occurrences permitted. Negative values interpreted as infinity.
    };

    /**
     * @brief Alias for a list of child type specification arguments.
     */
    typedef std::list<childTypeSpecArg_t> childTypeSpecArg_l;

    /**
     * @brief Alias for an initializer list of child type specification arguments.
     */
    typedef std::initializer_list<childTypeSpecArg_t> childClassSpecArg_init_l;

    /**
     * @brief Structure representing occurrence specifications.
     */
    struct occurSpec_t {
        unsigned long min;
        long max;
    };

    /**
     * @brief Defines the mapping from EBML ID to ebmlElementType pointers.
     */
    typedef std::unordered_map<ebmlID_t, const ebmlElementType *> ebmlElementClass_d;

    /**
     * @brief Defines a set of ebmlElementType pointers.
     */
    typedef std::unordered_set<const ebmlElementType *> ebmlElementClass_s;

    /**
     * @brief Defines the mapping from EBML ID to occurrence specifications.
     */
    typedef std::unordered_map<ebmlID_t, occurSpec_t> occurSpec_d;

    /**
     * @brief Specifies accepted child element types within a parent element.
     *
     * This class is used to look up allowed ebmlElementType instances by EBML ID
     * and to validate occurrences. Instances of this class are stored within an
     * `ebml::ebmlMasterElementType` instance.
     */
    class childTypeSpec_t {
    private:
        ebmlElementClass_d _cls_by_ebmlID;
        ebmlElementClass_s _clss;
        occurSpec_d _occurspec;
        inline void _clear() {
            _cls_by_ebmlID.clear();
            _clss.clear();
            _occurspec.clear();
        }

    public:
        childTypeSpec_t();
        childTypeSpec_t(childClassSpecArg_init_l);
        childTypeSpec_t(const childTypeSpecArg_l&);
        childTypeSpec_t(const childTypeSpec_t&) = default;
        childTypeSpec_t(childTypeSpec_t&&) = default;

        childTypeSpec_t & operator=(childClassSpecArg_init_l);
        childTypeSpec_t & operator=(const childTypeSpecArg_l&);
        childTypeSpec_t & operator=(const childTypeSpec_t&) = default;
        childTypeSpec_t & operator=(childTypeSpec_t&&) = default;

        /**
         * @brief Adds a child type.
         *
         * @param spec The specification to add.
         */
        void add(const childTypeSpecArg_t&);

        /**
         * @brief Adds a child type with occurrence limits.
         *
         * @param child Pointer to the `ebml::ebmlElementType`.
         * @param min Minimum required occurrences.
         * @param max Maximum allowed occurrences.
         */
        void add(const ebmlElementType*, unsigned long min=0, long max=-1);

        /**
         * @brief Removes child type for a given EBML ID.
         *
         * @param ebmlID The EBML ID to remove.
         */
        void remove(ebmlID_t);

        /**
         * @brief Retrieves the child type for a given EBML ID.
         *
         * @param ebmlID The EBML ID.
         * @return Pointer to the corresponding ebmlElementType.
         * @throws ebmlNoChildMatch if no matching type is found.
         */
        const ebmlElementType* operator[](ebmlID_t) const;

        /**
         * @brief Determines if a child type for a given EBML ID exists.
         *
         * @param ebmlID The EBML ID.
         * @return Count of matching specifications.
         */
        size_t count(ebmlID_t) const;

        /**
         * @brief Determines if an `ebml::ebmlElementType` instance is a child type.
         *
         * @param cls Pointer to the element type.
         * @return Occurrence count.
         */
        size_t count(const ebmlElementType*) const;

        /**
         * @brief Returns the total number of child type specifications.
         *
         * @return Total count.
         */
        size_t size() const;

        /**
         * @brief Retrieves the complete occurrence specifications.
         *
         * @return A constant reference to the occurrence specification map.
         */
        const occurSpec_d& occurSpec() const;

        /**
         * @brief Retrieves the occurrence specification for a given EBML ID.
         *
         * @param ebmlID The EBML ID.
         * @return The occurrence specification.
         */
        const occurSpec_t& occurSpec(ebmlID_t) const;

        /**
         * @brief Retrieves the occurrence specification for a given child type.
         *
         * @param cls Pointer to the child element type.
         * @return The occurrence specification.
         */
        const occurSpec_t& occurSpec(const ebmlElementType*) const;

        /**
         * @brief Checks whether a given element is valid under this specification.
         *
         * @param elem The EBML element smart pointer to validate.
         * @return True if valid, false otherwise.
         */
        bool isValid(const ebmlElement_sp&) const;

        /**
         * @brief Returns an iterator pointing to the beginning of the specifications.
         *
         * @return Constant iterator.
         */
        ebmlElementClass_d::const_iterator begin() const;

        /**
         * @brief Returns an iterator pointing past the end of the specifications.
         *
         * @return Constant iterator.
         */
        ebmlElementClass_d::const_iterator end() const;
    };
}
#endif
