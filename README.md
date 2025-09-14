@mainpage libebml_ng

**libebml_ng** is a modern, C++20-native library for parsing and generating EBML (Extensible Binary Meta Language) documents. This project represents a fresh take on EBML tooling, focusing on a clean, extensible, and high-performance design.

The library is built around a core factory pattern that cleanly separates the definition of an EBML element's `type` from the management of its `instance` data. This design makes it simple to extend the library with new EBML element types.

## Key Features

* **Modern C++20:** Leverages new language features to provide a type-safe and idiomatic API.

* **Factory Pattern:** A clean separation of concerns for EBML element types and instances.

* **High Performance:** Optimized for efficient encoding and decoding of large EBML documents.

* **Extensible:** Designed to be easily extended with new custom EBML element types.

## Getting Started

### Prerequisites

* A C++ compiler that supports the C++20 standard (e.g., GCC 10+).

### Installation

The library uses a standard `autotools` build system.

1. Clone the repository:

@verbatim
git clone https://github.com/shersonb/libebml_ng.git
cd libebml_ng
@endverbatim

2. Run the `configure` script. This will check your system for a compatible C++ compiler.

@verbatim
./configure
@endverbatim

3. Compile and install the library:

@verbatim
make
sudo make install
@endverbatim

## Using the Library

The core of `libebml_ng` is its extensibility. In addition to built-in types, you can create your own custom EBML elements by inheriting from the core classes and implementing the necessary methods. For detailed instructions on this process, please refer to the following documentation pages:

* @ref core_concepts

* @ref crtp_usage

* @ref data_type_templates

## Contributing

We welcome contributions! Please feel free to open a new issue or submit a pull request on the GitHub repository.

## License

This project is licensed under the GNU Lesser General Public License Version 3. See the `LICENSE` file for details.

