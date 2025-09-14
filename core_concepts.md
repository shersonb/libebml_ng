@page core_concepts Core Concepts: EBML Element Types and Instances

@section public_interface Overview of the Public Interface

The EBML library is designed with a clear separation between an element's metadata and factory functionality (the *type**) 
and its actual data and behavior (the **instance**). This allows you to:

- Create new elements from a high-level factory interface.
- Decode raw EBML data into usable element objects.
- Encode elements back into EBML format.
- Access common information such as the element's EBML ID, name, size, and file location.

The core public interfaces are defined in the abstract classes:

- `ebml::ebmlElementType`
  Provides public member functions such as:
  - `ebml::ebmlElementType::operator()()` to construct an element instance.
  - `ebml::ebmlElementType::decode()` and `ebml::ebmlElementType::cdecode()` for converting raw EBML data into element instances.
  - Public data members exposing metadata (e.g. the EBML ID and element name).

- `ebml::ebmlElement`
  Offers a public interface for:
  - Encoding the element into a binary representation through the `ebml::ebmlElement::encode()` functions.
  - Cloning the element through the `ebml::ebmlElement::clone()` function.
  - Retrieving hierarchical relationships:
    - `ebml::ebmlElement::hasParent()`
    - `ebml::ebmlElement::parent_is_const()`
    - `ebml::ebmlElement::parent()`
    - `ebml::ebmlElement::c_parent()`
    - `ebml::ebmlElement::root()`
    - `ebml::ebmlElement::c_root()`
    - `ebml::ebmlElement::document()`
  - Accessing size and location information;
    - `ebml::ebmlElement::dataSize()`
    - `ebml::ebmlElement::headSize()`
    - `ebml::ebmlElement::sizeWidth()`
    - `ebml::ebmlElement::outerSize()`
    - `ebml::ebmlElement::offsetInParent()`
    - `ebml::ebmlElement::offsetInFile()`
    - `ebml::ebmlElement::dataOffsetInParent()`
    - `ebml::ebmlElement::dataOffsetInFile()`

Notice that the public interface intentionally abstracts away complex internal details. The protected and private member 
functions focus on the implementation details of instance creation, decoding, and memory management.

@section basic_implementation Subclassing: Implementing a Fully Functional EBML Element

To implement a new EBML element, you typically define a subclass pair:
- `ebmltype_t`: A concrete subclass of `ebml::ebmlElementType` that encapsulates metadata and the factory methods.
- `ebmlinst_t`: A concrete subclass of `ebml::ebmlElement` that holds the actual element data.

This section outlines the essential requirements and optional recommendations for successfully implementing these subclasses.


A pair of CRTP templates `ebml::ebmlTypeCRTP<ebmltype_t, ebmlinst_t, typebase_t>` and `ebml::ebmlElementCRTP<ebmltype_t, ebmlinst_t, instbase_t>` are provided to help simplify the process of subclassing `ebml::ebmlElementType` and `ebml::ebmlElement`. See @ref crtp_usage for more information.

@subsection type_implementation_requirements Requirements for `ebmltype_t` (Element Type Subclass)

It is still possible, however, to subclass directly from `ebml::ebmlElementType` and `ebml::ebmlElement`. To do this, in your element type subclass, you must implement the following protected virtual functions:

- **Mandatory Methods:**
  - `ebml::ebmlElement* ebmltype_t::_new() const;`  
    Must create and return a new, default instance of your element instance (`ebmlinst_t`).

  - `ebml::ebmlElement* ebmltype_t::_decode_v(const ebml::parseString&) const;`  
    `ebml::ebmlElement* ebmltype_t::_decode_v(const ebml::parseFile&) const;`  
    These functions must decode raw data (provided as a `ebml::parseString` or a `ebml::parseFile`) into an instance of your element.

  - `ebml::ebmlElement* ebmltype_t::_cdecode_v(const parseString&) const;`  
    `ebml::ebmlElement* ebmltype_t::_cdecode_v(const parseFile&) const;`  
    Similar to the above, these functions decode data into a const instance.

- **Friend Declaration:**  
  In your element instance subclass (`ebmlinst_t`), declare your element type class as a friend. For example:
  ```cpp
  friend class YourEbmlTypeClass; // Replace with your actual type class name.
  ```

- **Optional Enhancements:**  
  Consider overloading `ebmltype_t::operator()` in your `ebmltype_t` subclass to provide convenient factory functions that 
  allow construction of `ebmlinst_t` objects using custom parameters.

@subsection inst_requirements Requirements for `ebmlinst_t` (Element Instance Subclass)

Your element instance subclass must at a minimum implement:

- **Constructor Requirements:**  
  Define a constructor `ebmlinst_t::ebmlinst_t(const ebmltype_t*)` that accepts a pointer to the companion `ebmltype_t` (i.e. an instance of your element type class).  
  This constructor should invoke the protected constructor of `ebml::ebmlElement`.

- **Mandatory Methods:**
  - `size_t ebmlinst_t::dataSize() const;`  
    Must return the size of the element’s data (excluding header).

  - `size_t ebmlinst_t::_encode(char*) const;`  
    Must encode the element (usually its data portion) into the provided buffer.

  - `ebmlElement* ebmlinst_t::_clone() const;`  
    Must create and return a deep copy of the element.

  - `std::wstring minirepr() const;`  
    Must return a concise wide string representation of the element.

- **Optional Overrides:**  
  You may override additional virtual functions to refine encoding behavior, add further validation, or to customize 
  parent–child relationship management beyond the default implementations.

@section core_summary Summary

- The **public interface** of `ebml::ebmlElementType` and `ebml::ebmlElement` provides the essential tools for creating,
  decoding, and encoding EBML elements without exposing the internal complexities.

- To implement a concrete EBML element:
  - Subclass `ebml::ebmlElementType` as <em>ebmltype_t</em> and implement the key virtual methods for instantiation and decoding.
  - Subclass `ebmlElement` as <em>ebmlinst_t</em> and implement the methods for encoding, cloning, and representing the element.
  - Declare your <em>ebmltype_t</em> as a friend of <em>ebmlinst_t</em> to allow controlled access to protected members.
  - Enhance your type class with overloads of `operator()` if you require customized construction logic.

This design encourages a robust, type-safe approach to working with EBML data, making it accessible to both new and experienced C++ developers.

@see @ref crtp_usage
@see @ref data_type_templates
