@page data_type_templates Data Type Templates: Creating Custom EBML Data Types

This page describes how to implement a custom EBML data type using the provided
CRTP templates, along with the helper functions such as `ebml::size()`, `ebml::pack()`, `ebml::unpack()` and `ebml::repr()`. These templates serve as the basis for the concrete type and
element instances used in encoding and decoding EBML data.

@section data_type_overview Overview

The EBML library employs the CRTP (Curiously Recurring Template Pattern) (@ref crtp_usage) to
simplify the process of subclassing `ebml::ebmlElementType` and `ebml::ebmlElement`. In particular, the
templates:

- `ebml::ebmlDataType<T>`: Inherits from a CRTP type base (`ebml::ebmlTypeCRTP`) and provides a
  factory interface and metadata for a given type `T`.
  - The following members are available:
    - `T ebml::ebmlDataType<T>::defaultval` provides the default value for an `ebml::ebmlDataElement<T>` element when none is provided, or an empty element is decoded.
    - `static T ebml::ebmlDataType<T>::defaultdefault` provides a default value for `defaultval` in`ebml::ebmlDataType<T>` element when none is provided.
  - `ebml::ebmlDataElement<T>` instances can be created using these functions:
    - `ebml::ebmlDataType<T>::operator()(const T& data)` constructs an `ebml::ebmlDataElement<T>` using the provided data.
    - `ebml::ebmlDataType<T>::operator()(T&& data)` constructs an `ebml::ebmlDataElement<T>`, moving the provided data into the element.
- `ebml::ebmlDataElement<T>`: Inherits from a CRTP element base (`ebml::ebmlElementCRTP`) and
  encapsulates the actual value of type `T` along with functions for encoding,
  decoding, cloning, and representation.
  - Provides a public data member `T data` representing the payload.

Template instances for the following types are already provided:
- `unsigned long long` (as `ebml::ebmlUnsignedIntegerType` and `ebml::ebmlUnsignedInteger`)
- `long long` (as `ebml::ebmlSignedIntegerType` and `ebml::ebmlSignedInteger`)
- `double` (as `ebml::ebmlFloatType` and `ebml::ebmlFloat`)
- `std::string` (as `ebml::ebmlBinaryType` and `ebml::ebmlBinary`)
- `std::wstring` (as `ebml::ebmlUnicodeType` and `ebml::ebmlUnicode`)
- `ebml::timepoint_t = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>` (as `ebml::ebmlDateTimeType` and `ebml::ebmlDateTime`)

To create a custom data type instance for EBML, you must provide implementations
for the following template functions:

- `ebml::size(const T&)`
  Calculates and returns the number of bytes required to encode a value of type `T`.
  A macro `DEF_SIZE(T)` is provided to ensure a correct function signature.

- `ebml::pack(const T&, char* dest)` or `pack(const T&, size_t, char* dest)`
  Encodes a value of type `T` into the provided destination buffer. The function
  returns the number of bytes written.
  Macros `DEF_PACK(T)` and `DEF_PACK_ALT(T)` are provided to ensure correct function signatures.

- `ebml::unpack<T>(const char*, size_t)`
  Decodes the raw byte buffer back into a value of type `T`.
  A macro `DEF_UNPACK(T)` is provided to ensure a correct function signature.

- `ebml::repr(const T&)`
  Returns a string representation of the `T` value.

These functions are typically defined in the `struct.h` and `repr.h` and implemented in `struct.tpp` and `repr.tpp`.

@note When implementing specializations for `ebml::size()`, `ebml::pack` and `ebml::unpack`, it is important that these specializations are declared **before** including `struct.tpp`.

@section data_impl Implementing a Custom EBML Data Type

To implement a custom EBML data type using these templates, follow these steps:

1. **Implement Template Functions**  
   Provide implementations (or overloads) for the functions:

   - `size_t ebml::size(const T& value)`: Should return the number of bytes needed for the
     encoded representation of `value`. A macro `DEF_SIZE(T)` is provided to ensure
     a correct function signature.

   - Pack functions:
     - `size_t ebml::pack(const T& value, char* dest)` *or*
     - `size_t ebml::pack(const T& value, size_t size, char* dest)`

     Should write the encoded bytes of `value` into the `dest` buffer, returning the number of bytes written
     into `dest`. Macros `DEF_PACK(T)` and `DEF_PACK_ALT(T)` are provided to ensure correct function signatures.

   - `T ebml::unpack<T>(const char* src, size_t size)`: Should read raw data from `src` and
     return the decoded value of type `T`. A macro `DEF_UNPACK(T)` is provided to ensure a correct function signature.

   - `std::wstring ebml::repr(const T&)`:
     Returns a wide string representation of the `T` value.

   For example, the implementation of these functions for `T = long long` is provided
   in the file `ll.h`.

2. **Define a default value.**
   Provide a value for `T ebml::ebmlDataType<T>::defaultdefault`. This can be done using the `INST_EBML_CLS(T, defaultval)` macro.

3. **Define the Data Type and Data Element Subclasses**  
   Use the provided CRTP templates to create your custom data type:

   - Define your data type by instantiating:

     ```c++
     typedef ebml::ebmlDataType<long long> ebmlSignedIntegerType;
     ```

   - Define the corresponding element instance type:

     ```c++
     typedef ebml::ebmlDataElement<long long> ebmlSignedInteger;
     ```

3. **Instantiate and Use Your Custom Type**  
   Once your functions are in place and the template classes are specialized
   (you can use macros like `DECL_DATA_CLS(T)` and `INST_EBML_CLS(T, defaultval)` to assist
   with declarations and instantiations), you can create and manipulate EBML elements
   of your custom type.

   **Example:**

   ```c++
   // Create an instance of the EBML type for signed integers.
   ebmlSignedIntegerType myIntegerType(0x01, L"TestSignedInteger", -42);

   // Use the type as a factory to create an element instance.
   auto element = myIntegerType(-100);

   // Encode the element into a raw data string.
   std::string encoded = element->encode();

   // Decode the raw data back into an element instance.
   auto decodedElement = myIntegerType.decode(encoded);

   // Retrieve the long long value from the element.
   long long value = ebml::data<long long>(decodedElement);
   ```

@section data_type_summary Summary

The approach using CRTP templates in `ebmlDataType<T>` and `ebmlDataElement<T>` allows you
to consistently implement new EBML data types. By implementing the helper functions (`size()`,
`pack()`, and `unpack()`) for your particular data type, you integrate with the EBML framework,
which then provides you with standardized methods for element creation, encoding/decoding,
cloning, and representation.

This design enables a uniform, type-safe interface for working with EBML documents and
simplifies the development of new EBML data types.
/
