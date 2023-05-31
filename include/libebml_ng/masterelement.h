#ifndef EBML_NG_MASTERELEMENT_H
#define EBML_NG_MASTERELEMENT_H

#include "base.h"
#include <unordered_map>
#include <unordered_set>

struct childClassSpec_t {
    const ebmlElementClass* cls;
    unsigned long minOccur;
    unsigned long maxOccur;
};

typedef std::unordered_map<unsigned long long, childClassSpec_t> childClasses_d;
typedef std::unordered_map<unsigned long long, childClassSpec_t>::iterator childClasses_i;
typedef std::pair<unsigned long long, childClassSpec_t> childClasses_p;

typedef std::unordered_map<unsigned long long, unsigned long long> occur_d;

// Basic set of EBMLELement* items.
typedef std::unordered_set<ebmlElement*> ebmlElement_s;
typedef std::unordered_set<ebmlElement*>::iterator ebmlElement_si;

// Map of EBMLELement* items, keyed by ebmlID.
typedef std::unordered_map<unsigned long long, ebmlElement*> ebmlElement_d;
typedef std::unordered_map<unsigned long long, ebmlElement*>::iterator ebmlElement_di;
typedef std::pair<unsigned long long, ebmlElement*> ebmlElement_p;

// Map of *sets* of EBMLELement* items, keyed by ebmlID.
typedef std::unordered_map<unsigned long long, ebmlElement_s> ebmlElement_sd;
typedef std::unordered_map<unsigned long long, ebmlElement_s>::iterator ebmlElement_sdi;
typedef std::pair<unsigned long long, ebmlElement_s> ebmlElement_sp;

class ebmlMasterElement;

class ebmlMasterElementClass : public ebmlElementClass {
public:
    childClasses_d childClasses;
    ebmlMasterElementClass(unsigned long long, std::string);
    void addChildType(const ebmlElementClass*);
    void addChildType(const ebmlElementClass*, unsigned long long);
    void addChildType(const ebmlElementClass*, unsigned long long, unsigned long long);
    void rmChildType(unsigned long long);
    void rmChildType(const ebmlElementClass*);
    friend class ebmlMasterElement;
};

class ebmlMasterElement: public ebmlElement {
protected:
//     ebmlElement_d _single;
//     ebmlElement_sd _multi;
    ebmlMasterElement(const ebmlMasterElementClass*);
    occur_d ebmlIDCount;
    virtual void _add(ebmlElement*);
    virtual void _remove(ebmlElement*) {};
    unsigned long long _encode(char*, unsigned long long) const;
    void _decode(const parseStringData&);
//     virtual void _decode(const parseFileData&);
    virtual void* _init_begin_iter() const;
    virtual void* _init_end_iter() const;
    virtual void _copy_iter(const void*, void*) const;
    virtual void* _copy_iter(const void*) const;
    virtual void _destroy_iter(void*) const;
    virtual void _increment_iter(void*) const;
    virtual void _decrement_iter(void*) const;
    virtual ebmlElement* _dereference_iter(void*) const;
    virtual bool _iseq_iter(const void*, const void*) const;
public:
    unsigned long long dataSize() const;
    void add(ebmlElement*);
    virtual bool contains(ebmlElement*) const;
    void remove(ebmlElement*) {};
//     ebmlElement* get(unsigned long long);
//     ebmlElement* get(unsigned long long, size_t);
    class iterator {
    private:
        iterator();
        iterator(const ebmlMasterElement*, void*);
        const ebmlMasterElement* elem;
        void* pos;
    public:
        ebmlElement* operator*() const;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator=(const iterator&);
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
        ~iterator();
        friend class ebmlMasterElement;
    };
    iterator begin(void) const;
    iterator end(void) const;
    friend class ebmlMasterElementClass;
};

#endif
