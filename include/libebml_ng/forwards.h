#ifndef EBML_NG_FORWARDS_H
#define EBML_NG_FORWARDS_H

namespace ebml {
    class ioBase;

    class ebmlSchema;
    class ebmlDocument;

    class ebmlElementClass;
    class ebmlElement;

    // class ebmlElementTypeBase;
    //
    // template<typename ebmltype_t, typename ebmlinst_t>
    // class ebmlElementType;
    //
    // template<typename ebmltype_t, typename ebmlinst_t>
    // class ebmlElement_ALT;

    class ebmlVoidClass;
    class ebmlVoid;

    template<typename T>
    class ebmlDataElementClass;

    template<typename T>
    class ebmlDataElement;

    class ebmlMasterElementClass;
    class ebmlMasterElement;

    class ebmlListClass;
    class ebmlList;

    class ebmlMultiSlotClass;
    class ebmlMultiSlot;

    class ebmlLazyLoadClass;
    class ebmlLazyLoad;

    class seekData_t;
}
#endif
