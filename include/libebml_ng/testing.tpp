#ifndef EBML_NG_TESTING_TPP
#define EBML_NG_TESTING_TPP

#include "libebml_ng/struct.h"
#include "libebml_ng/template.tpp"
#include "libebml_ng/repr.h"
#include "libebml_ng/repr.tpp"
#include <string>
#include <iostream>
#include <cassert>

namespace ebml {
    void printhex(const char* buffer, size_t length) {
        for (size_t i = 0; i < length; ++i) {
            printf("0x%02X ", static_cast<unsigned char>(buffer[i]));
        }
        printf("\n");
    }

    void printhex(const char32_t* buffer, size_t length) {
        for (size_t i = 0; i < length; ++i) {
            printf("0x%08X ", buffer[i]);
        }
        printf("\n");
    }

    template<typename T>
    void test_size(const T& value, size_t expected_size) {
        size_t size = size(value);
        assert(size == expected_size);
    };

    template<typename T>
    void test_pack(const T& value, const char* expected_string, size_t expected_size) {
        size_t computed_size = size(value);
        assert(computed_size == expected_size);

        std::unique_ptr<char[]> data(new char[computed_size]);
        size_t encoded_size = pack(value, data.get());
        assert(encoded_size == expected_size);
        assert(memcmp(data.get(), expected_string, encoded_size) == 0);
    };

    template<typename T>
    void test_pack_with_size(const T& value, const char* expected_string, size_t requested_size) {
        std::cout << "Testing " << repr(value) << " with requested size " << repr(requested_size) << std::endl;

        std::unique_ptr<char[]> data(new char[requested_size]);
        size_t encoded_size = pack(value, requested_size, data.get());

        std::cout << "    Output produced: " << repr(std::string(data.get(), encoded_size)) << std::endl;
        std::cout << "    Expected data: " << repr(std::string(expected_string, requested_size)) << std::endl;

//         printhex(data.get(), encoded_size);
//         printf("Expected data: ");
//         printhex(expected_string, requested_size);

        assert(encoded_size == requested_size);
        assert(memcmp(data.get(), expected_string, encoded_size) == 0);
    };

    template<typename T>
    void test_unpack(const char* data, size_t datasize, const T& expected_value) {
        T decoded_value = unpack<T>(data, datasize);
        assert(decoded_value == expected_value);
    };

    template<typename T>
    void test_encode(ebmlID_t ebmlID, const T& value, const char* expected_string, size_t expected_size) {
        ebmlDataElementClass<T>* cls = new ebmlDataElementClass<T>(ebmlID, std::wstring(U"Test Class"));
        std::cout << repr(cls) << std::endl;
        ebmlElement_sp elem = (*cls)(value);
        std::cout << repr(elem) << std::endl;
        size_t size = elem->outerSize();
        printf("Calculated size: %d, expected %d\n", size, expected_size);
        assert(size == expected_size);

        std::unique_ptr<char[]> data(new char[size]);
        size = elem->encode(data.get());
        printf("Encoded size: %d, expected %d\n", size, expected_size);
        printf("Encoded data: ");
        printhex(data.get(), size);
        printf("Expected data: ");
        printhex(expected_string, size);
        assert(size == expected_size);
        assert(memcmp(data.get(), expected_string, expected_size) == 0);
    };

    template<typename T>
    void test_decode(ebmlID_t ebmlID, const char* data, size_t datasize, const T& expected_value) {
        ebmlDataElementClass<T>* cls = new ebmlDataElementClass<T>(ebmlID, std::wstring(U"Test Class"));
        ebmlElement_sp elem = cls->decode(data, datasize);
        auto elem_T = ebml::recast<T>(elem);
        assert(expected_value == elem_T->data);
    };
}
#endif
