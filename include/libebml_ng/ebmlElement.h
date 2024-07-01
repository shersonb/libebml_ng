#ifndef EBML_NG_ELEMENT_H
#define EBML_NG_ELEMENT_H

#include "ptrs.h"
#include "parsing/string.h"
#include "parsing/io.h"
#include "flexptr.h"

namespace ebml {
    extern template class flexible_ptr<ebmlElement>;
    typedef flexible_ptr<ebmlElement> ebmlElement_fp;

    extern template class flexible_ptr<ebmlMasterElement>;
    typedef flexible_ptr<ebmlMasterElement> ebmlMasterElement_fp;

    /**
     * Abstract base class for EBML Element objects.
     * Every subclass of ebmlElement must have a companion subclass of
     * ebmlElementClass declared as a friend class.
     */

    class ebmlElement : public std::enable_shared_from_this<ebmlElement> {
        friend class ebmlDocument;
        friend class ebmlElementClass;
        friend class ebmlMasterElement;
        friend class ebmlLazyLoad;
        // friend class ebmlSchema;

        template<typename T>
        friend class ebmlDataElementClass;

        // Constructor/Destructor
    protected:
        // Only its companion class is allowed to construct instances of this class.
        ebmlElement(const ebmlElementClass*);
        void _validate();

    public:
        virtual ~ebmlElement();

        // Pointer to an instance of its companion ebmlElmeentClass subclass instance.
    protected:
        const ebmlElementClass* _cls;
        wptr<ebmlElement> _self;

    public:
        // This method *should* be overriden in subclasses to dyanmic cast its _cls member to that of the
        // companion ebmlElementClass subclass.
        virtual const ebmlElementClass& cls() const;
        ebmlID_t ebmlID() const;

        template<typename T=ebmlElement>
        inline T& as();

        template<typename T=ebmlElement>
        inline T& as() const;

        // template<typename T=ebmlElement>
        // T* ptr();
        //
        // template<typename T=ebmlElement>
        // const T* ptr() const;

        ebml::ptr<ebmlElement> sp();
        ebml::ptr<const ebmlElement> sp() const;

        template<typename T>
        inline ebml::ptr<T> sp();

        template<typename T>
        inline ebml::ptr<T> sp() const;

        ebml::wptr<ebmlElement> wp();
        ebml::wptr<const ebmlElement> wp() const;

        template<typename T>
        inline ebml::wptr<T> wp();

        template<typename T>
        inline ebml::wptr<T> wp() const;

        // Hierarchy members.
    public:
        /**
         * Specifies whether ebmlElement instance is storing a pointer to a
         * const instance of its parent.
         */

        bool parent_is_const() const; // Member function that specifies if pointer is to a const parent or non-const parent.

        /**
         * Returns a shared pointer to parent.
         */

        ebml::ptr<ebmlMasterElement> parent() const; // Returns pointer to non-const parent. Throws ebmlException if parent_is_const() returns true.

        /**
         * Returns a shared pointer to parent (const access).
         */

        ebml::ptr<const ebmlMasterElement> c_parent() const; // Returns pointer to const parent.

    private:
        flexible_ptr<ebmlMasterElement> _parent;
        // unsigned char _parent_flags = 0;
        //
        // union {
        //     ebmlElement_sp _parent;
        //     ebmlElement_wp _parent_weak;
        //     c_ebmlElement_sp _const_parent;
        //     c_ebmlElement_wp _const_parent_weak;
        // };

    protected:
        void _setParent(const ebml::ptr<ebmlMasterElement>& parent, bool weak = true);
        void _setParent(const ebml::ptr<const ebmlMasterElement>& parent, bool weak = true);
        void _setParent(ebml::ptr<ebmlMasterElement>&& parent, bool weak = true);
        void _setParent(ebml::ptr<const ebmlMasterElement>&& parent, bool weak = true);

        void _setParent(ebmlMasterElement& parent, bool weak = true);
        void _setParent(const ebmlMasterElement& parent, bool weak = true);

        // virtual void _attachChild(const ebmlElement_sp& child, bool weak = true);
        // virtual void _attachChild(const ebmlElement_sp& child, bool weak = true) const;

        void _detach();
        // void _detachChild(const ebmlElement_sp& child) const;

    public:
        ebmlElement_sp root() const;
        c_ebmlElement_sp c_root() const;

    protected:
        ebmlDocument_wp _document;

    public:
        ebmlDocument_sp document() const;

    public:
        bool hasParent() const;

        // Size functions.
    public:
        vintWidth_t _sizeWidth = 0;
        unsigned char headSize() const;
        unsigned char headSize(size_t) const; // May be deprecated
        unsigned char sizeWidth() const;
        unsigned char sizeWidth(size_t) const; // May be deprecated
        size_t outerSize() const;
        size_t outerSize(size_t) const; // May be deprecated
        virtual size_t dataSize() const = 0;

        // Location attributes and methods.
    public:
        off_t offsetInParent() const;
        off_t offsetInFile() const;
        off_t dataOffsetInParent() const;
        off_t dataOffsetInFile() const;

    protected:
        off_t _offsetInParent = -1; // value of -1 indicates undefined.

        // Encode functions.
    protected:
        size_t _encode_head(char*, size_t) const;
        virtual size_t _encode(char*) const = 0;

    public:
        virtual std::string encode() const;
        virtual size_t encode(char*) const;
        // size_t encode(const ioBase_sp&) const;
        // size_t encode(const ioBase_sp&, off_t) const;
        virtual size_t encode(ioBase&) const;
        virtual size_t encode(ioBase&, off_t) const;
        size_t encode(char*, size_t) const;

        // Cloning functions:
    protected:
        // virtual void _clonedata(const ebmlElement*) = 0;
        virtual ebmlElement* _clone() const = 0;

    public:
        // virtual ebmlElement_sp clone() const;
        ebml::ptr<ebmlElement> clone() const;

        // Repr function:
        virtual std::wstring minirepr() const = 0;
        virtual std::wstring repr() const;

        seekData_t* makeSeekData();
        seekData_t* makeSeekData() const;
    };

    template<typename ebmltype_t, typename ebmlinst_t, typename instbase_t=ebmlElement>
    class InstMixin : public instbase_t {
    protected:
        ebmlElement* _clone() const;

    public:
        using ebmltype = ebmltype_t;

        template<typename... Args>
        InstMixin(const ebmltype_t* cls, Args... args) : instbase_t(cls, args...) {}
        const ebmltype_t& cls() const;

        using ebmlElement::sp;
        ebml::ptr<ebmlinst_t> sp();
        ebml::ptr<const ebmlinst_t> sp() const;

        using ebmlElement::wp;
        ebml::wptr<ebmlinst_t> wp();
        ebml::wptr<const ebmlinst_t> wp() const;

        ebml::ptr<ebmlinst_t> clone() const;
    };

    template<typename ebmlinst_t, typename parse_t>
    concept has_init = requires(ebmlinst_t& instance, const parse_t& parsed) {
        { instance._init(parsed) };
    };

    template<typename ebmlinst_t, typename parse_t>
    concept has_cinit = requires(ebmlinst_t& instance, const parse_t& parsed) {
        { instance._cinit(parsed) };
    };

    template<typename ebmlinst_t, typename... Args>
    concept has_constructor = requires(Args... args) {
        { ebmlinst_t(args...) };
    };

    template<typename ebmlinst_t>
    concept has_clone_method = requires(ebmlinst_t& instance, ebmlinst_t& orig) {
        { instance._clone(orig) };
    };

    template<typename T>
    inline T& ebmlElement::as() {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    template<typename T>
    inline T& ebmlElement::as() const {
        if (auto recast = dynamic_cast<T*>(this)) {
            return *recast;
        }

        throw std::bad_cast();
    }

    template<typename T>
    ebml::wptr<T> ebmlElement::wp() {
        return ebml_dynamic_pointer_cast<T>(this->_self);
    }

    template<typename T>
    ebml::wptr<T> ebmlElement::wp() const {
        auto ptr = ebml_static_pointer_cast<const ebmlElement>(this->_self);
        return ebml_dynamic_pointer_cast<T>(std::move(ptr));
    }

    inline off_t ebmlElement::offsetInParent() const {
        return _offsetInParent;
    }

    inline void ebmlElement::_validate() {
        auto lock = std::unique_lock(_self.mutex);

        if (_self.ctl == nullptr) {
            _self.ctl = new control_block;
            _self.ctl->weakcount = 1;
            _self.ptr = this;
        }

        _self.ctl->valid = true;
    }

    inline size_t ebmlElement::encode(char* dest) const {
        size_t dataSize = this->dataSize();
        return ebmlElement::encode(dest, dataSize);
    }

    inline std::string ebmlElement::encode() const {
        std::string result;
        size_t dataSize = this->dataSize();
        result.resize(dataSize+16);
        size_t outerSize = this->encode(&result[0], dataSize);
        result.resize(outerSize);
        return result;
    }

    inline size_t ebmlElement::encode(ioBase& dest) const {
        size_t _dataSize = dataSize();
        auto buffer = std::make_unique<char[]>(_dataSize + 16);
        auto outerSize = encode(buffer.get(), _dataSize);
        return dest.write(buffer.get(), outerSize);
    }

    inline size_t ebmlElement::encode(ioBase& dest, off_t offset) const {
        size_t _dataSize = dataSize();
        auto buffer = std::make_unique<char[]>(_dataSize + 16);
        auto outerSize = encode(buffer.get(), _dataSize);
        return dest.write(buffer.get(), offset, outerSize);
    }
}

#endif
