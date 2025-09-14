#ifndef EBML_NG_FORWARDS_H
#define EBML_NG_FORWARDS_H

namespace ebml {
    class ioBase;

    class ebmlSchema;
    class ebmlDocument;

    class ebmlElementType;
    class ebmlElement;

    // class ebmlElementTypeBase;
    //
    // template<typename ebmltype_t, typename ebmlinst_t>
    // class ebmlElementType;
    //
    // template<typename ebmltype_t, typename ebmlinst_t>
    // class ebmlElement_ALT;

    class ebmlVoidType;
    class ebmlVoid;

    template<typename T>
    class ebmlDataType;

    template<typename T>
    class ebmlDataElement;

    class ebmlMasterElementType;
    class ebmlMasterElement;

    class ebmlListType;
    class ebmlList;

    class ebmlStructType;
    class ebmlStruct;

    class ebmlLazyLoadType;
    class ebmlLazyLoad;

    class seekData_t;
}
#endif
