@page crtp_usage The CRTP Design Pattern

@brief A comprehensive guide to using the `ebmlTypeCRTP` and `ebmlElementCRTP` mixin templates.

This page provides detailed usage guidelines for implementing your own EBML element types
and instances using the library's Curiously Recurring Template Pattern (CRTP) mixins.

## The Purpose of the CRTP Mixins

The `ebmlTypeCRTP` and `ebmlElementCRTP` templates work together to provide compile-time
type safety and automatic pointer conversions. By inheriting from these templates,
your custom EBML types gain all the necessary decoding, encoding, and instance
creation functionality without the need for manual casting.

## Usage Guidelines

To create a paired EBML element type and instance:

- **Element Type:**
    - Define your element type class (`ebmltype_t`) to inherit from:
    `ebml::ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>` where the default
    `typebase_t` is `ebml::ebmlElementType`.

- **Element Instance:**
    - Define your element instance class (`ebmlinst_t`) to inherit from:
    `ebml::ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>` where the default
    `instbase_t` is `ebml::ebmlElement`.
    - Both `ebml::ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>` and
    `ebml::ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>` must be friend
    classes to `ebmltype_t`.

## Required Implementations

To integrate proper decoding and cloning, the following must be implemented:

- **Construction and Decoding:**
  - A constructor of the form:
    `ebmlinst_t::ebmlinst_t(const ebmltype_t*)`
  - For decoding from an `ebml::parseString`:
    - Either implement:
      - A constructor: `ebmlinst_t::ebmlinst_t(const ebmltype_t*, const ebml::parseString&)`
        *or*
      - A member function: `ebmlinst_t::_init(const ebml::parseString&)`
    - Optionally, add a `_cinit(const ebml::parseString&)` for constructing a **const** instance.

- **Decoding From Files:**
  - (Optional) For direct decoding from `ebml::parseFile` (instead of converting it to
    `ebml::parseString`), implement:
    - Either a constructor: `ebmlinst_t::ebmlinst_t(const ebmltype_t*, const ebml::parseFile&)`
      *or*
    - A member function: `ebmlinst_t::_init(const ebml::parseFile&)`
    - Optionally, implement `_cinit(const ebml::parseFile&)` for decoding into a **const** instance.

- **Cloning:**
  - Either provide a copy constructor: `ebmlinst_t::ebmlinst_t(const ebmlinst_t&)`
    or implement an internal clone method: `ebmlinst_t::_clone(const ebmlinst_t&)`.

- **Encoding Support:**
  - Implement the following member functions (if not already handled by the CRTP mixin):
    - `ebmlinst_t::dataSize(char*)`
    - `ebmlinst_t::_encode(char*)`

