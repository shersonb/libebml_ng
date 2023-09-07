#ifndef EBML_NG_MASTERELEMENT_MULTSLOT_CPP
#define EBML_NG_MASTERELEMENT_MULTSLOT_CPP

#include "libebml_ng/masterelement/multislot.h"

namespace ebml {
    slotSpec_t::slotSpec_t(const std::string& name, const childClassSpecArg_l& childSpec, bool multielem) {
        this->name = name;
        this->_childClasses = childSpec;
        this->_multi = multielem;
    }

    slotSpec_t::slotSpec_t() {}

    slotSpec_t::slotSpec_t(const slotSpec_t& other) {
        this->name = other.name;
        this->_childClasses = other._childClasses;
        this->_multi = other._multi;
    }

    slotSpec_t::slotSpec_t(slotSpec_t&& other) {
        this->name = std::move(other.name);
        this->_childClasses = std::move(other._childClasses);
        this->_multi = std::move(other._multi);
    }

    slotSpec_t::~slotSpec_t() {}

    const childClassSpec_t& slotSpec_t::childClasses() const {
        return this->_childClasses;
    }



    slotSpec_t& slotSpec_t::operator=(const slotSpec_t& other) {
        this->name = other.name;
        this->_childClasses = other._childClasses;
        this->_multi = other._multi;
        return *this;
    }

    slotSpec_t& slotSpec_t::operator=(slotSpec_t&& other) {
        this->name = std::move(other.name);
        this->_childClasses = std::move(other._childClasses);
        this->_multi = std::move(other._multi);
        return *this;
    }

    bool slotSpec_t::multi() const {
        return this->_multi;
    }

    ebmlMultiSlotClass::ebmlMultiSlotClass(ebmlID_t ebmlID, const std::wstring& name, const slotSpec_l& slotSpecs) : ebmlMasterElementClass(ebmlID, name) {
        for (const auto& slotSpec : slotSpecs) {
            auto childClasses = slotSpec.childClasses();
            const auto& slotSpecRef = this->_slotSpecs.emplace_back(slotSpec);
            this->_slots_by_name[slotSpec.name] = &slotSpecRef;

            for (const auto& childClassSpec : childClasses) {
                const auto& occurSpec = childClasses.occurSpec(childClassSpec.first);
                this->_childClasses.add({childClassSpec.second, occurSpec.min, occurSpec.max});
                this->_slots_by_ebmlID[childClassSpec.first] = &slotSpecRef;
            }
        }
    }

    ebmlMultiSlotClass::ebmlMultiSlotClass(ebmlID_t ebmlID, const std::wstring& name, const slotSpec_l& slotSpecs, const occurSpec_t& recursive, size_t recurseslot) :
        ebmlMultiSlotClass(ebmlID, name, slotSpecs) {
        if (recursive.max != 0) {
            auto& slotSpec = this->_slotSpecs.at(recurseslot);
            this->_childClasses.add({this, recursive.min, recursive.max});
            slotSpec._childClasses.add({this, recursive.min, recursive.max});
        }
    }

    ebmlElement_sp ebmlMultiSlotClass::operator()() const {
        auto elem = new ebmlMultiSlot(this);
        return ebmlElement_sp(elem);
    }

    const std::vector<slotSpec_t>& ebmlMultiSlotClass::slotSpecs() const {
        return this->_slotSpecs;
    }

    const std::unordered_map<std::string, const slotSpec_t*>& ebmlMultiSlotClass::slotSpecsByKey() const {
        return this->_slots_by_name;
    }

    const std::unordered_map<ebmlID_t, const slotSpec_t*>& ebmlMultiSlotClass::slotSpecsByID() const {
        return this->_slots_by_ebmlID;
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
        new (&this->elem) ebmlElement_sp(elem);
    }

    slotArg_t::slotArg_t(ebmlElement_sp&& elem) {
        new (&this->elem) ebmlElement_sp(std::move(elem));
    }

    slotArg_t::slotArg_t(const ebmlElement_l& elems) {
        new (&this->elems) ebmlElement_l(elems);
    }

    slotArg_t::slotArg_t(ebmlElement_l&& elems) {
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



    class _slot_t {
    protected:
        const slotSpec_t* _spec;
        union {
            ebmlElement_sp elem;
            ebmlElement_l elems;
        };

    private:
        void _check_single() const {
            if (this->_spec == nullptr) {
                throw std::runtime_error("spec is null");
            } else if (this->_spec->multi()) {
                throw ebmlException("attempting single-element access in multi-element slot");
            }
        }

        void _check_multi() const {
            if (this->_spec == nullptr) {
                throw std::runtime_error("spec is null");
            } else if (!this->_spec->multi()) {
                throw ebmlException("attempting multi-element access in single-element slot");
            }
        }

    protected:
        _slot_t(const slotSpec_t& spec) {
            this->_spec = &spec;
            if (spec.multi()) {
                new (&this->elems) ebmlElement_l();
            } else {
                new (&this->elem) ebmlElement_sp();
            }
        }
        _slot_t(const slotSpec_t& spec, const ebmlElement_sp& elem) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems.push_back(elem);
            } else {
                this->elem = elem;
            }
        }
        _slot_t(const slotSpec_t& spec, ebmlElement_sp&& elem) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems.push_back(std::move(elem));
            } else {
                this->elem = std::move(elem);
            }
        }

        _slot_t(const slotSpec_t& spec, const ebmlElement_l& elems) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems = elems;
            } else {
                this->elem.~ebmlElement_sp();
                throw ebmlException("cannot assign multiple elements to single-element slot");
            }
        }

        _slot_t(const slotSpec_t& spec, ebmlElement_l&& elems) : _slot_t(spec) {
            if (spec.multi()) {
                this->elems = std::move(elems);
            } else {
                this->elem.~ebmlElement_sp();
                throw ebmlException("cannot assign multiple elements to single-element slot");
            }
        }

    public:
        _slot_t() {
            this->_spec = nullptr;
        }
        _slot_t(const _slot_t& other) {
            if (other._spec != nullptr) {
                if (other._spec->multi()) {
                    new (&this->elems) ebmlElement_l(other.elems);
                } else {
                    new (&this->elem) ebmlElement_sp(other.elem);
                }
            }
            this->_spec = other._spec;
        }
        _slot_t(_slot_t&& other) {
            if (other._spec != nullptr) {
                if (other._spec->multi()) {
                    new (&this->elems) ebmlElement_l(std::move(other.elems));
                } else {
                    new (&this->elem) ebmlElement_sp(std::move(other.elem));
                }
            }
            this->_spec = other._spec;
        }
        _slot_t& operator=(const _slot_t& other) {
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
        _slot_t& operator=(_slot_t&& other) {
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

            this->_spec = other._spec;
            other._spec = nullptr;
            return *this;
        };
        ~_slot_t() {
            if (this->_spec->multi()) {
                this->elems.~ebmlElement_l();
            } else {
                this->elem.~ebmlElement_sp();
            }
        }

    private:
        void _validate(const ebmlElement_sp& elem) const {
            if (!this->_spec->childClasses().isValid(elem)) {
                std::string errmsg = "cannot add '";
                errmsg += pack(elem->cls()->name);
                errmsg += "' object to '";
                errmsg += this->_spec->name;
                errmsg += "' slot.";
                throw std::invalid_argument(errmsg);
            }
        }

        void _validate(const ebmlElement_l& elems) const {
            for (const auto& elem : elems) {
                this->_validate(elem);
            }
        }

    public:
        void validate(const ebmlElement_sp& elem) const {
            this->_check_single();
            this->_validate(elem);
        }

        void validate(const ebmlElement_l& elems) const {
            this->_check_multi();
            this->_validate(elems);
        }

        const slotSpec_t& slotSpec() const {
            return *this->_spec;
        }

        bool multi() const {
            return this->_spec->multi();
        }

        size_t size() const {
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
        _slot_t& operator=(const ebmlElement_sp& other) {
            this->_check_single();
            this->elem = other;
            return *this;
        }

        _slot_t& operator=(ebmlElement_sp&& other) {
            this->_check_single();
            this->elem = std::move(other);
            return *this;
        }

        operator ebmlElement_sp() const {
            this->_check_single();
            return this->elem;
        }

        operator ebmlElement_sp&() {
            this->_check_single();
            return this->elem;
        }

        operator const ebmlElement_sp&() const {
            this->_check_single();
            return this->elem;
        }

        operator c_ebmlElement_sp() const {
            this->_check_single();
            return std::static_pointer_cast<const ebmlElement>(this->elem);
        }

        // Mutli-element access and modification
        operator ebmlElement_l&() {
            this->_check_multi();
            return this->elems;
        }

        ebmlElement_sp& operator[](size_t index) { // Access and modify single element
            this->_check_multi();
            return this->elems[index];
        }

        ebmlElement_sp& at(size_t index) { // Access and modify single element
            this->_check_multi();
            return this->elems.at(index);
        }

        ebmlElement_sp& front() { // Access and modify single element
            this->_check_multi();
            return this->elems.front();
        }
        ebmlElement_sp& back() { // Access and modify single element
            this->_check_multi();
            return this->elems.back();
        }

        ebmlElement_sp& insert(size_t index, const ebmlElement_sp& elem) {
            this->_check_multi();
            this->elems.insert(this->elems.begin() + index, elem);
            return this->elems.at(index);
        }

        ebmlElement_sp& insert(size_t index, ebmlElement_sp&& elem) {
            this->_check_multi();
            this->elems.insert(this->elems.begin() + index, std::move(elem));
            return this->elems.at(index);
        }

        void erase(size_t index) {
            this->_check_multi();
            this->elems.erase(this->elems.begin() + index);
        }

        ebmlElement_sp& push_back(const ebmlElement_sp& elem) {
            this->_check_multi();
            this->elems.push_back(elem);
            return this->back();
        }

        ebmlElement_sp& push_back(ebmlElement_sp&& elem) {
            this->_check_multi();
            this->elems.push_back(std::move(elem));
            return this->back();
        }

        ebmlElement_sp& emplace_back(const ebmlElement_sp& elem) {
            this->_check_multi();
            this->elems.emplace_back(elem);
            return this->back();
        }

        ebmlElement_sp& emplace_back(ebmlElement_sp&& elem) {
            this->_check_multi();
            this->elems.emplace_back(std::move(elem));
            return this->back();
        }

#if RAW
        const ebmlElement* operator[](size_t index) const {
            this->_check_multi();
            return this->elems[index].get();
        }

        const ebmlElement* at(size_t index) const{
            this->_check_multi();
            return this->elems.at(index).get();
        }
#else
        c_ebmlElement_sp operator[](size_t) const;
        c_ebmlElement_sp at(size_t) const;
        c_ebmlElement_sp front() const;
        c_ebmlElement_sp back() const;
#endif
        // operator ebmlElement_l();
        // operator c_ebmlElement_l();

        // Assign multi-element at once
        _slot_t& operator=(const ebmlElement_l&);
        _slot_t& operator=(ebmlElement_l&&);

        std::wstring repr() const {
            std::wstring result = unpack<std::wstring>(this->_spec->name);
            result += L"=";

            const auto& childClasses = this->_spec->childClasses();

            if (this->multi()) {
                if (childClasses.size() == 0) {
                    return result + L"[]";
                } else if (childClasses.size() > 1) {
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
                if (childClasses.size() > 1) {
                    return result + this->elem->repr();
                }

                return result + this->elem->minirepr();
            }
            return L"";
        }

        friend class slot_t::iterator;
        friend class slot_t::const_iterator;
        friend class ebmlMultiSlot;
    };

    // ebmlElement_sp ebmlMultiSlotClass::operator()(const slot_init_list& init) {
    //     auto elem = new ebmlMultiSlot(this, init);
    //     return std::shared_ptr<ebmlElement>(elem);
    // }

    ebmlMultiSlot::ebmlMultiSlot(const ebmlMultiSlotClass* cls) : ebmlMasterElement(cls) {
        const auto& slotSpecs = cls->slotSpecs();
        unsigned k = 0;
        unsigned K = slotSpecs.size();
        this->_slots.reserve(K);

        for (const auto& slotSpec : slotSpecs) {
            _slot_t _slot = slotSpec;
            this->_slots.push_back(std::move(_slot));
            auto slot_ptr = &this->_slots[k];
            this->_slots_by_name[slotSpec.name] = slot_ptr;

            for (const auto& pr : slotSpec.childClasses()) {
                this->_slots_by_ebmlID[pr.first] = slot_ptr;
            }

            k++;
        }
    }

    ebmlMultiSlot::ebmlMultiSlot(const ebmlMultiSlotClass* cls, const slotArg_l& args) : ebmlMultiSlot(cls) {
        this->setData(args);
    }

    void ebmlMultiSlot::_attachChildren(const ebmlElement_l& elems, bool weak) {
        auto start = elems.cbegin();
        auto end = elems.cend();
        auto iter = start;

        while (iter != end) {
            try {
                const auto& elem = *iter;

                if (elem.get() == this) {
                    throw ebmlException("cannot attach element to itself");
                }

                ebmlElement::_attachChild(elem, weak);
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                    this->_detachChild(*iter);
                }
                throw;
            }
        }
    }

    void ebmlMultiSlot::_checkArgs(const slotArg_l& args) {
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
            errmsg += " and'";
            errmsg += this->_slots[n]._spec->name;
            errmsg += "'";
            throw std::invalid_argument(errmsg);
        }
    }

    void ebmlMultiSlot::_checkArgs(const slotArg_d& kwargs) {
        std::list<const std::string*> missing;
        std::list<const std::string*> extra;

        for (const auto& slot : this->_slots) {
            if (kwargs.count(slot.slotSpec().name) == 0) {
                missing.push_back(&slot.slotSpec().name);
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
    }

    void ebmlMultiSlot::_checkArgs(const slotArg_l& args, const slotArg_d& kwargs) {
        std::list<const std::string*> missing;
        std::list<const std::string*> extra;
        std::list<const std::string*> multiple;
        auto n = args.size();
        auto N = this->_slots.size();
        unsigned int k = 0;

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

    void ebmlMultiSlot::_checkData(const slotArg_l& args) {
        auto start = args.begin();
        auto end = args.end();
        auto iter = start;

        auto slot_iter = this->_slots.begin();

        unsigned int k = 0;

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
                    slot.validate(elem);
                    this->_attachChild(elem);
                }

                ++slot_iter;
                ++iter;
            } catch (...) {
                while (iter != start) {
                    --iter;
                }
                throw;
            }
        }
    }

    void ebmlMultiSlot::_setData(const slotArg_l& args) {
    }

    void ebmlMultiSlot::_setData(slotArg_l&& args) {
    }

    void ebmlMultiSlot::setData(const slotArg_l& args) {
        this->_checkArgs(args);
        this->_checkData(args);
        this->_setData(args);
    }

    void ebmlMultiSlot::setData(slotArg_l&& args) {
        this->_checkArgs(args);
        this->_checkData(args);
        this->_setData(std::move(args));
    }

    void ebmlMultiSlot::_clear() {
        for (auto& slot : this->_slots) {
            if (slot.multi()) {
                ebmlElement_l& elems = slot;

                for (auto& elem : elems) {
                    this->_detachChild(elem);
                }

                elems.clear();
            } else {
                ebmlElement_sp& elem = slot;
                this->_detachChild(elem);
                elem = nullptr;
            }
        }
    }

    std::wstring ebmlMultiSlot::minirepr() const {
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

    void ebmlMultiSlot::_addChild(const ebmlElement_sp& child) {
        auto& slot = *this->_slots_by_ebmlID.at(child->ebmlID());

        if (slot.multi()) {
            ebmlElement_l& elems = slot;
            elems.push_back(child);
        } else {
            ebmlElement_sp& elem = slot;
            elem  = child;
        }
    }

    void ebmlMultiSlot::_addChild(ebmlElement_sp&& child) {
        auto& slot = *this->_slots_by_ebmlID.at(child->ebmlID());

        if (slot.multi()) {
            ebmlElement_l& elems = slot;
            elems.push_back(std::move(child));
        } else {
            ebmlElement_sp& elem = slot;
            elem  = child;
        }
    }

    ebmlMasterElement::_iterator* ebmlMultiSlot::_begin() {
        return nullptr;
    }

    ebmlMasterElement::_iterator* ebmlMultiSlot::_end() {
        return nullptr;
    }

    ebmlMasterElement::_const_iterator* ebmlMultiSlot::_cbegin() const {
        return nullptr;
    }

    ebmlMasterElement::_const_iterator* ebmlMultiSlot::_cend() const {
        return nullptr;
    }

    // ebmlMultiSlot::ebmlMultiSlot(const ebmlMultiSlotClass* cls, const slot_init_list& init) : ebmlMasterElement(cls) {
    //     auto slotspec_iter = cls->_slotspecs.cbegin();
    //     auto slotspec_end = cls->_slotspecs.cend();
    //
    //     auto init_iter = init.begin();
    //     auto init_end = init.end();
    //
    //     while (slotspec_iter != slotspec_end) {
    //         const auto& slotspec = *slotspec_iter;
    //
    //         if (init_iter != init_end) {
    //             const auto& initslot = *init_iter;
    //
    //             switch (initslot.mode()) {
    //                 case 0: {
    //                     _slot_t elemslot = {this, slotspec, initslot.item()};
    //                     this->_slots.push_back(elemslot);
    //                     // this->_slots.emplace_back(this, &slotspec, slot.item());
    //                     break;
    //                 }
    //                 case 1: {
    //                     _slot_t elemslot = {this, slotspec, initslot.items()};
    //                     this->_slots.push_back(elemslot);
    //                     // this->_slots.emplace_back({this, &slotspec, initslot.items());
    //                     break;
    //                 }
    //                 case 2: {
    //                     _slot_t elemslot = {this, slotspec, initslot.initlist()};
    //                     this->_slots.push_back(elemslot);
    //                     // this->_slots.emplace_back(this, &slotspec, slot.initlist());
    //                     break;
    //                 }
    //             }
    //         } else {
    //             _slot_t elemslot = {this, slotspec, nullptr};
    //             this->_slots.push_back(elemslot);
    //         }
    //
    //         ++slotspec_iter;
    //         ++init_iter;
    //     }
    // }
    //
    // slot_t ebmlMultiSlot::operator[](size_t k) {
    //     return this->_slots.at(k);
    // }
    //
    // const_slot_t ebmlMultiSlot::operator[](size_t k) const {
    //     return this->_slots.at(k);
    // }
    // // typedef std::vector<ebmlElement_sp>::iterator ebmlElement_i;
    // // typedef std::vector<ebmlElement_sp>::const_iterator ebmlElement_ci;
    // // typedef std::vector<ebmlElement_sp> ebmlElement_l;
    //
    // // class _slotspec_t {
    // // private:
    // //     unsigned char _multi; // Flags for Multiple classes, multiple items;
    // //     union {
    // //         const ebmlElementClass* cls;
    // //         std::unordered_set<const ebmlElementClass*> clss;
    // //     }
    // //
    // // public:
    // //     _slotspec_t(const ebmlElementClass*, bool multielem=false);
    // //     _slotspec_t(const std::unordered_set<const ebmlElementClass*>&);
    // //     _slotspec_t(const std::initializer_list<const ebmlElementClass*>&);
    // //     ~_slotspec_t()
    // //
    // //     bool isValid(const ebmlElement_sp&) const;
    // //     bool isMutliCls() const;
    // //     bool isMutliElem() const;
    // //
    // //     friend class _slot_t;
    // // };
    //
    // // class _slot_t {
    // // protected:
    // //     const _slotspec_t* _spec;
    // //     union {
    // //         ebmlElement_sp elem;
    // //         ebmlElement_l elems;
    // //     }
    // //
    // // public:
    // //     _slot_t(const _slotspec_t*);
    // //     _slot_t(const _slotspec_t*, const ebmlElement_sp&);
    // //     _slot_t(const _slotspec_t*, const ebmlElement_l&);
    // //     _slot_t(const _slotspec_t*, const std::initializer_list<ebmlElement_sp>&);
    // //     ~_slot_t()
    // //     bool isMulti() const;
    // //
    // //     // Single-element slot access
    // //     ebmlElement_sp& get();
    // //     const ebmlElement_sp& get() const;
    // //
    // //     // Multiple-element slot access
    // //     ebmlElement_sp& at(size_t);
    // //     ebmlElement_l& items();
    // //     ebmlElement_sp& emplace_back(const ebmlElement_sp&);
    // //
    // //     const ebmlElement_sp& at(size_t) const;
    // //     const ebmlElement_l& items() const;
    // //
    // //     class iterator {
    // //     private:
    // //         _slot_t* _slot;
    // //         union {
    // //             bool done;
    // //             ebmlElement_l::iterator iter;
    // //         }
    // //
    // //     protected:
    // //         iterator(_slot_t&, bool);
    // //         iterator(_slot_t&, const ebmlElement_l::iterator&);
    // //
    // //     public:
    // //         iterator(iterator&);
    // //         iterator& operator=(iterator&);
    // //
    // //         iterator& operator++();
    // //         iterator operator++(int);
    // //         const ebmlElement_sp& operator*();
    // //
    // //         ~iterator();
    // //         friend class _slot;
    // //     }
    // //
    // //     iterator begin();
    // //     iterator end();
    // //
    // //     class const_iterator {
    // //     private:
    // //         const _slot_t* _slot;
    // //         union {
    // //             bool done;
    // //             ebmlElement_l::const_iterator iter;
    // //         }
    // //
    // //     protected:
    // //         const_iterator(_slot_t&, bool);
    // //         const_iterator(_slot_t&, const ebmlElement_l::const_iterator&);
    // //
    // //     public:
    // //         const_iterator(const_iterator&);
    // //         const_iterator& operator=(const_iterator&);
    // //
    // //         const_iterator& operator++();
    // //         const_iterator operator++(int);
    // //         c_ebmlElement_sp operator*();
    // //
    // //         ~const_iterator();
    // //         friend class _slot;
    // //     }
    // //
    // //     const_iterator cbegin() const;
    // //     const_iterator cend() const;
    // //
    // //     friend class iterator;
    // //     friend class const_iterator;
    // // };
    //
    //
    // // class ebmlMultiSlotClass : public ebmlMasterElementClass {
    // // public:
    // //     ebmlMultiSlotClass(ebmlID_t, std::wstring);
    // //     ebmlElement_sp operator()() const;
    // //     // std::wstring repr() const;
    // //
    // //     std::unordered_map<ebmlID_t, unsigned int> _slots_by_ebmlID;
    // //     std::unordered_map<unsigned int, std::unordered_set<ebmlID_t>> _ebmlIDs_by_slot;
    // // private:
    // //     friend class ebmlMultiSlot;
    // // };
    // //
    // // class ebmlMultiSlot : public ebmlMasterElement {
    // // private:
    // //     std::vector<ebmlElement_sp> _data;
    // //
    // // protected:
    // //     ebmlMultiSlot(const ebmlMultiSlotClass*);
    // //     ebmlMultiSlot(const ebmlMultiSlotClass*, const std::vector<ebmlDocument_sp>&);
    // //     void _clear() override;
    // //
    // // public:
    // //     std::wstring minirepr() const override;
    // //
    // //     // Element access
    // //     const ebmlElement_sp& operator[](off_t);
    // //     const ebmlElement_sp& at(off_t);
    // //     const ebmlElement_sp& front();
    // //     const ebmlElement_sp& back();
    // //
    // //     // Const element access
    // //     c_ebmlElement_sp operator[](off_t) const;
    // //     c_ebmlElement_sp at(off_t) const;
    // //     c_ebmlElement_sp front() const;
    // //     c_ebmlElement_sp back() const;
    // //
    // //     // Element search
    // //     bool contains(const c_ebmlElement_sp&) const;
    // //     off_t index(const c_ebmlElement_sp&) const;
    // //
    // //     // Element modification
    // //     void assign(off_t, const ebmlElement_sp&);
    // //     void push_back(const ebmlElement_sp&);
    // //     void pop_back();
    // //     void insert(off_t, const ebmlElement_sp&);
    // //     void erase(off_t);
    // //     void clear();
    // //
    // //     // Container size
    // //     size_t size() const;
    // //
    // //     // Iteration
    // // protected:
    // //     class _iterator : public ebmlMasterElement::_iterator {
    // //     private:
    // //         ebmlElement_sp _elem;
    // //         std::vector<ebmlElement_sp>::iterator _iter;
    // //
    // //     protected:
    // //         _iterator(const ebmlElement_sp& elem, const std::vector<ebmlElement_sp>::iterator& iter);
    // //
    // //     public:
    // //         _iterator();
    // //         virtual ~_iterator();
    // //
    // //         // std::shared_ptr<ebmlMasterElement::_iterator> copy();
    // //         ebmlMasterElement::_iterator* copy();
    // //
    // //         const ebmlElement_sp& operator*() const;
    // //         ebmlMasterElement::_iterator& operator++();
    // //         ebmlMasterElement::_iterator& operator=(const ebmlMasterElement::_iterator&);
    // //         // ebmlMasterElement::_iterator& operator=(const _iterator&);
    // //         bool operator==(const ebmlMasterElement::_iterator&) const;
    // //         bool operator!=(const ebmlMasterElement::_iterator&) const;
    // //         // bool operator==(const _iterator&) const;
    // //         // bool operator!=(const _iterator&) const;
    // //         friend class ebmlMultiSlot;
    // //         friend class ebmlMasterElement::iterator;
    // //         // friend std::shared_ptr<_iterator> std::make_shared<_iterator>(const ebmlElement_sp&, const std::vector<ebmlElement_sp>::iterator&);
    // //         friend class std::shared_ptr<_iterator>;
    // //     };
    // //
    // //     // std::shared_ptr<ebmlMasterElement::_iterator> _begin() override;
    // //     // std::shared_ptr<ebmlMasterElement::_iterator> _end() override;
    // //
    // //     ebmlMasterElement::_iterator* _begin() override;
    // //     ebmlMasterElement::_iterator* _end() override;
    // //
    // //
    // //     class _const_iterator : public ebmlMasterElement::_const_iterator {
    // //     private:
    // //         c_ebmlElement_sp _elem;
    // //         std::vector<ebmlElement_sp>::const_iterator _iter;
    // //
    // //     protected:
    // //         _const_iterator(const c_ebmlElement_sp& elem, const std::vector<ebmlElement_sp>::const_iterator& iter);
    // //
    // //     public:
    // //         _const_iterator();
    // //         virtual ~_const_iterator();
    // //
    // //         // std::shared_ptr<ebmlMasterElement::_const_iterator> copy();
    // //         ebmlMasterElement::_const_iterator* copy();
    // //
    // //         c_ebmlElement_sp operator*() const;
    // //         ebmlMasterElement::_const_iterator& operator++();
    // //         ebmlMasterElement::_const_iterator& operator=(const ebmlMasterElement::_const_iterator&);
    // //         bool operator==(const ebmlMasterElement::_const_iterator&) const;
    // //         bool operator!=(const ebmlMasterElement::_const_iterator&) const;
    // //         friend class ebmlMultiSlot;
    // //         friend class ebmlMasterElement::const_iterator;
    // //     };
    // //
    // //     // std::shared_ptr<ebmlMasterElement::_const_iterator> _cbegin() const override;
    // //     // std::shared_ptr<ebmlMasterElement::_const_iterator> _cend() const override;
    // //
    // //     ebmlMasterElement::_const_iterator* _cbegin() const override;
    // //     ebmlMasterElement::_const_iterator* _cend() const override;
    // //
    // //     void _addChild(const ebmlElement_sp&) override;
    // //     // virtual void _scanChildren(parseFile::iterator&); // Decode children from an iterParseFile instance created by _decode.
    // //     friend class ebmlMultiSlotClass;
    // // };
}
#endif
