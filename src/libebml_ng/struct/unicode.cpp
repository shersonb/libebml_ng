#ifndef EBML_NG_STRUCT_UNICODE_CPP
#define EBML_NG_STRUCT_UNICODE_CPP

#include "libebml_ng/struct/unicode.h"

namespace ebml {
    unsigned long long sizes[4] = {1LL << 7, 1LL << 11, 1LL << 16, 1LL << 21};
    unsigned char fcp[4] = {0b00000000, 0b11000000, 0b11100000, 0b11110000};
    unsigned char fcm[4] = {0b10000000, 0b11100000, 0b11110000, 0b11111000};

    unicode::unicode() : std::u32string() {
        this->planes.bmp = 0;
        this->planes.smp = 0;
        this->planes.sip = 0;
        this->planes.sup = 0;
    }

//     template<>
//     ebmlElementClass<unicode>::ebmlElementClass(ebmlID_t _ebmlID, const std::u32string& _name, const std::u32string& _defaultval) {
//         // stuff
//     }
}
// ebmlElement* ebmlUnicodeClass::operator()() const {
//     return new ebmlUnicode(this);
// }
//
// ebmlUnicode* ebmlUnicodeClass::operator()(const unichar* data, unsigned long long size) const {
//     return new ebmlUnicode(this, data, size);
// }
//
// ebmlUnicode* ebmlUnicodeClass::operator()(unicode& data) const {
//     return new ebmlUnicode(this, data);
// }
//
// ebmlUnicode::ebmlUnicode(const ebmlUnicodeClass* cls) : ebmlElement(cls) {
//     this->data = unicode();
// }
//
// ebmlUnicode::ebmlUnicode(const ebmlUnicodeClass* cls, const unichar* _data, unsigned long long size) : ebmlElement(cls) {
//     this->data = unicode(_data, size);
// }
//
// ebmlUnicode::ebmlUnicode(const ebmlUnicodeClass* cls, unicode& _data) : ebmlElement(cls) {
//     this->data = _data;
// }
//
// unsigned long long ebmlUnicode::dataSize() const {
//     const unichar* data = this->data.c_str();
//     unsigned long long size = this->data.size();
//     unsigned long uk = 0;
//     unichar u;
//     unsigned char j;
//     unsigned long dsize = 0;
//
//     while (uk < size) {
//         u = data[uk];
//
//         if (u > 0x10ffff) {
//             throw ebmlException("Character out of range.", __LINE__, __FILE__);
//         }
//
//         if (0xd800 <= u and u < 0xe000) {
//             throw ebmlException("Surrogate detected.", __LINE__, __FILE__);
//         }
//
//         j = 0;
//
//         while (j < 4) {
//             if (u < sizes[j]) {
//                 dsize += j + 1;
//                 break;
//             }
//
//             j++;
//         }
//
//         uk++;
//     }
//
//     return dsize;
// }
//
// unsigned long long ebmlUnicode::_encode(char* dest, unsigned long long dataSize) const {
//     const unichar* data = this->data.c_str();
//     unsigned long long size = this->data.size();
//     unsigned long uk = 0;
//     unsigned long dk = 0;
//     unichar u;
//     unsigned char j;
//     unsigned char charWidth;
//     unsigned char o;
//
//     while (uk < size) {
//         u = data[uk];
//
//         if (u > 0x10ffff) {
//             throw ebmlException("Character out of range.", __LINE__, __FILE__);
//         }
//
//         if (0xd800 <= u and u < 0xe000) {
//             throw ebmlException("Surrogate detected.", __LINE__, __FILE__);
//         }
//
//         j = 0;
//
//         while (j < 4) {
//             if (u < sizes[j]) {
//                 charWidth = j + 1;
//                 break;
//             }
//
//             j++;
//         }
//
//         o = 6*(charWidth - 1);
//         dest[dk] = (
//             ((u >> o) & ~fcm[charWidth - 1])
//             | fcp[charWidth - 1]);
//
//         o -= 6;
//         j = 1;
//
//         while (j < charWidth) {
//             dest[dk + j] = ((u >> o) & 0b00111111) | 0b10000000;
//             j++;
//             o -= 6;
//         }
//         dk += charWidth;
//         uk++;
//     }
//
//     return dk;
// }
//
// void ebmlUnicode::_decode(const parseStringData& parsed) {
//     this->data.resize(parsed.dataSize);
//     unsigned long uk = 0;
//     unsigned long dk = 0;
//     unichar u;
//     unsigned char d;
//     unsigned char charWidth;
//     unsigned char o;
//     unsigned char m;
//     unsigned char j;
//     unsigned char k;
//
// //     if (parsed.dataSize > data->bufsize) {
// //         data->resize(parsed.dataSize + 32);
// //     }
//
//     while (dk < parsed.dataSize) {
//         d = parsed.data[dk];
//         u = 0;
//         k = 0;
//
//         while (k < 4) {
//             if ((d & fcm[k]) == fcp[k]) {
//                 charWidth = k + 1;
//                 break;
//             }
//
//             k++;
//         }
//
//         o = 6*(charWidth - 1);
//
//         m = ~fcm[charWidth - 1];
//         u = (unichar)(d & m) << o;
//         j = 1;
//         o -= 6;
//
//         while (j < charWidth) {
//             d = parsed.data[dk + j];
//
//             if ((d & 0b11000000) != 0b10000000) {
//                 throw ebmlException("Invalid byte", __LINE__, __FILE__);
//             }
//
//             m = 0b00111111;
//             u |= (unichar)(d & m) << o;
//             j++;
//             o -= 6;
//         }
//
// //         data->data[uk] = u;
// //         this->data.assign(uk, u);
//         this->data.replace(uk, 1, 1, u);
//         dk += charWidth;
//         uk++;
//
//         if (dk > parsed.dataSize) {
//             throw ebmlException("Unexpected end of data.", __LINE__, __FILE__);
//         }
//
// //     data->size = uk;
//     this->data.resize(uk);
//     }
// }
//
// void ebmlUnicode::_clonedata(const ebmlElement* src) {
//     auto recast = static_cast<const ebmlUnicode*>(src);
//     this->data = recast->data;
// }

#endif
