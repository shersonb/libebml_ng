#ifndef EBML_NG_LAZYLOADMASTERELEMENT_H
#define EBML_NG_LAZYLOADMASTERELEMENT_H

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <mutex>
#include <shared_mutex>

#include "base.h"
#include "../seekdata.h"

namespace ebml {
    class ebmlLazyLoadClass : public ebmlMasterElementClass {
    public:
        ebmlLazyLoadClass(const char*, const std::wstring&, const occurSpec_t& recursive={0, 0});
        ebmlLazyLoadClass(const char*, const std::wstring&, const childClassSpecArg_l&, const occurSpec_t& recursive={0, 0});
        ebmlLazyLoadClass(const char*, const std::wstring&, childClassSpecArg_init_l, const occurSpec_t& recursive={0, 0});

        ebmlLazyLoadClass(ebmlID_t, const std::wstring&, const occurSpec_t& recursive={0, 0});
        ebmlLazyLoadClass(ebmlID_t, const std::wstring&, const childClassSpecArg_l&, const occurSpec_t& recursive={0, 0});
        ebmlLazyLoadClass(ebmlID_t, const std::wstring&, childClassSpecArg_init_l, const occurSpec_t& recursive={0, 0});

    protected:
        // Read existing element in file
        ebmlElement_sp operator()(ioBase*) const;
        ebmlElement_sp operator()(ioBase*, off_t) const;

        // Create new element in file
        ebmlElement_sp operator()(ioBase*, vintWidth_t, size_t) const;
        ebmlElement_sp operator()(ioBase*, off_t, vintWidth_t, size_t) const;

        ebmlElement* _new() const override;
        // void _handleSeekData(ebmlMasterElement* elem, const parseFile&) const override;

    public:

        // Create new element in parent element
        ebmlElement_sp operator()(const ebmlElement_sp&, vintWidth_t, size_t) const;
        ebmlElement_sp operator()(ebmlElement_sp&&, vintWidth_t, size_t) const;
        ebmlElement_sp operator()(const ebmlElement_sp&, off_t, vintWidth_t, size_t) const;
        ebmlElement_sp operator()(ebmlElement_sp&&, off_t, vintWidth_t, size_t) const;
        friend class ebmlSchema;

    };

    class ebmlLazyLoad : public ebmlMasterElement {
    private:
        std::shared_mutex _mutex;
        ioBase_sp _file;
        ioBase* _file_raw = nullptr;
        size_t _dataSize;

        std::map<off_t, std::unique_ptr<seekData_t>> _children_by_offset;
        std::unordered_map<
            ebmlID_t,
            std::map<off_t, seekData_t*>
        > _children_by_ebmlID;
        std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>> _children_by_key;

    protected:
        ebmlLazyLoad(const ebmlLazyLoadClass* cls);

        // // Read existing element in file
        // // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase*);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp&);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&&);
        //
        // // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase*, off_t);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp&, off_t);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&&, off_t);
        //
        // // Create new element in file
        // // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase*, vintWidth_t, size_t);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp&, vintWidth_t, size_t);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&&, vintWidth_t, size_t);
        // // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase*, off_t, vintWidth_t, size_t);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, const ioBase_sp&, off_t, vintWidth_t, size_t);
        // ebmlLazyLoad(const ebmlLazyLoadClass* cls, ioBase_sp&&, off_t, vintWidth_t, size_t);

        // void _initExisting(); // Probably should just be _decode
        void _initNew(ioBase*, off_t, vintWidth_t, size_t);

    public:
        const ioBase_sp& file();

        // Insertion and removal functions
        seekData_t& insert(const ebmlElement_sp& child, off_t offset);
        seekData_t& insert(ebmlElement_sp&& child, off_t offset);
        seekData_t& insert(const std::string& data);
        seekData_t& insert(const char* data);

        template<typename T, typename... Args>
        std::enable_if_t<std::is_base_of<ebmlLazyLoadClass, T>::value, seekData_t&>
        insert(T& cls, Args...);

        template<typename T, typename... Args>
        std::enable_if_t<!std::is_base_of<ebmlLazyLoadClass, T>::value, seekData_t&>
        insert(T& cls, Args...);

        void remove(const ebmlElement_sp& child);
        void remove(off_t offset);


        // Block boundary functions
        off_t findBoundary(off_t offset);
        off_t rfindBoundary(off_t offset);

        // Child navigation functions
        seekData_t* firstChild();
        seekData_t* lastChild();
        seekData_t* nextChild(off_t offset);
        seekData_t* prevChild(off_t offset);

        // Insertion and move check functions
        bool canInsert(off_t offset, size_t size);
        bool canMove(off_t src_offset, off_t dst_offset);

        // Space management functions
        off_t findFree(off_t offset, size_t size);
        off_t rfindFree(off_t offset, size_t size);

        // Resize check functions
        bool canResize(size_t size);
        // bool canResizeChild(const seekData_t&, size_t size);
        // bool canResizeChild(ebmlElement* child, size_t size);

        // Range insertion functions
        bool canInsertRange(off_t offset, size_t size);
        void insertRange(off_t offset, size_t size);

        // Range collapse functions
        bool canCollapseRange(off_t offset, size_t size);
        void collapseRange(off_t offset, size_t size);

        // Hole punching functions
        bool canPunchHole(off_t offset, size_t size);
        void punchHole(off_t offset, size_t size);

        // Members from ebmlElement
        std::wstring minirepr() const override;
        size_t dataSize() const override;

    protected:
        void _handleParseFile(const parseFile&) override;
        virtual void _handleSeekData(std::unique_ptr<seekData_t>&&);
        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;
        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;
        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;
        // Synchroniziation
//     private:
//         std::mutex _mutex;
//
//     public:
//         class localLock {
//             friend class ebmlLazyLoad;
//         private:
//             ebmlLazyLoad* _element;
//             std::shared_lock<std::mutex> _documentLock;
//
//         protected:
//             localLock(ebmlLazyLoad* element, bool write = false)
//                 : _element(element)
//             {
//                 _documentLock = std::shared_lock<std::mutex>(_element->_document.lock());
//                 if (write) {
//                     _element->_mutex.lock();
//                 }
//                 else {
//                     _element->_mutex.lock_shared();
//                 }
//             }
//
//         public:
//             ~localLock()
//             {
//                 if (_documentLock.owns_lock()) {
//                     _element->_mutex.unlock();
//                 }
//                 else {
//                     _element->_mutex.unlock_shared();
//                 }
//             }
//         };
//
//         std::unique_lock<std::mutex> getLocalLock(bool write=false);
//         std::unique_lock<std::mutex> getGlobalLock();
        friend class ebmlLazyLoadClass;
    };

    EXTERN_AS_MEMBERS(ebmlLazyLoadClass, ebmlLazyLoad)
}
#endif
