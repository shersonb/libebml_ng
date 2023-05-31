#include "libebml_ng/libebml_ng-masterelement.h"

ebmlMasterElementClass::ebmlMasterElementClass(unsigned long long _ebmlID, std::string _name) :
    ebmlElementClass(_ebmlID, _name) {
this->childClasses = childClasses_d();
};

void ebmlMasterElementClass::addChildType(const ebmlElementClass* cls) {
    if (this->childClasses.count(cls->ebmlID) == 1) {
        if (this->childClasses.at(cls->ebmlID).cls != cls) {
            throw ebmlException("EBML ID collission detected.", __LINE__, __FILE__);
        }

        return;
    }
    this->childClasses[cls->ebmlID] = {cls, 0, 1};
}

// void ebmlMasterElementClass::_create(ebmlElement* elem) const {
//     throw ebmlException("Create function not defined. Please override _create(ebmlElement*).", __LINE__, __FILE__);
// }
// 
// void ebmlMasterElementClass::_destroy(ebmlElement* elem) const {
//     throw ebmlException("Destroy function not defined. Please override _destroy(ebmlElement*).", __LINE__, __FILE__);
// }

// void ebmlMasterElementClass::_init(ebmlElement* elem, const void* initdata) const {
//     throw ebmlException("Init function not defined. Please override _init(ebmlElement*, const void*).", __LINE__, __FILE__);
// }



ebmlMasterElement::ebmlMasterElement(const ebmlMasterElementClass* __cls) :
        ebmlElement(__cls) {
    auto cls = static_cast<const ebmlMasterElementClass*>(__cls);
    childClasses_p pair;
    childClasses_d::const_iterator iterator;
    std::pair<unsigned long long, unsigned long long> pair2;

    pair2.second = 0;
    iterator = cls->childClasses.begin();

    while (iterator != cls->childClasses.end()) {
        pair = *iterator;
        ++iterator;
        pair2.first = pair.first;
        this->ebmlIDCount.insert(pair2);
    }
};


void* ebmlMasterElement::_init_begin_iter() const {
    throw ebmlException("void* _init_begin_iter() not implemented.", __LINE__, __FILE__);    
};

void* ebmlMasterElement::_init_end_iter() const {
    throw ebmlException("void* _init_end_iter() not implemented.", __LINE__, __FILE__);    
};

void* ebmlMasterElement::_copy_iter(const void* data) const {
    throw ebmlException("void* _copy_iter(const void*) not implemented.", __LINE__, __FILE__);    
};

void ebmlMasterElement::_copy_iter(const void* data, void* dest) const {
    throw ebmlException("void* _copy_iter(const void*, void*) not implemented.", __LINE__, __FILE__);    
};

void ebmlMasterElement::_destroy_iter(void* data) const {
    throw ebmlException("void _destroy_iter(void*) not implemented.", __LINE__, __FILE__);    
};

void ebmlMasterElement::_increment_iter(void* data) const {
    throw ebmlException("void _increment_iter(void*) not implemented.", __LINE__, __FILE__);    
};

void ebmlMasterElement::_decrement_iter(void* data) const {
    throw ebmlException("void _preincrement_iter(void*) not implemented.", __LINE__, __FILE__);    
};

ebmlElement* ebmlMasterElement::_dereference_iter(void* data) const {
    throw ebmlException("ebmlElement* _dereference_iter(void*) not implemented.", __LINE__, __FILE__);    
};

bool ebmlMasterElement::_iseq_iter(const void* lhs, const void* rhs) const {
    throw ebmlException("bool _iseq_iter(const void*, const void*) not implemented.", __LINE__, __FILE__);    
};

void ebmlMasterElement::_add(ebmlElement* child) {
    throw ebmlException("void _add(ebmlElement*) not implemented.", __LINE__, __FILE__);
};

// void ebmlMasterElement::_clonedata(ebmlElement* elem) const {
//     ebmlElement* child;
//     ebmlElement* newchild;
// //     ebmlMasterElementClass::iterator i = this->begin(elem);
// //     ebmlMasterElement::iterator i = elem->begin();
//     void* i = this->_init_begin_iter(src.as_ptr);
//     void* e = this->_init_end_iter(src.as_ptr);
// 
//     while (!this->_iseq_iter(i, e)) {
//         child = this->_dereference_iter(i);
//         this->_increment_iter(i);
//         newchild = child->clone();
// 
//         try {
//             (static_cast<const ebmlMasterElementClass*>(elem->cls))->_append(elem, newchild);
//         } catch (...) {
//             delete newchild;
//             throw;
//         }
//     }
// }

void ebmlMasterElement::_decode(const parseStringData& parsed) {
    auto cls = static_cast<const ebmlMasterElementClass*>(this->_cls);
    auto childClasses = cls->childClasses;
    ebmlElement* newchild;
    const ebmlElementClass* childcls;
    childClassSpec_t spec;
    parseStringData subparsed;
    iterParseString i = parsed.begin();

    while (i != parsed.end()) {
        subparsed = *i;
        i++;

        if (childClasses.count(subparsed.ebmlID) == 0) {
            throw ebmlException("EBML ID not found.", __LINE__, __FILE__);
        }

        spec = childClasses.at(subparsed.ebmlID);
        childcls = spec.cls;
        newchild = childcls->decode(subparsed);

        try {
            this->add(newchild);
        } catch (...) {
            delete newchild;
            throw;
        }
    }
}

unsigned long long ebmlMasterElement::dataSize() const {
    unsigned long long size = 0;
    const ebmlElement* child;
    ebmlMasterElement::iterator i = this->begin();
    ebmlMasterElement::iterator e = this->end();

    while (i != e) {
        child = *i;
        ++i;
        size += child->outerSize();
    }

    return size;
};

unsigned long long ebmlMasterElement::_encode(char* dest, unsigned long long dataSize) const {
    unsigned long long offset = 0;
    ebmlElement* child;
    ebmlMasterElement::iterator i = this->begin();
    ebmlMasterElement::iterator e = this->end();

    while (i != e) {
        child = *i;
        i++;
        offset += child->encode(dest + offset);
    }

    return offset;
}

ebmlMasterElement::iterator::iterator() {
    this->elem = NULL;
    this->pos = NULL;
}

ebmlMasterElement::iterator::iterator(const ebmlMasterElement* _elem, void* _pos) {
//     printf("begin iterator::iterator(const ebmlElement*, void*) %llu\n", (unsigned long long)this);
    this->elem = _elem;
    this->pos = _pos;
//     printf("pos: %llu\n", (unsigned long long)_pos);
//     printf("end iterator::iterator(const ebmlElement*, void*) %llu\n", (unsigned long long)this);
}

ebmlMasterElement::iterator::~iterator(){
//     printf("begin iterator::~iterator() %llu\n", (unsigned long long)this);
    if (this->elem != NULL and this->elem != NULL) {
        this->elem->_destroy_iter(this->pos);
        this->elem = NULL;
        this->pos = NULL;
    }
//     printf("end iterator::~iterator() %llu\n", (unsigned long long)this);
}

ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator=(const ebmlMasterElement::iterator& other) {
//     printf("begin iterator::operator=(const iterator&) %llu %llu\n", (unsigned long long)this, (unsigned long long)&other);
    if (this->elem != other.elem) {
        this->elem->_destroy_iter(this->pos);
        this->elem = other.elem;
        this->pos = NULL;
    }

    if (other.pos != NULL) {
        if (this->pos != NULL) {
            this->elem->_copy_iter(other.pos, this->pos);
        } else {
            this->pos = this->elem->_copy_iter(other.pos);
        }
    }
//     printf("end iterator::operator=(const iterator&)\n");
    return *this;
}

ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator++() {
    this->elem->_increment_iter(this->pos);
    return *this;
}

ebmlMasterElement::iterator ebmlMasterElement::iterator::operator++(int) {
//     printf("begin iterator::operator++(int) %llu\n", (unsigned long long)this);
    ebmlMasterElement::iterator old;
    old = *this;
    this->elem->_increment_iter(this->pos);
//     printf("end iterator::operator++(int) %llu\n", (unsigned long long)this);
    return old;
}

bool ebmlMasterElement::iterator::operator==(const iterator& other) const {
    if (this->elem != other.elem) {return false;};
    return this->elem->_iseq_iter(this->pos, other.pos);
}

bool ebmlMasterElement::iterator::operator!=(const iterator& other) const {
    bool ret;
//     printf("begin iterator::operator!=(const iterator&) %llu %llu\n", (unsigned long long)this, (unsigned long long)&other);
    if (this->elem != other.elem) {return true;};
//     printf("AAA\n");
    ret = not this->elem->_iseq_iter(this->pos, other.pos);
//     printf("end iterator::operator!=(const iterator&) %llu %llu %d\n", (unsigned long long)this, (unsigned long long)&other, ret);
    return ret;
}

ebmlElement* ebmlMasterElement::iterator::operator*() const {
//     printf("begin iterator::operator*() %llu\n", (unsigned long long)this);
    return this->elem->_dereference_iter(this->pos);
//     printf("end iterator::operator*() %llu\n", (unsigned long long)this);
}

void ebmlMasterElement::add(ebmlElement* child) {
//     if (child->_parent != NULL) {
//         throw ebmlException("EBML Element already has parent.", __LINE__, __FILE__); 
//     }
// 
//     (static_cast<const ebmlMasterElementClass*>(this->cls))->_append(this, child);
//     child->_parent = this;
}

bool ebmlMasterElement::contains(ebmlElement* child) const {
//     unsigned long long ebmlID = child->cls()->ebmlID;
//     const ebmlMasterElementClass* cls = static_cast<const ebmlMasterElementClass*>(this->_cls);
//     auto spec = childClasses.at(subparsed.ebmlID);
// 
//     if (cls->childClasses.count(ebmlID) == 0 or cls->maxOccur.at(ebmlID) > 1) {
//         return this->_multi.at(ebmlID).count(child) > 0;
//     }
//     return (this->_single.count(ebmlID) > 0 and this->_single.at(ebmlID) == child);
}

ebmlMasterElement::iterator ebmlMasterElement::begin() const {
//     printf("begin()\n");
    return ebmlMasterElement::iterator(this, this->_init_begin_iter());
}

ebmlMasterElement::iterator ebmlMasterElement::end() const {
//     printf("end()\n");
    return ebmlMasterElement::iterator(this, this->_init_end_iter());
}
