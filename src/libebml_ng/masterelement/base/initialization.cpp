#ifndef EBML_NG_MASTERELEMENT_CLASS_CPP
#define EBML_NG_MASTERELEMENT_CLASS_CPP

#include "libebml_ng/masterelement/base.h"
#include "libebml_ng/ebmlElementClass.tpp"
#include "libebml_ng/element.tpp"
#include "libebml_ng/vint.h"

namespace ebml {
    // ebmlMasterElementClass::ebmlMasterElementClass(const char* ebmlID, const std::wstring& name, const childClassSpecArg_l& childClasses)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name), _childClasses(childClasses) {}
    //
    // ebmlMasterElementClass::ebmlMasterElementClass(ebmlID_t ebmlID, const std::wstring& name, const childClassSpecArg_l& childClasses)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name), _childClasses(childClasses) {}
    //
    // ebmlMasterElementClass::ebmlMasterElementClass(const char* ebmlID, const std::wstring& name, const childClassSpecArg_l& childClasses, const seekHelper_t* seekHelper)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name, seekHelper), _childClasses(childClasses) {}
    //
    // ebmlMasterElementClass::ebmlMasterElementClass(ebmlID_t ebmlID, const std::wstring& name, const childClassSpecArg_l& childClasses, const seekHelper_t* seekHelper)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name, seekHelper), _childClasses(childClasses) {}
    //
    // ebmlMasterElementClass::ebmlMasterElementClass(const char* ebmlID, const std::wstring& name)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name) {}
    //
    // ebmlMasterElementClass::ebmlMasterElementClass(ebmlID_t ebmlID, const std::wstring& name)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name) {}
    //
    // ebmlMasterElementClass::ebmlMasterElementClass(const char* ebmlID, const std::wstring& name, const seekHelper_t* seekHelper)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name, seekHelper) {}
    //
    // ebmlMasterElementClass::ebmlMasterElementClass(ebmlID_t ebmlID, const std::wstring& name, const seekHelper_t* seekHelper)
    // : ClsMixin<ebmlMasterElementClass, ebmlMasterElement>(ebmlID, name, seekHelper) {}
}
#endif
