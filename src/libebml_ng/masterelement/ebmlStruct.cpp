#ifndef EBML_NG_MASTERELEMENT_MULTSLOT_CPP
#define EBML_NG_MASTERELEMENT_MULTSLOT_CPP

#include <utility>
#include <memory>

#include "../struct/unicode.h"
#include "../struct.tpp"
#include "ebmlStruct.h"
#include "ebmlStruct.tpp"

#include "../exceptions.h"
#include "../vint.h"
#include "../ebmlElementType.h"
#include "../ebmlElement.tpp"

namespace ebml {
    slotSpec_t::slotSpec_t(const std::string& name, const childTypeSpecArg_l& childSpec, bool multielem)
    : name(name), _childTypes(childSpec), _multi(multielem) {
        // this->name = name;
        // this->_childTypes = childSpec;
        // this->_multi = multielem;
    }

    slotSpec_t::slotSpec_t() {}

    slotSpec_t::slotSpec_t(const slotSpec_t& other) {
        this->name = other.name;
        this->_childTypes = other._childTypes;
        this->_multi = other._multi;
    }

    slotSpec_t::slotSpec_t(slotSpec_t&& other) {
        this->name = std::move(other.name);
        this->_childTypes = std::move(other._childTypes);
        this->_multi = std::move(other._multi);
    }

    slotSpec_t::~slotSpec_t() {}

    const childTypeSpec_t& slotSpec_t::childTypes() const {
        return this->_childTypes;
    }

    slotSpec_t& slotSpec_t::operator=(const slotSpec_t& other) {
        this->name = other.name;
        this->_childTypes = other._childTypes;
        this->_multi = other._multi;
        return *this;
    }

    slotSpec_t& slotSpec_t::operator=(slotSpec_t&& other) {
        this->name = std::move(other.name);
        this->_childTypes = std::move(other._childTypes);
        this->_multi = std::move(other._multi);
        return *this;
    }

    bool slotSpec_t::multi() const {
        return this->_multi;
    }

    // childTypeSpec_t _consolidateSlotSpec(const slotSpec_l& slotSpecs) {
    //     for (const auto& slotSpec : slotSpecs) {
    //         auto childTypes = slotSpec.childTypes();
    //         this->_slotSpecs.push_back(slotSpec);
    //         const auto& slotSpecRef = this->_slotSpecs.back();
    //         this->_slots_by_name[slotSpec.name] = &slotSpecRef;
    //
    //         for (const auto& childTypeSpec : childTypes) {
    //             const auto& occurSpec = childTypes.occurSpec(childTypeSpec.first);
    //             this->_childTypes.add({childTypeSpec.second, occurSpec.min, occurSpec.max});
    //             this->_slots_by_ebmlID[childTypeSpec.first] = &slotSpecRef;
    //         }
    //     }
    // }

    ebmlStructType::ebmlStructType(const char* ebmlID, const std::wstring& name, const slotSpec_l& slotSpecs)
    : ebmlStructType(unpackVint(ebmlID), name, slotSpecs, &seekHelper) {}

    ebmlStructType::ebmlStructType(const char* ebmlID, const std::wstring& name, const slotSpec_l& slotSpecs, const seekHelper_t* seekHelper)
    : ebmlStructType(unpackVint(ebmlID), name, slotSpecs, seekHelper) {}

    ebmlStructType::ebmlStructType(ebmlID_t ebmlID, const std::wstring& name, const slotSpec_l& slotSpecs)
    : ebmlStructType(ebmlID, name, slotSpecs, &seekHelper) {}

    ebmlStructType::ebmlStructType(ebmlID_t ebmlID, const std::wstring& name, const slotSpec_l& slotSpecs, const seekHelper_t* seekHelper)
    : ebmlTypeCRTP<ebmlStructType, ebmlStruct, ebmlMasterElementType>(ebmlID, name, seekHelper) {
        for (const auto& slotSpec : slotSpecs) {
            this->_slotSpecs.push_back(slotSpec);
        }

        for (const auto& slotSpec : this->_slotSpecs) {
            auto& childTypes = slotSpec.childTypes();
            this->_slots_by_name[slotSpec.name] = &slotSpec;

            for (const auto& childTypeSpec : childTypes) {
                const auto& occurSpec = childTypes.occurSpec(childTypeSpec.first);
                this->_childTypes.add({childTypeSpec.second, occurSpec.min, occurSpec.max});
                this->_slots_by_ebmlID[childTypeSpec.first] = &slotSpec;
            }
        }
    }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls, const slotArg_l& args)
    : ebmlElementCRTP<ebmlStructType, ebmlStruct, ebmlMasterElement>(cls) {
        _validateArgs(args);
        _validateData(args);
        _setData(args);
    }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls, slotArg_l&& args)
    : ebmlStruct(cls) {
        _validateArgs(args);
        _validateData(args);
        _setData(std::move(args));
    }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls, const slotArg_d& kwargs)
    : ebmlStruct(cls) {
        _validateArgs(kwargs);
        _validateData(kwargs);
        _setData(kwargs);
    }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls, slotArg_d&& kwargs)
    : ebmlStruct(cls) {
        _validateArgs(kwargs);
        _validateData(kwargs);
        _setData(std::move(kwargs));
    }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls, const slotArg_l& args, const slotArg_d& kwargs)
    : ebmlStruct(cls) {
        _validateArgs(args, kwargs);
        _validateData(args);
        _validateData(kwargs);
        _setData(args);
        _setData(kwargs);
    }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls, const slotArg_l& args, slotArg_d&& kwargs)
    : ebmlStruct(cls) {
        _validateArgs(args, kwargs);
        _validateData(args);
        _validateData(kwargs);
        _setData(args);
        _setData(std::move(kwargs));
    }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls, slotArg_l&& args, const slotArg_d& kwargs)
    : ebmlStruct(cls) {
        _validateArgs(args, kwargs);
        _validateData(args);
        _validateData(kwargs);
        _setData(std::move(args));
        _setData(kwargs);
    }
    ebmlStruct::ebmlStruct(const ebmlStructType * cls, slotArg_l&& args, slotArg_d&& kwargs)
    : ebmlStruct(cls) {
        _validateArgs(args, kwargs);
        _validateData(args);
        _validateData(kwargs);
        _setData(std::move(args));
        _setData(std::move(kwargs));
    }

    // ebmlStruct_sp ebmlStructType::operator()(const slotArg_l& args) const {
    //     auto elem = new ebmlStruct(this);
    //     auto elem_sp = ebmlStruct_sp(elem);
    //     elem->_validateArgs(args);
    //     elem->_validateData(args);
    //     elem->_setData(args);
    //     return elem_sp;
    // }
    //
    // ebmlStruct_sp ebmlStructType::operator()(slotArg_l&& args) const {
    //     auto elem = new ebmlStruct(this);
    //     auto elem_sp = ebmlStruct_sp(elem);
    //     elem->_validateArgs(args);
    //     elem->_validateData(args);
    //     elem->_setData(std::move(args));
    //     return elem_sp;
    // }
    //
    // ebmlStruct_sp ebmlStructType::operator()(const slotArg_d& kwargs) const {
    //     auto elem = new ebmlStruct(this);
    //     auto elem_sp = ebmlStruct_sp(elem);
    //     elem->_validateArgs(kwargs);
    //     elem->_validateData(kwargs);
    //     elem->_setData(kwargs);
    //     return elem_sp;
    // }
    //
    // ebmlStruct_sp ebmlStructType::operator()(slotArg_d&& kwargs) const {
    //     auto elem = new ebmlStruct(this);
    //     auto elem_sp = ebmlStruct_sp(elem);
    //     elem->_validateArgs(kwargs);
    //     elem->_validateData(kwargs);
    //     elem->_setData(std::move(kwargs));
    //     return elem_sp;
    // }
    //
    // ebmlStruct_sp ebmlStructType::operator()(const slotArg_l& args, const slotArg_d& kwargs) const {
    //     auto elem = new ebmlStruct(this);
    //     auto elem_sp = ebmlStruct_sp(elem);
    //     elem->_validateArgs(args, kwargs);
    //     elem->_validateData(args);
    //     elem->_validateData(kwargs);
    //     elem->_setData(args);
    //     elem->_setData(kwargs);
    //     return elem_sp;
    // }
    //
    // ebmlStruct_sp ebmlStructType::operator()(slotArg_l&& args, slotArg_d&& kwargs) const {
    //     auto elem = new ebmlStruct(this);
    //     auto elem_sp = ebmlStruct_sp(elem);
    //     elem->_validateArgs(args, kwargs);
    //     elem->_validateData(args);
    //     elem->_validateData(kwargs);
    //     elem->_setData(std::move(args));
    //     elem->_setData(std::move(kwargs));
    //     return elem_sp;
    // }

    const std::vector<slotSpec_t>& ebmlStructType::slotSpecs() const {
        return this->_slotSpecs;
    }

    const std::unordered_map<std::string, const slotSpec_t*>& ebmlStructType::slotSpecsByKey() const {
        return this->_slots_by_name;
    }

    const std::unordered_map<ebmlID_t, const slotSpec_t*>& ebmlStructType::slotSpecsByID() const {
        return this->_slots_by_ebmlID;
    }

    const ebmlElementType * ebmlStructType::typeBySlot(const std::string& key) const {
        if (this->_slots_by_name.count(key)) {
            auto slot = this->_slots_by_name.at(key);

            if (slot == nullptr) {
                return nullptr;
            }

            auto& childTypes = slot->childTypes();

            if (childTypes.size()) {
                return childTypes.begin()->second;
            }
        }

        return nullptr;
    }

    const ebmlElementType * ebmlStructType::typeBySlot(size_t index) const {
        if (index < this->_slotSpecs.size()) {
            auto& childTypes = this->_slotSpecs.at(index).childTypes();

            if (childTypes.size()) {
                return childTypes.begin()->second;
            }
        }

        return nullptr;
    }

    void slotArg_t::_check_single() const {
        if (this->_multi) {
            throw ebmlException("attempting single-element access in multi-element slot");
        }
    }

    void slotArg_t::_check_multi() const {
        if (!this->_multi) {
            throw ebmlException("attempting multi-element access in single-element slot");
        }
    }

    slotArg_t::slotArg_t(const ebmlElement_sp& elem) {
        this->_multi = false;
        new (&this->elem) ebmlElement_sp(elem);
    }

    slotArg_t::slotArg_t() {
        this->_multi = false;
        new (&this->elem) ebmlElement_sp();
    }

    slotArg_t::slotArg_t(const std::nullptr_t&) : slotArg_t() {}

    slotArg_t::slotArg_t(ebmlElement_sp&& elem) {
        this->_multi = false;
        new (&this->elem) ebmlElement_sp(std::move(elem));
    }

    slotArg_t::slotArg_t(const ebmlElement_l& elems) {
        this->_multi = true;
        new (&this->elems) ebmlElement_l(elems);
    }

    slotArg_t::slotArg_t(ebmlElement_l&& elems) {
        this->_multi = true;
        new (&this->elems) ebmlElement_l(std::move(elems));
    }

    slotArg_t::slotArg_t(const slotArg_t& other) {
        if (other._multi) {
            new (&this->elems) ebmlElement_l(other.elems);
        } else {
            new (&this->elem) ebmlElement_sp(other.elem);
        }
        this->_multi = other._multi;
    }

    slotArg_t::slotArg_t(slotArg_t&& other) {
        if (other._multi) {
            new (&this->elems) ebmlElement_l(std::move(other.elems));
        } else {
            new (&this->elem) ebmlElement_sp(std::move(other.elem));
        }
        this->_multi = other._multi;
    }

    slotArg_t& slotArg_t::operator=(const slotArg_t& other) {
        if (this->_multi) {
            if (other._multi) {
                this->elems = other.elems;
            } else {
                this->elems.~ebmlElement_l();
                new (&this->elem) ebmlElement_sp(other.elem);
            }
        } else {
            if (other._multi) {
                this->elem.~ebmlElement_sp();
                new (&this->elems) ebmlElement_l(other.elems);
            } else {
                this->elem = other.elem;
            }
        }
        this->_multi = other._multi;
        return *this;
    }

    slotArg_t& slotArg_t::operator=(slotArg_t&& other) {
        if (this->_multi) {
            if (other._multi) {
                this->elems = std::move(other.elems);
            } else {
                this->elems.~ebmlElement_l();
                new (&this->elem) ebmlElement_sp(std::move(other.elem));
            }
        } else {
            if (other._multi) {
                this->elem.~ebmlElement_sp();
                new (&this->elems) ebmlElement_l(std::move(other.elems));
            } else {
                this->elem = std::move(other.elem);
            }
        }
        this->_multi = other._multi;
        return *this;
    }

    slotArg_t& slotArg_t::operator=(const ebmlElement_sp& elem) {
        if (this->_multi) {
            this->elems.~ebmlElement_l();
            new (&this->elem) ebmlElement_sp(elem);
            this->_multi = false;
        } else {
            this->elem = elem;
        }
        return *this;
    }

    slotArg_t& slotArg_t::operator=(ebmlElement_sp&& elem) {
        if (this->_multi) {
            this->elems.~ebmlElement_l();
            new (&this->elem) ebmlElement_sp(std::move(elem));
            this->_multi = false;
        } else {
            this->elem = std::move(elem);
        }
        return *this;
    }

    slotArg_t& slotArg_t::operator=(const ebmlElement_l& elems) {
        if (this->_multi) {
            this->elems = elems;
        } else {
            this->elem.~ebmlElement_sp();
            new (&this->elems) ebmlElement_l(elems);
            this->_multi = true;
        }
        return *this;
    }

    slotArg_t& slotArg_t::operator=(ebmlElement_l&& elems) {
        if (this->_multi) {
            this->elems = std::move(elems);
        } else {
            this->elem.~ebmlElement_sp();
            new (&this->elems) ebmlElement_l(std::move(elems));
            this->_multi = true;
        }
        return *this;
    }

    bool slotArg_t::multi() const {
        return this->_multi;
    }

    slotArg_t::operator ebmlElement_sp&() {
        this->_check_single();
        return this->elem;
    }

    slotArg_t::operator ebmlElement_l&() {
        this->_check_multi();
        return this->elems;
    }

    slotArg_t::operator const ebmlElement_sp&() const {
        this->_check_single();
        return this->elem;
    }

    slotArg_t::operator const ebmlElement_l&() const {
        this->_check_multi();
        return this->elems;
    }

    slotArg_t::~slotArg_t() {
        if (this->_multi) {
            this->elems.~ebmlElement_l();
        } else {
            this->elem.~ebmlElement_sp();
        }
    }



        void _slot_t::_check_single() const {
            if (this->_spec == nullptr) {
                throw std::runtime_error("spec is null");
            } else if (this->_spec->multi()) {
                throw ebmlException("attempting single-element access in multi-element slot");
            }
        }

        void _slot_t::_check_multi() const {
            if (this->_spec == nullptr) {
                throw std::runtime_error("spec is null");
            } else if (!this->_spec->multi()) {
                throw ebmlException("attempting multi-element access in single-element slot");
            }
        }

        _slot_t::_slot_t(const slotSpec_t& spec) {
            this->_spec = &spec;
            if (spec.multi()) {
                new (&this->elems) ebmlElement_l();
            } else {
                new (&this->elem) ebmlElement_sp();
            }
        }
        _slot_t::_slot_t(const slotSpec_t& spec, const ebmlElement_sp& elem) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems.push_back(elem);
            } else {
                this->elem = elem;
            }
        }
        _slot_t::_slot_t(const slotSpec_t& spec, ebmlElement_sp&& elem) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems.push_back(std::move(elem));
            } else {
                this->elem = std::move(elem);
            }
        }

        _slot_t::_slot_t(const slotSpec_t& spec, const ebmlElement_l& elems) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems = elems;
            } else {
                this->elem.~ebmlElement_sp();
                throw ebmlException("cannot assign multiple elements to single-element slot");
            }
        }

        _slot_t::_slot_t(const slotSpec_t& spec, ebmlElement_l&& elems) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems = std::move(elems);
            } else {
                this->elem.~ebmlElement_sp();
                throw ebmlException("cannot assign multiple elements to single-element slot");
            }
        }

        _slot_t::_slot_t() {
            this->_spec = nullptr;
        }
        _slot_t::_slot_t(const _slot_t& other) {
            if (other._spec != nullptr) {
                if (other._spec->multi()) {
                    new (&this->elems) ebmlElement_l(other.elems);
                } else {
                    new (&this->elem) ebmlElement_sp(other.elem);
                }
            }
            this->_spec = other._spec;
        }
        _slot_t::_slot_t(_slot_t&& other) {
            if (other._spec != nullptr) {
                if (other._spec->multi()) {
                    new (&this->elems) ebmlElement_l(std::move(other.elems));
                } else {
                    new (&this->elem) ebmlElement_sp(std::move(other.elem));
                }
            }
            this->_spec = other._spec;
        }
        _slot_t& _slot_t::operator=(const _slot_t& other) {
            if (this == &other) {
                return *this;
            }

            if (this->_spec == nullptr) {
                if (other._spec != nullptr) {
                    if (other._spec->multi()) {
                        new (&this->elems) ebmlElement_l(other.elems);
                    } else {
                        new (&this->elem) ebmlElement_sp(other.elem);
                    }
                }
            } else if (this->_spec->multi()) {
                if (other._spec == nullptr) {
                    this->elems.~ebmlElement_l();
                } else if (other._spec->multi()) {
                    this->elems = other.elems;
                } else {
                    this->elems.~ebmlElement_l();
                    new (&this->elem) ebmlElement_sp(other.elem);
                }
            } else {
                if (other._spec == nullptr) {
                    this->elem.~ebmlElement_sp();
                } else if (other._spec->multi()) {
                    this->elem.~ebmlElement_sp();
                    new (&this->elems) ebmlElement_l(other.elems);
                } else {
                    this->elem = other.elem;
                }
            }

            this->_spec = other._spec;
            return *this;
        };
        _slot_t& _slot_t::operator=(_slot_t&& other) {
            if (this == &other) {
                return *this;
            }

            if (this->_spec == nullptr) {
                if (other._spec != nullptr) {
                    if (other._spec->multi()) {
                        new (&this->elems) ebmlElement_l(std::move(other.elems));
                        other.elems.~ebmlElement_l();
                    } else {
                        new (&this->elem) ebmlElement_sp(std::move(other.elem));
                        other.elem.~ebmlElement_sp();
                    }
                }
            } else if (this->_spec->multi()) {
                if (other._spec == nullptr) {
                    this->elems.~ebmlElement_l();
                } else if (other._spec->multi()) {
                    this->elems = std::move(other.elems);
                } else {
                    this->elems.~ebmlElement_l();
                    new (&this->elem) ebmlElement_sp(std::move(other.elem));
                    other.elem.~ebmlElement_sp();
                }
            } else {
                if (other._spec == nullptr) {
                    this->elem.~ebmlElement_sp();
                } else if (other._spec->multi()) {
                    this->elem.~ebmlElement_sp();
                    new (&this->elems) ebmlElement_l(std::move(other.elems));
                    other.elems.~ebmlElement_l();
                } else {
                    this->elem = std::move(other.elem);
                    other.elem.~ebmlElement_sp();
                }
            }

            this->_spec = std::exchange(other._spec, nullptr);
            return *this;
        };
        _slot_t::~_slot_t() {
            if (this->_spec->multi()) {
                this->elems.~ebmlElement_l();
            } else {
                this->elem.~ebmlElement_sp();
            }
        }

        void _slot_t::_validate(const ebmlElement_sp& elem) const {
            if (!_spec->childTypes().isValid(elem)) {
                std::string errmsg = "cannot add '";
                errmsg += pack(elem->cls().name);
                errmsg += "' object to '";
                errmsg += _spec->name;
                errmsg += "' slot.";
                throw std::invalid_argument(errmsg);
            }
        }

        void _slot_t::_validate(const ebmlElement_l& elems) const {
            for (const auto& elem : elems) {
                this->_validate(elem);
            }
        }

        void _slot_t::validate(const ebmlElement_sp& elem) const {
            this->_check_single();
            this->_validate(elem);
        }

        void _slot_t::validate(const ebmlElement_l& elems) const {
            this->_check_multi();
            this->_validate(elems);
        }

        const slotSpec_t& _slot_t::slotSpec() const {
            return *this->_spec;
        }

        bool _slot_t::multi() const {
            return this->_spec->multi();
        }

        size_t _slot_t::size() const {
            if (this->_spec == nullptr) {
                return -1;
            } else if (this->_spec->multi()) {
                return this->elems.size();
            } else if (this->elem != nullptr) {
                return 1;
            }
            return 0;
        }

        // Single-element access and modification
        _slot_t& _slot_t::operator=(const ebmlElement_sp& other) {
            this->_check_single();
            this->_validate(other);
            this->elem = other;
            return *this;
        }

        _slot_t& _slot_t::operator=(ebmlElement_sp&& other) {
            this->_check_single();
            this->_validate(other);
            this->elem = std::move(other);
            return *this;
        }

        _slot_t::operator ebmlElement_sp() const {
            this->_check_single();
            return this->elem;
        }

        _slot_t::operator ebmlElement_sp&() {
            this->_check_single();
            return this->elem;
        }

        _slot_t::operator const ebmlElement_sp&() const {
            this->_check_single();
            return this->elem;
        }

        _slot_t::operator c_ebmlElement_sp() const {
            this->_check_single();
            return ebml_const_pointer_cast<const ebmlElement>(elem);
        }

        // Mutli-element access and modification
        _slot_t::operator ebmlElement_l&() {
            this->_check_multi();
            return this->elems;
        }

        _slot_t::operator const ebmlElement_l&() const {
            this->_check_multi();
            return this->elems;
        }

        _slot_t::operator c_ebmlElement_l() const {
            this->_check_multi();
            return this->elems;
        }

        ebmlElement_sp& _slot_t::operator[](size_t index) { // Access and modify single element
            this->_check_multi();
            return this->elems[index];
        }

        ebmlElement_sp& _slot_t::at(size_t index) { // Access and modify single element
            this->_check_multi();
            return this->elems.at(index);
        }

        ebmlElement_sp& _slot_t::front() { // Access and modify single element
            this->_check_multi();
            return this->elems.front();
        }

        ebmlElement_sp& _slot_t::back() { // Access and modify single element
            this->_check_multi();
            return this->elems.back();
        }

        ebmlElement_sp& _slot_t::insert(size_t index, const ebmlElement_sp& elem) {
            this->_check_multi();
            this->_validate(elem);
            this->elems.insert(this->elems.begin() + index, elem);
            return this->elems.at(index);
        }

        ebmlElement_sp& _slot_t::insert(size_t index, ebmlElement_sp&& elem) {
            this->_check_multi();
            this->_validate(elem);
            this->elems.insert(this->elems.begin() + index, std::move(elem));
            return this->elems.at(index);
        }

        void _slot_t::erase(size_t index) {
            this->_check_multi();
            this->elems.erase(this->elems.begin() + index);
        }

        ebmlElement_sp& _slot_t::push_back(const ebmlElement_sp& elem) {
            this->_check_multi();
            this->_validate(elem);
            this->elems.push_back(elem);
            return this->back();
        }

        ebmlElement_sp& _slot_t::push_back(ebmlElement_sp&& elem) {
            this->_check_multi();
            this->_validate(elem);
            this->elems.push_back(std::move(elem));
            return this->back();
        }

        ebmlElement_sp& _slot_t::emplace_back(const ebmlElement_sp& elem) {
            this->_check_multi();
            this->_validate(elem);
            this->elems.emplace_back(elem);
            return this->back();
        }

        ebmlElement_sp& _slot_t::emplace_back(ebmlElement_sp&& elem) {
            this->_check_multi();
            this->_validate(elem);
            this->elems.emplace_back(std::move(elem));
            return this->back();
        }

#if RAW
        const ebmlElement* _slot_t::operator[](size_t index) const {
            this->_check_multi();
            return this->elems[index].get();
        }

        const ebmlElement* _slot_t::at(size_t index) const{
            this->_check_multi();
            return this->elems.at(index).get();
        }

        const ebmlElement* _slot_t::front() const {
            this->_check_multi();
            return this->elems.front().get();
        }

        const ebmlElement* _slot_t::back() const {
            this->_check_multi();
            return this->elems.back().get();
        }
#else
        c_ebmlElement_sp _slot_t::operator[](size_t index) const {
            this->_check_multi();
            return std::const_pointer_cast<const ebmlElement>(this->elems[index]);
        }

        c_ebmlElement_sp _slot_t::at(size_t index) const {
            this->_check_multi();
            return std::const_pointer_cast<const ebmlElement>(this->elems.at(index));
        }

        c_ebmlElement_sp _slot_t::front() const {
            this->_check_multi();
            return std::const_pointer_cast<const ebmlElement>(this->elems.front());
        }

        c_ebmlElement_sp _slot_t::back() const {
            this->_check_multi();
            return std::const_pointer_cast<const ebmlElement>(this->elems.back());
        }
#endif
        // operator ebmlElement_l();
        // operator c_ebmlElement_l();

        // Assign multi-element at once
        _slot_t& _slot_t::operator=(const ebmlElement_l& other) {
            this->_check_multi();
            this->_validate(other);
            this->elems = other;
            return *this;
        }

        _slot_t& _slot_t::operator=(ebmlElement_l&& other) {
            this->_check_multi();
            this->_validate(other);
            this->elems = std::move(other);
            return *this;
        }

        std::wstring _slot_t::repr() const {
            std::wstring result = unpack<std::wstring>(this->_spec->name);
            result += L"=";

            const auto& childTypes = this->_spec->childTypes();

            if (this->multi()) {
                if (childTypes.size() == 0) {
                    return result + L"[]";
                } else if (childTypes.size() > 1) {
                    return result + L"[...]";
                }

                unsigned int k = 0;
                result += L"[";

                for (const auto& _elem : this->elems) {
                    if (k > 0) {
                        result += L", ";
                    }
                    result += _elem->minirepr();
                    k++;
                }

                return result + L"]";
            } else if (this->elem != nullptr) {
                if (childTypes.size() > 1) {
                    return result + this->elem->repr();
                }

                return result + this->elem->minirepr();
            }
            return L"";
        }

    // ebmlElement_sp ebmlStructType::operator()(const slot_init_list& init) {
    //     auto elem = new ebmlStruct(this, init);
    //     return std::shared_ptr<ebmlElement>(elem);
    // }

    ebmlStruct::ebmlStruct(const ebmlStructType * cls)
      : ebmlElementCRTP<ebmlStructType, ebmlStruct, ebmlMasterElement>(cls) {
        const auto& slotSpecs = cls->slotSpecs();
        unsigned k = 0;
        unsigned K = slotSpecs.size();
        this->_slots.reserve(K);

        for (const auto& slotSpec : slotSpecs) {
            _slot_t _slot = slotSpec;
            this->_slots.push_back(std::move(_slot));
            auto slot_ptr = &this->_slots[k];
            this->_slots_by_name[slotSpec.name] = slot_ptr;

            for (const auto& pr : slotSpec.childTypes()) {
                this->_slots_by_ebmlID[pr.first] = slot_ptr;
            }

            k++;
        }
    }

    ebmlStruct::ebmlStruct(const ebmlStruct& orig) : ebmlStruct(&orig.cls()) {
        auto iter1 = orig._slots.cbegin();
        auto iter1end = orig._slots.cend();
        auto iter2 = _slots.begin();

        while (iter1 != iter1end) {
            auto& origslot = *iter1;

            if (origslot.multi()) {
                const ebmlElement_l& origelems = origslot;
                ebmlElement_l& elems = *iter2;

                for (auto& origelem : origelems) {
                    auto newelem = origelem->clone();
                    _attachChild(newelem);
                    elems.push_back(std::move(newelem));
                }
                // auto iter = origelems.cbegin();
            } else {
                const ebmlElement_sp& origelem = origslot;
                auto newelem = origelem->clone();
                _attachChild(newelem);
                *iter2 = std::move(newelem);

            }

            ++iter1;
            ++iter2;
        }
    }


    // ebmlStruct::ebmlStruct(const ebmlStructType* cls, const slotArg_l& args, ebmlElement_sp& this_sp) : ebmlStruct(cls) {
    //     this_sp = ebmlElement_sp(this);
    //     this->setData(args);
    // }
    //
    // ebmlStruct::ebmlStruct(const ebmlStructType* cls, slotArg_l&& args, ebmlElement_sp& this_sp) : ebmlStruct(cls) {
    //     this_sp = ebmlElement_sp(this);
    //     this->setData(std::move(args));
    // }
    //
    // ebmlStruct::ebmlStruct(const ebmlStructType* cls, const slotArg_d& kwargs, ebmlElement_sp& this_sp) : ebmlStruct(cls) {
    //     this_sp = ebmlElement_sp(this);
    //     this->setData(kwargs);
    // }
    //
    // ebmlStruct::ebmlStruct(const ebmlStructType* cls, slotArg_d&& kwargs, ebmlElement_sp& this_sp) : ebmlStruct(cls) {
    //     this_sp = ebmlElement_sp(this);
    //     this->setData(std::move(kwargs));
    // }
    //
    // ebmlStruct::ebmlStruct(const ebmlStructType* cls, const slotArg_l& args, const slotArg_d& kwargs, ebmlElement_sp& this_sp) : ebmlStruct(cls) {
    //     this_sp = ebmlElement_sp(this);
    //     this->setData(args, kwargs);
    // }
    //
    // ebmlStruct::ebmlStruct(const ebmlStructType* cls, slotArg_l&& args, slotArg_d&& kwargs, ebmlElement_sp& this_sp) : ebmlStruct(cls) {
    //     this_sp = ebmlElement_sp(this);
    //     this->setData(std::move(args), std::move(kwargs));
    // }

    void ebmlStruct::_validateArgs(const slotArg_l& args) {
        auto N = this->_slots.size();
        auto n = args.size();

        if (n > N) {
            std::string errmsg = "setData() takes ";
            errmsg += std::to_string(N);
            errmsg += " positional arguments but ";
            errmsg += std::to_string(n);
            errmsg += " were given";
            throw std::invalid_argument(errmsg);
        } else if (n == N - 1) {
            std::string errmsg = "setData() missing 1 required argument: '";
            errmsg += this->_slots[n]._spec->name;
            errmsg += "'";
            throw std::invalid_argument(errmsg);
        } else if (n == N - 2) {
            std::string errmsg = "setData() missing 2 required arguments: '";
            errmsg += this->_slots[n]._spec->name;
            errmsg += "' and '";
            errmsg += this->_slots[n+1]._spec->name;
            errmsg += "'";
            throw std::invalid_argument(errmsg);
        } else if (n < N) {
            std::string errmsg = "setData(): missing ";
            errmsg += std::to_string(N - n);
            errmsg += " arguments: ";

            while (n < N - 1) {
                errmsg += "'";
                errmsg += this->_slots[n]._spec->name;
                errmsg += "', ";
                n++;
            }
            errmsg += " and '";
            errmsg += this->_slots[n]._spec->name;
            errmsg += "'";
            throw std::invalid_argument(errmsg);
        }
    }

    void ebmlStruct::_validateArgs(const slotArg_d& kwargs) {
        std::list<const std::string*> missing;
        std::list<const std::string*> extra;

        for (const auto& slot : _slots) {
            const auto& spec = slot.slotSpec();
            const auto& childclss = spec.childTypes();

            if (childclss.size() == 1) {
                auto minOccur = (*childclss.occurSpec().begin()).second.min;

                if (minOccur == 0) {
                    continue;
                }
            }

            if ((kwargs.count(spec.name) == 0)) {
                missing.push_back(&spec.name);
            }
        }

        for (const auto& pr : kwargs) {
            if (this->_slots_by_name.count(pr.first) == 0) {
                extra.push_back(&pr.first);
            }
        }

        switch (missing.size()) {
            case 0: {
                break;
            }
            case 1: {
                std::string errmsg = "setData() missing 1 required argument: '";
                errmsg += *missing.front();
                errmsg += "'";
                throw std::invalid_argument(errmsg);
            }
            case 2: {
                std::string errmsg = "setData() missing 2 required arguments: '";
                errmsg += *missing.front();
                errmsg += "' and '";
                errmsg += *missing.back();
                errmsg += "'";
                throw std::invalid_argument(errmsg);
            }
            default: {
                std::string errmsg = "setData() missing ";
                errmsg += std::to_string(missing.size());
                errmsg += " required arguments: ";

                auto iter = missing.begin();
                auto end = missing.end();
                auto str = *iter;
                errmsg += "'";
                errmsg += *str;
                errmsg += "'";
                ++iter;

                while (iter != end) {
                    str = *iter;
                    ++iter;
                    errmsg += ", ";

                    if (iter == end) {
                        errmsg += "and ";
                    }

                    errmsg += "'";
                    errmsg += *str;
                    errmsg += "'";
                }

                throw std::invalid_argument(errmsg);
            }
        }
        switch (extra.size()) {
            case 0: {
                break;
            }
            case 1: {
                std::string errmsg = "setData() got 1 unexpected argument: '";
                errmsg += *extra.front();
                errmsg += "'";
                throw std::invalid_argument(errmsg);
            }
            case 2: {
                std::string errmsg = "setData() got 2 unexpected arguments: '";
                errmsg += *extra.front();
                errmsg += "' and '";
                errmsg += *extra.back();
                errmsg += "'";
                throw std::invalid_argument(errmsg);
            }
            default: {
                std::string errmsg = "setData() got ";
                errmsg += std::to_string(extra.size());
                errmsg += " unexpected arguments: ";

                auto iter = extra.begin();
                auto end = extra.end();
                auto str = *iter;
                errmsg += "'";
                errmsg += *str;
                errmsg += "'";
                ++iter;

                while (iter != end) {
                    str = *iter;
                    ++iter;
                    errmsg += ", ";

                    if (iter == end) {
                        errmsg += "and ";
                    }

                    errmsg += "'";
                    errmsg += *str;
                    errmsg += "'";
                }

                throw std::invalid_argument(errmsg);
            }
        }
    }

    void ebmlStruct::_validateArgs(const slotArg_l& args, const slotArg_d& kwargs) {
        std::list<const std::string*> missing;
        std::list<const std::string*> extra;
        std::list<const std::string*> multiple;
        auto n = args.size();
        auto N = this->_slots.size();
        unsigned int k = 0;

        if (args.size() > N) {
            std::string errmsg = "setData() takes ";
            errmsg += std::to_string(N);
            errmsg += " positional arguments but ";
            errmsg += std::to_string(args.size());
            errmsg += "were given";
            throw std::invalid_argument(errmsg);
        }

        for (const auto& slot : this->_slots) {
            if (k < n) {
                if (kwargs.count(slot.slotSpec().name) == 1) {
                    multiple.push_back(&slot.slotSpec().name);
                }
            } else {
                if (kwargs.count(slot.slotSpec().name) == 0) {
                    missing.push_back(&slot.slotSpec().name);
                }
            }
            k++;
        }

        for (const auto& pr : kwargs) {
            if (this->_slots_by_name.count(pr.first) == 0) {
                extra.push_back(&pr.first);
            }
        }

        switch (missing.size()) {
            case 0: {
                break;
            }
            case 1: {
                std::string errmsg = "setData() missing 1 required argument: '";
                errmsg += *missing.front();
                errmsg += "'";
                throw std::invalid_argument(errmsg);
            }
            case 2: {
                std::string errmsg = "setData() missing 2 required arguments: '";
                errmsg += *missing.front();
                errmsg += "' and '";
                errmsg += *missing.back();
                throw std::invalid_argument(errmsg);
            }
            default: {
                std::string errmsg = "setData() missing ";
                errmsg += std::to_string(missing.size());
                errmsg += " required arguments: ";

                auto iter = missing.begin();
                auto end = missing.end();
                auto str = *iter;
                errmsg += "'";
                errmsg += *str;
                errmsg += "'";
                ++iter;

                while (iter != end) {
                    str = *iter;
                    ++iter;
                    errmsg += ", ";

                    if (iter == end) {
                        errmsg += "and ";
                    }

                    errmsg += "'";
                    errmsg += *str;
                    errmsg += "'";
                }

                throw std::invalid_argument(errmsg);
            }
        }
        switch (extra.size()) {
            case 0: {
                break;
            }
            case 1: {
                std::string errmsg = "setData() got 1 unexpected argument: '";
                errmsg += *extra.front();
                errmsg += "'";
                throw std::invalid_argument(errmsg);
            }
            case 2: {
                std::string errmsg = "setData() got 2 unexpected arguments: '";
                errmsg += *extra.front();
                errmsg += "' and '";
                errmsg += *extra.back();
                throw std::invalid_argument(errmsg);
            }
            default: {
                std::string errmsg = "setData() got ";
                errmsg += std::to_string(missing.size());
                errmsg += " unexpected arguments: ";

                auto iter = extra.begin();
                auto end = extra.end();
                auto str = *iter;
                errmsg += "'";
                errmsg += *str;
                errmsg += "'";
                ++iter;

                while (iter != end) {
                    str = *iter;
                    ++iter;
                    errmsg += ", ";

                    if (iter == end) {
                        errmsg += "and ";
                    }

                    errmsg += "'";
                    errmsg += *str;
                    errmsg += "'";
                }

                throw std::invalid_argument(errmsg);
            }
        }
        switch (multiple.size()) {
            case 0: {
                break;
            }
            case 1: {
                std::string errmsg = "setData() got multiple values for argument '";
                errmsg += *multiple.front();
                errmsg += "'";
                throw std::invalid_argument(errmsg);
            }
            case 2: {
                std::string errmsg = "setData() got multiple values for arguments '";
                errmsg += *multiple.front();
                errmsg += "' and '";
                errmsg += *multiple.back();
                throw std::invalid_argument(errmsg);
            }
            default: {
                std::string errmsg = "setData() got multiple values for arguments ";

                auto iter = multiple.begin();
                auto end = multiple.end();
                auto str = *iter;
                errmsg += "'";
                errmsg += *str;
                errmsg += "'";
                ++iter;

                while (iter != end) {
                    str = *iter;
                    ++iter;
                    errmsg += ", ";

                    if (iter == end) {
                        errmsg += "and ";
                    }

                    errmsg += "'";
                    errmsg += *str;
                    errmsg += "'";
                }

                throw std::invalid_argument(errmsg);
            }
        }
    }

    void ebmlStruct::_validateData(const slotArg_l& args) {
        auto start = args.begin();
        auto end = args.end();
        auto iter = start;

        auto slot_iter = this->_slots.begin();

        while (iter != end) {
            try {
                const auto& arg = *iter;
                const auto& slot = *slot_iter;

                if (arg.multi()) {
                    const ebmlElement_l& elems = arg;
                    slot.validate(elems);
                    this->_attachChildren(elems);
                } else {
                    const ebmlElement_sp& elem = arg;

                    if (elem != nullptr) {
                        slot.validate(elem);
                        this->_attachChild(elem);
                    }
                }

                ++slot_iter;
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    const auto& arg = *iter;

                    if (arg.multi()) {
                        const ebmlElement_l& elems = arg;

                        for (const auto& elem : elems) {
                            this->_detachChild(elem);
                        }
                    } else {
                        const ebmlElement_sp& elem = arg;

                        if (elem != nullptr) {
                            this->_detachChild(elem);
                        }
                    }

                }
                throw;
            }
        }
    }

    void ebmlStruct::_setData(const slotArg_l& args) {
        auto args_iter = args.begin();
        auto args_end = args.end();
        auto slot_iter = this->_slots.begin();

        while (args_iter != args_end) {
            auto& arg = *args_iter;
            auto& slot = *slot_iter;

            if (arg.multi()) {
                slot.elems = arg;
            } else {
                slot.elem = arg;
            }

            ++slot_iter;
            ++args_iter;
        }
    }

    void ebmlStruct::_setData(slotArg_l&& args) {
        auto args_iter = args.begin();
        auto args_end = args.end();
        auto slot_iter = this->_slots.begin();

        while (args_iter != args_end) {
            auto& arg = *args_iter;
            auto& slot = *slot_iter;

            if (arg.multi()) {
                ebmlElement_l& elems = arg;
                slot.elems = std::move(elems);
            } else {
                ebmlElement_sp& elem = arg;
                slot.elem = std::move(elem);
            }

            ++slot_iter;
            ++args_iter;
        }

        args.clear();
    }

    void ebmlStruct::_validateData(const slotArg_d& kwargs) {
        auto start = kwargs.begin();
        auto end = kwargs.end();
        auto iter = start;

        while (iter != end) {
            try {
                const auto& pr = *iter;
                const auto& slot = *this->_slots_by_name.at(pr.first);
                const auto& arg = pr.second;

                if (arg.multi()) {
                    const ebmlElement_l& elems = arg;
                    slot.validate(elems);
                    this->_attachChildren(elems);
                } else {
                    const ebmlElement_sp& elem = arg;

                    if (elem != nullptr) {
                        slot.validate(elem);
                        this->_attachChild(elem);
                    }
                }

                ++iter;
            } catch (...) {
                while (iter != start) {
                    const auto& pr = *start;
                    const auto& arg = pr.second;

                    if (arg.multi()) {
                        const ebmlElement_l& elems = arg;

                        for (const auto& elem : elems) {
                            this->_detachChild(elem);
                        }
                    } else {
                        const ebmlElement_sp& elem = arg;

                        if (elem != nullptr) {
                            this->_detachChild(elem);
                        }
                    }

                    ++start;
                }
                throw;
            }
        }
    }

    void ebmlStruct::_setData(const slotArg_d& kwargs) {
        auto iter = kwargs.begin();
        auto end = kwargs.end();

        while (iter != end) {
            auto& pr = *iter;
            auto& slot = *this->_slots_by_name.at(pr.first);
            auto& arg = pr.second;

            if (arg.multi()) {
                slot.elems = arg;
            } else {
                slot.elem = arg;
            }

            ++iter;
        }
    }

    void ebmlStruct::_setData(slotArg_d&& kwargs) {
        auto iter = kwargs.begin();
        auto end = kwargs.end();

        while (iter != end) {
            auto& pr = *iter;
            auto& slot = *this->_slots_by_name.at(pr.first);
            auto& arg = pr.second;

            if (arg.multi()) {
                ebmlElement_l& elems = arg;
                slot.elems = std::move(elems);
            } else {
                ebmlElement_sp& elem = arg;
                slot.elem = std::move(elem);
            }

            ++iter;
        }
        kwargs.clear();
    }

    void ebmlStruct::setData(const slotArg_l& args) {
        this->_validateArgs(args);
        this->_validateData(args);
        this->_clear();
        this->_setData(args);
    }

    void ebmlStruct::setData(slotArg_l&& args) {
        this->_validateArgs(args);
        this->_validateData(args);
        this->_clear();
        this->_setData(std::move(args));
    }

    void ebmlStruct::setData(const slotArg_d& kwargs) {
        this->_validateArgs(kwargs);
        this->_validateData(kwargs);
        this->_clear();
        this->_setData(kwargs);
    }

    void ebmlStruct::setData(slotArg_d&& kwargs) {
        this->_validateArgs(kwargs);
        this->_validateData(kwargs);
        this->_clear();
        this->_setData(std::move(kwargs));
    }

    void ebmlStruct::setData(const slotArg_l& args, const slotArg_d& kwargs) {
        this->_validateArgs(args, kwargs);
        this->_validateData(args);
        this->_validateData(kwargs);
        this->_clear();
        this->_setData(args);
        this->_setData(kwargs);
    }

    void ebmlStruct::setData(slotArg_l&& args, slotArg_d&& kwargs) {
        this->_validateArgs(args, kwargs);
        this->_validateData(args);
        this->_validateData(kwargs);
        this->_clear();
        this->_setData(std::move(args));
        this->_setData(std::move(kwargs));
    }

    void ebmlStruct::_clear() {
        for (auto& slot : this->_slots) {
            if (slot.multi()) {
                ebmlElement_l& elems = slot;

                for (auto& elem : elems) {
                    this->_detachChild(elem);
                }

                elems.clear();
            } else {
                ebmlElement_sp& elem = slot;

                if (elem != nullptr) {
                    this->_detachChild(elem);
                }

                elem = nullptr;
            }
        }
    }

    std::wstring ebmlStruct::minirepr() const {
        std::wstring result = L"{";
        unsigned int k = 0;

        for (auto& slot : this->_slots) {
            auto s = slot.repr();

            if (s.size() > 0) {
                if (k > 0) {
                    result += L", ";
                }
                result += s;
                k++;
            }
        }

        return result + L"}";
    }

    void ebmlStruct::_addChild(const ebmlElement_sp& child) {
        auto& slot = *this->_slots_by_ebmlID.at(child->ebmlID());

        if (slot.multi()) {
            ebmlElement_l& elems = slot;
            elems.push_back(child);
        } else {
            ebmlElement_sp& elem = slot;
            elem  = child;
        }
    }

    void ebmlStruct::_addChild(ebmlElement_sp&& child) {
        auto& slot = *this->_slots_by_ebmlID.at(child->ebmlID());

        if (slot.multi()) {
            ebmlElement_l& elems = slot;
            elems.push_back(std::move(child));
        } else {
            ebmlElement_sp& elem = slot;
            elem  = child;
        }
    }

    ebmlMasterElement::_iterator* ebmlStruct::_begin() {
        auto this_sp = sp();
        auto slotiter = _slots.begin();
        auto slotiterend = _slots.end();
        slot_t slot = {this_sp, *slotiter};
        auto iter = slot.begin();
        auto iterend = slot.end();

        while ((iter == iterend) and (slotiter != slotiterend)) {
            ++slotiter;
            slot._slot = &*slotiter;
            iter = slot.begin();
            iterend = slot.end();
        }

        return new ebmlStruct::_iterator(std::move(this_sp), std::move(slotiter), std::move(slotiterend), std::move(iter), std::move(iterend));
    }

    ebmlMasterElement::_iterator* ebmlStruct::_end() {
        auto this_sp = sp();
        auto slotiter = _slots.end();
        auto slotiterend = slotiter;
        slot_t::iterator iter;
        slot_t::iterator iterend;
        return new ebmlStruct::_iterator(std::move(this_sp), std::move(slotiter), std::move(slotiterend), std::move(iter), std::move(iterend));
    }

    ebmlMasterElement::_const_iterator* ebmlStruct::_cbegin() const {
        auto this_sp = sp();
        auto slotiter = this->_slots.cbegin();
        auto slotiterend = this->_slots.cend();
        const_slot_t slot = {this_sp, *slotiter};
        auto iter = slot.cbegin();
        auto iterend = slot.cend();
        unsigned int k = 0;

        while (iter == iterend) {
            ++slotiter;

            if (slotiter == slotiterend) {
                break;
            }

            slot._slot = &*slotiter;
            iter = slot.cbegin();
            iterend = slot.cend();
            ++k;
        }

        return new ebmlStruct::_const_iterator(std::move(this_sp), std::move(slotiter), std::move(slotiterend), std::move(iter), std::move(iterend));
    }

    ebmlMasterElement::_const_iterator* ebmlStruct::_cend() const {
        auto this_sp = sp();
        auto slotiter = this->_slots.cend();
        auto slotiterend = slotiter;
        const_slot_t::iterator iter;
        const_slot_t::iterator iterend;
        return new ebmlStruct::_const_iterator(std::move(this_sp), std::move(slotiter), std::move(slotiterend), std::move(iter), std::move(iterend));
    }

    slot_t ebmlStruct::operator[](size_t offset) {
        if (offset >= this->_slots.size()) {
            std::string errmsg = "slot index '";
            errmsg += std::to_string(offset);
            errmsg += "' out of range";
            throw ebmlException(errmsg);
        }

        return {sp(), _slots[offset]};
    }

    slot_t ebmlStruct::operator[](const std::string& key) {
        if (this->_slots_by_name.count(key) == 0) {
            std::string errmsg = "key '";
            errmsg += key;
            errmsg += "' not found";
            throw ebmlException(errmsg);
        }

        return {sp(), *_slots_by_name[key]};
    }

    const_slot_t ebmlStruct::operator[](size_t offset) const {
        if (offset >= this->_slots.size()) {
            std::string errmsg = "slot index '";
            errmsg += std::to_string(offset);
            errmsg += "' out of range";
            throw ebmlException(errmsg);
        }

        return {sp(), this->_slots[offset]};
    }

    const_slot_t ebmlStruct::operator[](const std::string& key) const {
        if (this->_slots_by_name.count(key) == 0) {
            std::string errmsg = "key '";
            errmsg += key;
            errmsg += "' not found";
            throw ebmlException(errmsg);
        }

        return {sp(), *_slots_by_name.at(key)};
    }

    slot_t::slot_t(const ebml::ptr<ebmlStruct>& parent, _slot_t& slot) :_parent(parent), _slot(&slot) {}
    slot_t::slot_t(ebml::ptr<ebmlStruct>&& parent, _slot_t& slot) :_parent(std::move(parent)), _slot(&slot) {}
    slot_t::slot_t(const slot_t& other) :_parent(other._parent), _slot(other._slot) {}
    slot_t::slot_t(slot_t&& other) :_parent(std::move(other._parent)), _slot(std::exchange(other._slot, nullptr)) {}

    slot_t& slot_t::operator=(const slot_t& other) {
        if (this != &other) {
            this->_parent = other._parent;
            this->_slot = other._slot;
        }
        return *this;
    }

    slot_t& slot_t::operator=(slot_t&& other) {
        if (this != &other) {
            this->_parent = std::move(other._parent);
            this->_slot = std::exchange(other._slot, nullptr);
        }
        return *this;
    }

    // Single-element access
    slot_t& slot_t::operator=(const ebmlElement_sp& elem) {
        const ebmlElement_sp& old = *this->_slot;
        this->_parent->_attachChild(elem);

        if (old != nullptr) {
            this->_parent->_detachChild(old);
        }

        *this->_slot = elem;
        return *this;
    }

    slot_t& slot_t::operator=(ebmlElement_sp&& elem) {
        const ebmlElement_sp& old = *this->_slot;
        this->_parent->_attachChild(elem);

        if (old != nullptr) {
            this->_parent->_detachChild(old);
        }

        *this->_slot = std::move(elem);
        return *this;
    }

    slot_t::operator const ebmlElement_sp&() {
        const ebmlElement_sp& ret = *this->_slot;
        return ret;
    }

    slot_t::operator c_ebmlElement_sp() const {
        ebml::ptr<const ebmlElement> ret = *_slot;
        return ret;
    }

    // Multi-element access
    slot_t& slot_t::operator=(const ebmlElement_l& elems) {
        const ebmlElement_l& old = *this->_slot;
        this->_parent->_attachChildren(elems);
        this->_parent->_detachChildren(old);
        *this->_slot = elems;
        return *this;
    }

    slot_t& slot_t::operator=(ebmlElement_l&& elems) {
        const ebmlElement_l& old = *this->_slot;
        this->_parent->_attachChildren(elems);
        this->_parent->_detachChildren(old);
        *this->_slot = std::move(elems);
        return *this;
    }

    slot_t::operator const ebmlElement_l&() {
        const ebmlElement_l& ret = *this->_slot;
        return ret;
    }

    slot_t::operator c_ebmlElement_l() const {
        const ebmlElement_l& ret = *this->_slot;
        return ret;
    }

    childSlot_t<> slot_t::operator[](size_t offset) {
        ebmlElement_sp& elem = (*this->_slot)[offset];
        return {this->_parent.get(), this->_slot->slotSpec().childTypes(), elem, false};
    }

    childSlot_t<> slot_t::push_back(const ebmlElement_sp& elem) {
        if (elem == nullptr) {
            throw ebmlException("Cannot push_back nullptr");
        }
        this->_parent->_addChild(elem);
        ebmlElement_sp& pos = this->_slot->push_back(elem);
        return {this->_parent.get(), this->_slot->slotSpec().childTypes(), pos, false};
    }

    childSlot_t<> slot_t::push_back(ebmlElement_sp&& elem) {
        if (elem == nullptr) {
            throw ebmlException("Cannot push_back nullptr");
        }
        this->_parent->_addChild(elem);
        ebmlElement_sp& pos = this->_slot->push_back(std::move(elem));
        return {this->_parent.get(), this->_slot->slotSpec().childTypes(), pos, false};
    }

    size_t slot_t::size() const {
        return this->_slot->size();
    }

#if RAW
    const ebmlElement* slot_t::operator[](size_t offset) const {
        ebmlElement_sp& elem = (*this->_slot)[offset];
        return elem.get();
    }

    const ebmlElement* slot_t::at(size_t offset) const {
        ebmlElement_sp& elem = this->_slot->at(offset);
        return elem.get();
    }

    const ebmlElement* slot_t::front() const {
        ebmlElement_sp& elem = this->_slot->front();
        return elem.get();
    }

    const ebmlElement* slot_t::back() const {
        ebmlElement_sp& elem = this->_slot->back();
        return elem.get();
    }

#else
    c_ebmlElement_sp slot_t::operator[](size_t offset) const {
        ebmlElement_sp& elem = (*this->_slot)[offset];
        return std::const_pointer_cast<const ebmlElement>(elem);
    }

    c_ebmlElement_sp slot_t::at(size_t offset) const {
        ebmlElement_sp& elem = this->_slot->at(offset);
        return std::const_pointer_cast<const ebmlElement>(elem);
    }

    c_ebmlElement_sp slot_t::front() const {
        ebmlElement_sp& elem = this->_slot->front();
        return std::const_pointer_cast<const ebmlElement>(elem);
    }

    c_ebmlElement_sp slot_t::back() const {
        ebmlElement_sp& elem = this->_slot->back();
        return std::const_pointer_cast<const ebmlElement>(elem);
    }
#endif

    slot_t::iterator::iterator() {
        this->_slot = nullptr;
        this->_done = true;
    }

    bool slot_t::iterator::multi() const {
        return (this->_slot != nullptr) and this->_slot->multi();
    }

    slot_t::iterator::iterator(_slot_t& slot, bool done) {
        if (slot.multi()) {
            throw ebmlException("attempting single-element access in multi-element slot");
        }

        this->_slot = &slot;
        this->_done = done;
    }

    slot_t::iterator::iterator(_slot_t& slot, const ebmlElement_l::iterator& iter) {
        if (!slot.multi()) {
            throw ebmlException("attempting multi-element access in single-element slot");
        }

        this->_slot = &slot;
        this->_iter = iter;
    }

    slot_t::iterator::iterator(_slot_t& slot, ebmlElement_l::iterator&& iter) {
        if (!slot.multi()) {
            throw ebmlException("attempting multi-element access in single-element slot");
        }

        this->_slot = &slot;
        this->_iter = std::move(iter);
    }

    slot_t::iterator::~iterator() {
        using v_iterator = typename ebmlElement_l::iterator;
        if ((this->_slot != nullptr) and (this->_slot->multi())) {
            this->_iter.~v_iterator();
        }
    }

    slot_t::iterator::iterator(const slot_t::iterator& other) {
        this->_slot = other._slot;

        if ((this->_slot != nullptr) and this->_slot->multi()) {
            this->_iter = other._iter;
        } else {
            this->_done = other._done;
        }
    }

    slot_t::iterator::iterator(slot_t::iterator&& other) {
        using v_iterator = typename ebmlElement_l::iterator;
        this->_slot = std::exchange(other._slot, nullptr);

        if ((this->_slot != nullptr) and this->_slot->multi()) {
            this->_iter = std::move(other._iter);
            other._iter.~v_iterator();
        } else {
            this->_done = std::exchange(other._done, false);
        }
    }

    slot_t::iterator& slot_t::iterator::operator=(const slot_t::iterator& other) {
        using v_iterator = typename ebmlElement_l::iterator;

        if (this != &other) {
            if ((other._slot != nullptr) and other._slot->multi()) {
                if ((this->_slot != nullptr) and this->_slot->multi()) {
                    this->_iter = other._iter;
                } else {
                    new (&this->_iter) v_iterator(other._iter);
                }
            } else {
                if (this->_slot->multi()) {
                    this->_iter.~v_iterator();
                }
                this->_done = other._done;
            }
            this->_slot = other._slot;
        }
        return *this;
    }

    slot_t::iterator& slot_t::iterator::operator=(slot_t::iterator&& other) {
        using v_iterator = typename ebmlElement_l::iterator;

        if (this != &other) {
            if ((other._slot != nullptr) and other._slot->multi()) {
                if ((this->_slot != nullptr) and this->_slot->multi()) {
                    this->_iter = std::move(other._iter);
                } else {
                    new (&this->_iter) v_iterator(std::move(other._iter));
                }
                other._iter.~v_iterator();
            } else {
                if (this->_slot->multi()) {
                    this->_iter.~v_iterator();
                }
                this->_done = std::exchange(other._done, false);
            }
            this->_slot = std::exchange(other._slot, nullptr);
        }
        return *this;
    }

    slot_t::iterator& slot_t::iterator::operator++() {
        if (this->_slot->multi()) {
            ++this->_iter;
        } else {
            this->_done = true;
        }
        return *this;
    }

    slot_t::iterator slot_t::iterator::operator++(int) {
        slot_t::iterator copy = *this;
        ++(*this);
        return copy;
    }

    const ebmlElement_sp& slot_t::iterator::operator*() const {
        if (this->_slot->multi()) {
            return *this->_iter;
        }
        return this->_slot->elem;
    }

    bool slot_t::iterator::operator==(const slot_t::iterator& other) const {
        if (this->_slot != other._slot) {
            return false;
        }
        if (this->_slot == nullptr) {
            return true;
        }
        if (this->_slot->multi()) {
            return this->_iter == other._iter;
        }
        return this->_done == other._done;
    }

    slot_t::iterator slot_t::begin() {
        if (this->_slot->multi()) {
            auto iter = this->_slot->elems.begin();
            return slot_t::iterator(*this->_slot, std::move(iter));
        } else if (this->_slot->elem != nullptr) {
            return slot_t::iterator(*this->_slot, false);
        }
        return slot_t::iterator(*this->_slot, true);
    }

    slot_t::iterator slot_t::end() {
        if (this->_slot->multi()) {
            auto iter = this->_slot->elems.end();
            return slot_t::iterator(*this->_slot, std::move(iter));
        }
        return slot_t::iterator(*this->_slot, true);
    }

    const_slot_t::const_slot_t(const ebml::ptr<const ebmlStruct>& parent, const _slot_t& slot) :_parent(parent), _slot(&slot) {}
    const_slot_t::const_slot_t(ebml::ptr<const ebmlStruct>&& parent, const _slot_t& slot) :_parent(std::move(parent)), _slot(&slot) {}

    const_slot_t::const_slot_t(const const_slot_t& other) :_parent(other._parent), _slot(other._slot) {}
    const_slot_t::const_slot_t(const_slot_t&& other) :_parent(std::move(other._parent)), _slot(std::exchange(other._slot, nullptr)) {}

    const_slot_t::const_slot_t(const slot_t& other) :
        _parent(other._parent), _slot(other._slot) {}
    const_slot_t::const_slot_t(slot_t&& other) :
        _parent(ebml_const_pointer_cast<const ebmlStruct>(std::move(other._parent))), _slot(std::exchange(other._slot, nullptr)) {}

    const_slot_t& const_slot_t::operator=(const const_slot_t& other) {
        if (this != &other) {
            this->_parent = other._parent;
            this->_slot = other._slot;
        }
        return *this;
    }

    const_slot_t& const_slot_t::operator=(const_slot_t&& other) {
        if (this != &other) {
            this->_parent = std::move(other._parent);
            this->_slot = std::exchange(other._slot, nullptr);
        }
        return *this;
    }

    const_slot_t& const_slot_t::operator=(const slot_t& other) {
        this->_parent = other._parent;
        this->_slot = other._slot;
        return *this;
    }

    const_slot_t& const_slot_t::operator=(slot_t&& other) {
        this->_parent = std::move(other._parent);
        this->_slot = std::exchange(other._slot, nullptr);
        return *this;
    }

    const_slot_t::operator c_ebmlElement_sp() const {
        c_ebmlElement_sp ret = *this->_slot;
        return ret;
    }

    const_slot_t::operator c_ebmlElement_l() const {
        c_ebmlElement_l ret = *this->_slot;
        return ret;
    }

#if RAW
    const ebmlElement* const_slot_t::operator[](size_t offset) const {
        return (*this->_slot)[offset];
    }
#else
    c_ebmlElement_sp const_slot_t::operator[](size_t offset) const {
        const auto& elem = (*this->_slot)[offset];
        return elem;
    }
#endif

    const_slot_t::iterator::iterator() {
        this->_slot = nullptr;
        this->_done = true;
    }

    const_slot_t::iterator::iterator(const _slot_t& slot, bool done) {
        if (slot.multi()) {
            throw ebmlException("attempting single-element access in multi-element slot");
        }

        this->_slot = &slot;
        this->_done = done;
    }

    const_slot_t::iterator::iterator(const _slot_t& slot, const ebmlElement_l::const_iterator& iter) {
        if (!slot.multi()) {
            throw ebmlException("attempting multi-element access in single-element slot");
        }

        this->_slot = &slot;
        this->_iter = iter;
    }

    const_slot_t::iterator::iterator(const _slot_t& slot, ebmlElement_l::const_iterator&& iter) {
        if (!slot.multi()) {
            throw ebmlException("attempting multi-element access in single-element slot");
        }

        this->_slot = &slot;
        this->_iter = std::move(iter);
    }

    const_slot_t::iterator::iterator(const const_slot_t::iterator& other) {
        this->_slot = other._slot;

        if (this->_slot->multi()) {
            this->_iter = other._iter;
        } else {
            this->_done = other._done;
        }
    }

    const_slot_t::iterator::iterator(const_slot_t::iterator&& other) {
        using cv_iterator = typename ebmlElement_l::const_iterator;
        this->_slot = std::exchange(other._slot, nullptr);

        if (this->multi()) {
            this->_iter = std::move(other._iter);
            other._iter.~cv_iterator();
        } else {
            this->_done = std::exchange(other._done, false);
        }
    }

    bool const_slot_t::iterator::multi() const {
        return (this->_slot != nullptr) and this->_slot->multi();
    }

    const_slot_t::iterator::~iterator() {
        using cv_iterator = typename ebmlElement_l::const_iterator;
        if (this->multi()) {
            this->_iter.~cv_iterator();
        }
    }

    const_slot_t::iterator& const_slot_t::iterator::operator=(const const_slot_t::iterator& other) {
        using cv_iterator = typename ebmlElement_l::const_iterator;

        if (this != &other) {
            if (other.multi()) {
                if (this->multi()) {
                    this->_iter = other._iter;
                } else {
                    new (&this->_iter) cv_iterator(other._iter);
                }
            } else {
                if (this->multi()) {
                    this->_iter.~cv_iterator();
                }
                this->_done = other._done;
            }
            this->_slot = other._slot;
        }
        return *this;
    }

    const_slot_t::iterator& const_slot_t::iterator::operator=(const_slot_t::iterator&& other) {
        using cv_iterator = typename ebmlElement_l::const_iterator;

        if (this != &other) {
            if (other.multi()) {
                if (this->multi()) {
                    this->_iter = std::move(other._iter);
                } else {
                    new (&this->_iter) cv_iterator(std::move(other._iter));
                }
                other._iter.~cv_iterator();
            } else {
                if (this->_slot->multi()) {
                    this->_iter.~cv_iterator();
                }
                this->_done = std::exchange(other._done, false);
            }
            this->_slot = std::exchange(other._slot, nullptr);
        }
        return *this;
    }

    const_slot_t::iterator& const_slot_t::iterator::operator++() {
        if (this->multi()) {
            ++this->_iter;
        } else {
            this->_done = true;
        }
        return *this;
    }

    const_slot_t::iterator const_slot_t::iterator::operator++(int) {
        const_slot_t::iterator copy = *this;
        ++(*this);
        return copy;
    }

#if RAW
    const ebmlElement* const_slot_t::iterator::operator*() const {
        if (this->multi()) {
            return (*this->_iter).get();
        }
        return this->_slot->elem.get();
    }
#else
    c_ebmlElement_sp const_slot_t::iterator::operator*() const {
        if (this->multi()) {
            auto& elem = *this->_iter;
            return std::const_pointer_cast<const ebmlElement>(elem);
        }
        auto& elem = this->_slot->elem;
        return std::const_pointer_cast<const ebmlElement>(elem);
    }
#endif

    bool const_slot_t::iterator::operator==(const const_slot_t::iterator& other) const {
        if (this->_slot != other._slot) {
            return false;
        }
        if (this->_slot == nullptr) {
            return true;
        }
        if (this->multi()) {
            return this->_iter == other._iter;
        }
        return this->_done == other._done;
    }

    const_slot_t::iterator const_slot_t::cbegin() const {
        if (this->_slot->multi()) {
            auto iter = this->_slot->elems.cbegin();
            return const_slot_t::iterator(*this->_slot, std::move(iter));
        } else if (this->_slot->elem != nullptr) {
            return const_slot_t::iterator(*this->_slot, false);
        }
        return const_slot_t::iterator(*this->_slot, true);
    }

    const_slot_t::iterator const_slot_t::cend() const {
        if (this->_slot->multi()) {
            auto iter = this->_slot->elems.cend();
            return const_slot_t::iterator(*this->_slot, std::move(iter));
        }
        return const_slot_t::iterator(*this->_slot, true);
    }

    const_slot_t::iterator slot_t::cbegin() const {
        if (this->_slot->multi()) {
            auto iter = this->_slot->elems.cbegin();
            return const_slot_t::iterator(*this->_slot, std::move(iter));
        } else if (this->_slot->elem != nullptr) {
            return const_slot_t::iterator(*this->_slot, false);
        }
        return const_slot_t::iterator(*this->_slot, true);
    }

    const_slot_t::iterator slot_t::cend() const {
        if (this->_slot->multi()) {
            auto iter = this->_slot->elems.cend();
            return const_slot_t::iterator(*this->_slot, std::move(iter));
        }
        return const_slot_t::iterator(*this->_slot, true);
    }

    ebmlStruct::_iterator::_iterator() : _elem(nullptr) {}
    ebmlStruct::_iterator::~_iterator() {}

    ebmlStruct::_iterator::_iterator(
        const ebml::ptr<ebmlStruct>& elem,
        const std::vector<_slot_t>::iterator& slotiter, const std::vector<_slot_t>::iterator& slotiterend,
        const slot_t::iterator& iter, const slot_t::iterator& iterend) :
        _elem(elem), _slotiter(slotiter), _slotiterend(slotiterend), _iter(iter), _iterend(iterend) {}

    ebmlStruct::_iterator::_iterator(
        ebml::ptr<ebmlStruct>&& elem,
        std::vector<_slot_t>::iterator&& slotiter, std::vector<_slot_t>::iterator&& slotiterend,
        slot_t::iterator&& iter, slot_t::iterator&& iterend) :
        _elem(std::move(elem)), _slotiter(std::move(slotiter)), _slotiterend(std::move(slotiterend)),
        _iter(std::move(iter)), _iterend(std::move(iterend)) {}

    ebmlMasterElement::_iterator* ebmlStruct::_iterator::copy() const {
        return new ebmlStruct::_iterator(this->_elem, this->_slotiter, this->_slotiterend, this->_iter, this->_iterend);
    }

    ebmlElement& ebmlStruct::_iterator::operator*() const {
        return **this->_iter;
    }

    ebmlMasterElement::_iterator& ebmlStruct::_iterator::operator++() {
        ++this->_iter;

        while (this->_iter == this->_iterend) {
            ++this->_slotiter;

            if (this->_slotiter == this->_slotiterend) {
                break;
            }

            auto& slot = *this->_slotiter;

            if (slot.multi()) {
                this->_iter = slot_t::iterator(slot, slot.elems.begin());
                this->_iterend = slot_t::iterator(slot, slot.elems.end());
            } else if (slot.elem != nullptr) {
                this->_iter = slot_t::iterator(slot, false);
                this->_iterend = slot_t::iterator(slot, true);
            } else {
                this->_iter = slot_t::iterator(slot, true);
                this->_iterend = slot_t::iterator(slot, true);
            }
        }

        return *this;
    }

    ebmlMasterElement::_iterator& ebmlStruct::_iterator::operator=(const ebmlMasterElement::_iterator& other) {
        if (this != &other) {
            if (auto recast = dynamic_cast<const ebmlStruct::_iterator*>(&other)) {
                this->_slotiter = recast->_slotiter;
                this->_slotiterend = recast->_slotiterend;
                this->_iter = recast->_iter;
                this->_iterend = recast->_iterend;
            }
        }

        return *this;
    }

    bool ebmlStruct::_iterator::operator==(const ebmlMasterElement::_iterator& other) const {
        if (this == &other) {
            return true;
        }

        if (auto recast = dynamic_cast<const ebmlStruct::_iterator*>(&other)) {
            if (this->_slotiter == this->_slotiterend) {
                return (this->_slotiter == recast->_slotiter);
            }

            if (recast->_slotiter == recast->_slotiterend) {
                return (this->_slotiter == recast->_slotiter);
            }

            return (this->_slotiter == recast->_slotiter) and (this->_iter == recast->_iter);
        }

        return false;
    }

    bool ebmlStruct::_iterator::operator!=(const ebmlMasterElement::_iterator& other) const {
        return not (*this == other);
    }

    ebmlStruct::_const_iterator::_const_iterator() : _elem(nullptr) {}
    ebmlStruct::_const_iterator::~_const_iterator() {}

    ebmlStruct::_const_iterator::_const_iterator(
        const ebml::ptr<const ebmlStruct>& elem,
        const std::vector<_slot_t>::const_iterator& slotiter, const std::vector<_slot_t>::const_iterator& slotiterend,
        const const_slot_t::iterator& iter, const const_slot_t::iterator& iterend) :
        _elem(elem), _slotiter(slotiter), _slotiterend(slotiterend), _iter(iter), _iterend(iterend) {}

    ebmlStruct::_const_iterator::_const_iterator(
        ebml::ptr<const ebmlStruct>&& elem,
        std::vector<_slot_t>::const_iterator&& slotiter, std::vector<_slot_t>::const_iterator&& slotiterend,
        const_slot_t::iterator&& iter, const_slot_t::iterator&& iterend) :
        _elem(std::move(elem)), _slotiter(std::move(slotiter)), _slotiterend(std::move(slotiterend)),
        _iter(std::move(iter)), _iterend(std::move(iterend)) {}

    ebmlMasterElement::_const_iterator* ebmlStruct::_const_iterator::copy() const {
        return new ebmlStruct::_const_iterator(this->_elem, this->_slotiter, this->_slotiterend, this->_iter, this->_iterend);
    }

    const ebmlElement& ebmlStruct::_const_iterator::operator*() const {
        return **this->_iter;
    }

    ebmlMasterElement::_const_iterator& ebmlStruct::_const_iterator::operator++() {
        ++this->_iter;

        while (this->_iter == this->_iterend) {
            ++this->_slotiter;

            if (this->_slotiter == this->_slotiterend) {
                break;
            }

            auto& slot = *this->_slotiter;

            if (slot.multi()) {
                this->_iter = slot_t::const_iterator(slot, slot.elems.begin());
                this->_iterend = slot_t::const_iterator(slot, slot.elems.end());
            } else if (slot.elem != nullptr) {
                this->_iter = slot_t::const_iterator(slot, false);
                this->_iterend = slot_t::const_iterator(slot, true);
            } else {
                this->_iter = slot_t::const_iterator(slot, true);
                this->_iterend = slot_t::const_iterator(slot, true);
            }
        }

        return *this;
    }

    ebmlMasterElement::_const_iterator& ebmlStruct::_const_iterator::operator=(const ebmlMasterElement::_const_iterator& other) {
        if (this != &other) {
            if (auto recast = dynamic_cast<const ebmlStruct::_const_iterator*>(&other)) {
                this->_slotiter = recast->_slotiter;
                this->_slotiterend = recast->_slotiterend;
                this->_iter = recast->_iter;
                this->_iterend = recast->_iterend;
            } else {
                throw std::runtime_error("Incompatible iterators");
            }
        }

        return *this;
    }

    bool ebmlStruct::_const_iterator::operator==(const ebmlMasterElement::_const_iterator& other) const {
        if (this == &other) {
            return true;
        }

        if (auto recast = dynamic_cast<const ebmlStruct::_const_iterator*>(&other)) {
            if (this->_slotiter == this->_slotiterend) {
                return (this->_slotiter == recast->_slotiter);
            }

            if (recast->_slotiter == recast->_slotiterend) {
                return (this->_slotiter == recast->_slotiter);
            }

            return (this->_slotiter == recast->_slotiter) and (this->_iter == recast->_iter);
        }

        return false;
    }

    bool ebmlStruct::_const_iterator::operator!=(const ebmlMasterElement::_const_iterator& other) const {
        return not (*this == other);
    }

    template class ebmlTypeCRTP<ebmlStructType, ebmlStruct, ebmlMasterElementType>;
    template class ebmlElementCRTP<ebmlStructType, ebmlStruct, ebmlMasterElement>;
    template class flexible_ptr<ebmlStruct>;
    template class ebml_shared_ptr<ebmlStruct>;
    template class ebml_weak_ptr<ebmlStruct>;
}
#endif
