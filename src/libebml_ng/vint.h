#ifndef EBML_NG_VINT_H
#define EBML_NG_VINT_H

#include "exceptions.h"
#include "io.h"
#include "ebmlID_t.h"
#include "struct.h"
#include "struct/ull.h"
#include <array>
#include <stdexcept>
#include <string>

namespace ebml {

    /// Special constant representing an unknown or maximum value.
    extern const size_t UNKNOWN;

    /**
     * @brief Determines the width (in bytes) of a variable-length integer (vint) based on its first byte.
     *
     * This function inspects the first byte of a vint and returns the number of bytes that make up the
     * entire vint. It examines the leading bits and uses them as a mask to determine the length.
     *
     * @param b The first byte of the vint.
     * @return The width (number of bytes) of the encoded vint.
     * @throws ebmlInvalidVint if the integer starts with a null byte.
     */
    inline unsigned char vintWidth(char b) {
        static const std::array<unsigned char, 8> masks = {{
            0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
        }};
        unsigned char k;
        for (k = 0; k < 8; ++k) {
            if (b & masks[k]) {
                return k + 1;
            }
        }
        throw ebmlInvalidVint("Variable-length integer cannot start with null byte.");
    }

    /**
     * @brief Computes the minimal width (in bytes) required to encode an unsigned long long as a vint.
     *
     * This function calculates how many bytes are needed to represent the given number in the variable-length
     * integer format used in EBML.
     *
     * @param n The number to encode.
     * @return The minimal width in bytes required to represent the number.
     * @throws std::overflow_error if the number is too large to encode.
     */
    inline unsigned char widthAsVint(unsigned long long n) {
        unsigned char k = 0;
        if (n == 0xffffffffffffffffULL) {
            return 8;
        }
        while (k <= 8) {
            if (n < ((1ULL << (7 * k)) - 1ULL)) {
                return k;
            }
            k++;
        }
        throw std::overflow_error(
            std::string("int too big to convert (") + std::to_string(__LINE__)
            + std::string(":") + std::string(__FILE__) + std::string(")")
        );
    }

    /**
     * @brief Decodes a variable-length integer (vint) from a byte array with a given size.
     *
     * This function unpacks the vint from the provided data buffer. It masks out the marker bits
     * that indicate the width.
     *
     * @param data Pointer to the byte array containing the encoded vint.
     * @param size The size (width) of the vint in bytes.
     * @return The decoded unsigned long long value.
     *
     * @note If the unpacked value equals the maximum representable value for the width, the function returns
     *       0xffffffffffffffffULL.
     */
    inline unsigned long long unpackVint(const char* data, unsigned char size) {
        unsigned long long val = unpack<unsigned long long>(data, size);
        val &= ~(1ULL << (7 * size)); // remove the marker bit
        if (val == ((1ULL << (7 * size)) - 1ULL)) {
            return 0xffffffffffffffffULL;
        }
        return val;
    }

    /**
     * @brief Automatically detects the vint width and decodes the variable-length integer.
     *
     * This function inspects the first byte, determines the full length of the vint, and then calls
     * the appropriate unpacking routine.
     *
     * @param data Pointer to the byte array containing the encoded vint.
     * @return The decoded unsigned long long value.
     */
    inline unsigned long long unpackVint(const char* data) {
        unsigned char size = vintWidth(data[0]);
        return unpackVint(data, size);
    }

    /**
     * @brief Encodes an unsigned long long as a variable-length integer (vint) with a specified width.
     *
     * This function encodes the given number into the vint format using the provided width.
     *
     * @param n The number to encode.
     * @param size The desired width (in bytes) for the encoded vint.
     * @param dest Pointer to the destination buffer where the encoded vint will be stored.
     * @throws std::invalid_argument if the provided width is not within the valid range [1, 8].
     * @throws std::overflow_error if the number is too large to fit within the provided width.
     */
    inline void packVint(unsigned long long n, unsigned char size, char* dest) {
        if ((size == 0) || (size > 8)) {
            throw std::invalid_argument(
                std::string("packVint: invalid value for size (") + std::to_string(__LINE__)
                + std::string(":") + std::string(__FILE__) + std::string(")")
            );
        }

        if (n == 0xffffffffffffffffULL) {
            pack((1ULL << (7 * size + 1)) - 1ULL, size, dest);
            return;
        }

        if (((1ULL << (7 * size)) - 1ULL) <= n) {
            throw std::overflow_error(
                std::string("int too big to convert (") + std::to_string(__LINE__)
                + std::string(":") + std::string(__FILE__) + std::string(")")
            );
        }

        pack((1ULL << (7 * size)) | n, size, dest);
    }

    /**
     * @brief Encodes an unsigned long long as a variable-length integer (vint) by automatically determining the required width.
     *
     * This wrapper function calculates the minimal width needed to represent the number as a vint,
     * then encodes the number to the destination buffer.
     *
     * @param n The number to encode.
     * @param dest Pointer to the destination buffer where the encoded vint will be stored.
     * @return The width (in bytes) used to encode the number.
     */
    inline unsigned char packVint(unsigned long long n, char* dest) {
        unsigned char size = widthAsVint(n);
        packVint(n, size, dest);
        return size;
    }

    /**
     * @brief Decodes a vint from a byte array of known size.
     *
     * This function is used when the data buffer size is known. It will throw an exception if the data is insufficient.
     *
     * @param data Pointer to the byte array containing the encoded vint.
     * @param dataSize Size of the data buffer in bytes.
     * @param vintw Reference to an unsigned char where the detected width of the vint will be stored.
     * @return The decoded unsigned long long value.
     * @throws ebmlUnexpectedEndOfData if there is no data or if the data is incomplete.
     */
    unsigned long long unpackVint(const char* data, size_t dataSize, unsigned char& vintw);

    /**
     * @brief Decodes a vint from an I/O stream.
     *
     * Reads a vint from the provided file stream. The detected width of the vint is stored in @p vintw.
     *
     * @param file Reference to the I/O file stream.
     * @param vintw Reference to an unsigned char in which the width of the decoded vint will be stored.
     * @return The decoded unsigned long long value. If no data is available, returns 0 and sets @p vintw to 0.
     * @throws ebmlInvalidVint or ebmlUnexpectedEndOfData on errors.
     */
    unsigned long long unpackVint(ioBase& file, unsigned char& vintw);

    /**
     * @brief Decodes a vint from an I/O stream starting at a specified offset.
     *
     * Reads a vint from the provided file stream beginning at the specified offset. The detected width
     * of the vint is stored in @p vintw.
     *
     * @param file Reference to the I/O file stream.
     * @param offset The offset (in bytes) from which to start reading.
     * @param vintw Reference to an unsigned char in which the width of the decoded vint will be stored.
     * @return The decoded unsigned long long value. If no data is available, returns 0 and sets @p vintw to 0.
     * @throws ebmlInvalidVint or ebmlUnexpectedEndOfData on errors.
     */
    unsigned long long unpackVint(ioBase& file, off_t offset, unsigned char& vintw);

} // namespace ebml

#endif
