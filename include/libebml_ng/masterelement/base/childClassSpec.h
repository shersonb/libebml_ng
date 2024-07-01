#ifndef EBML_NG_CHILDCLASSSPEC_H
#define EBML_NG_CHILDCLASSSPEC_H

// #include "../ebmlID_t.h"
// #include <cstddef>
#include "../../ebmlElementClass.h"
// #include "ebmlMasterElementClass.h"
// #include "../element.h"
// #include "../exceptions.h"
//
#include <unordered_map>
#include <unordered_set>
// #include <deque>
#include <list>
// #include <vector>
// #include <optional>

namespace ebml {
    // Used in the initialization of a new ebmlMasterElementClass (and its c++ subclass) instances.
    struct childClassSpecArg_t {
        childClassSpecArg_t();
        childClassSpecArg_t(const ebmlElementClass*, unsigned long min=0, long max=-1);
        const ebmlElementClass* cls;
        unsigned long min; // Minimum number of occurrences required.
        long max; // Maximum number of occurrences permitted. Negative values interpreted as infinity.
    };

    typedef std::list<childClassSpecArg_t> childClassSpecArg_l;
    typedef std::initializer_list<childClassSpecArg_t> childClassSpecArg_init_l;

    struct occurSpec_t {
        unsigned long min;
        long max;
    };

    typedef std::unordered_map<ebmlID_t, const ebmlElementClass*> ebmlElementClass_d;
    typedef std::unordered_set<const ebmlElementClass*> ebmlElementClass_s;
    typedef std::unordered_map<ebmlID_t, occurSpec_t> occurSpec_d;

    // Child element class specification. Used to specify accepted EBML Element types within a parent element.
    // Also used to look up EBMLElementClass* instances by ebmlID_t

    class childClassSpec_t {
    private:
        ebmlElementClass_d _cls_by_ebmlID;
        ebmlElementClass_s _clss;
        occurSpec_d _occurspec;
        inline void _clear() {
            _cls_by_ebmlID.clear();
            _clss.clear();
            _occurspec.clear();
        }

    public:
        childClassSpec_t();
        childClassSpec_t(childClassSpecArg_init_l);
        childClassSpec_t(const childClassSpecArg_l&);
        childClassSpec_t(const childClassSpec_t&) = default;
        childClassSpec_t(childClassSpec_t&&) = default;

        childClassSpec_t& operator=(childClassSpecArg_init_l);
        childClassSpec_t& operator=(const childClassSpecArg_l&);
        childClassSpec_t& operator=(const childClassSpec_t&) = default;
        childClassSpec_t& operator=(childClassSpec_t&&) = default;

        void add(const childClassSpecArg_t&);
        void add(const ebmlElementClass*, unsigned long min=0, long max=-1);
        void remove(ebmlID_t);

        const ebmlElementClass* operator[](ebmlID_t) const;
        size_t count(ebmlID_t) const;
        size_t count(const ebmlElementClass*) const;
        size_t size() const;

        const occurSpec_d& occurSpec() const;
        const occurSpec_t& occurSpec(ebmlID_t) const;
        const occurSpec_t& occurSpec(const ebmlElementClass*) const;

        bool isValid(const ebmlElement_sp&) const;

        ebmlElementClass_d::const_iterator begin() const;
        ebmlElementClass_d::const_iterator end() const;
    };
}
#endif
