#ifndef EBML_NG_PARSESTR_H
#define EBML_NG_PARSESTR_H

#include <stddef.h>
#include <unistd.h>

#include "libebml_ng/ebmlID_t.h"
#include "libebml_ng/parsing/io.h"
#include "libebml_ng/repr.h"
#include "libebml_ng/struct/binary.h"

namespace ebml {
    /**
     * @brief Represents a parsed EBML string segment.
     *
     * A parseString holds the parsed details for an EBML element extracted
     * from a raw buffer. It stores the EBML ID, the widths used for encoding,
     * the size of the data, and a pointer into the raw data. It may also reference
     * a "parent" parseString for nested elements.
     *
     * Public members:
     *  - ebmlID: The numeric ID of the EBML element.
     *  - ebmlIDWidth: The number of bytes used to store the EBML ID.
     *  - dataSize: The size (in bytes) of the element’s data.
     *  - sizeWidth: The number of bytes used to store the data size.
     *  - offset: The offset (relative to the parent element’s data) of this element.
     *  - data: Pointer to the start of the element’s data.
     *  - parent: Pointer to a parent parseString (if any).
     *
     * It also defines an iterator for sequential parsing of nested EBML elements.
     *
     * @note The class provides a template member function unpack<T>() that uses the
     * externally defined function ebml::unpack<T>().
     *
     * @ingroup Parsing
     */
    class parseString {
    public:
        ebmlID_t    ebmlID;       /**< The EBML ID for the element. */
        vintWidth_t ebmlIDWidth;  /**< The width (in bytes) of the EBML ID. */
        size_t      dataSize;     /**< The size (in bytes) of the element data. */
        vintWidth_t sizeWidth;    /**< The width (in bytes) of the size field. */
        off_t       offset;       /**< Offset relative to start of parent data. */
        const char* data;         /**< Pointer to the beginning of the element data. */
        parseString* parent;      /**< Pointer to the parent parseString (if any). */

        parseString();
        parseString(const parseString&);
        parseString(parseString&&);
        parseString& operator=(const parseString&);
        parseString& operator=(parseString&&);

        /**
         * @brief Construct from a raw buffer.
         *
         * @param data Pointer to raw data containing EBML element.
         * @param size Total number of bytes available in the data.
         */
        parseString(const char*, size_t);

        /**
         * @brief Construct from a raw buffer with an offset parameter.
         *
         * @param data Pointer to raw data.
         * @param size Total size in bytes.
         * @param offset Offset relative to the parent element.
         */
        parseString(const char*, size_t, off_t);

        /**
         * @brief Main constructor that initializes all fields.
         *
         * @param ebmlID The EBML ID.
         * @param ebmlIDWidth Number of bytes used to encode ebmlID.
         * @param dataSize Size of the element data in bytes.
         * @param sizeWidth Number of bytes used to encode dataSize.
         * @param offset Offset relative to the containing parent's data.
         * @param data Pointer to the actual element data.
         */
        parseString(
            ebmlID_t, vintWidth_t,
            size_t, vintWidth_t,
            off_t, const char*);

        /**
         * @brief Construct a parseString directly from a parseFile object.
         *
         * @param parsed A parseFile object.
         * @param buffer A character buffer to hold the data.
         */
        parseString(const parseFile&, char*);

        class iterator;
        /**
         * @brief Returns an iterator to the beginning of the contained EBML elements.
         *
         * This iterator walks through the child elements contained in the data.
         *
         * @return An iterator for this parseString.
         */
        parseString::iterator begin() const;

        /**
         * @brief Returns the outer size (header + data) of the element.
         *
         * @return Total size in bytes of the element including EBML ID and size fields.
         */
        unsigned long long outerSize() const;

        /**
         * @brief Template method to unpack data from the element's payload.
         *
         * This function calls the global function ebml::unpack<T>() on the element's data.
         *
         * @tparam T The target type to unpack.
         * @return The unpacked value of type T.
         */
        template<typename T>
        inline T unpack() const {
            return ebml::unpack<T>(this->data, this->dataSize);
        }
    };

    /**
     * @brief Iterator for parseString objects.
     *
     * Provides sequential access to a sequence of EBML elements stored within the same
     * buffer as described by a parseString.
     */
    class parseString::iterator {
        const char* _data;
        off_t _offset;
        size_t _dataSize;
        parseString _next;
    public:
        /**
         * @brief Construct an iterator with the given data pointer and size.
         *
         * @param data Pointer to the beginning of the data.
         * @param size Total size in bytes of the data.
         */
        iterator(const char*, size_t);

        /**
         * @brief Dereference the iterator.
         *
         * @return A parseString representing the current element.
         */
        parseString operator*();

        /**
         * @brief Pre-increment operator.
         *
         * @return Reference to the iterator after advancing.
         */
        parseString::iterator& operator++();

        /**
         * @brief Post-increment operator.
         *
         * @return The iterator before increment.
         */
        parseString::iterator operator++(int);

        /**
         * @brief Equality comparison.
         *
         * @param other Another parseString iterator.
         * @return True if both iterators are equal.
         */
        bool operator==(const parseString::iterator&) const;

        /**
         * @brief Inequality comparison.
         *
         * @param other Another parseString iterator.
         * @return True if the iterators differ.
         */
        bool operator!=(const parseString::iterator&) const;

        /**
         * @brief Check whether the iterator has reached the end.
         *
         * @return True if no more elements remain.
         */
        bool atEnd() const;
    };
}
#endif
