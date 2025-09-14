#ifndef EBML_NG_LAZYLOAD_BASE_H
#define EBML_NG_LAZYLOAD_BASE_H

#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>

#include "../base/ebmlMasterElementType.h"
#include "../base/ebmlMasterElement.h"
#include "childElemHelper_t.h"
#include "children_m.h"
#include "status.h"
#include "prepared_insert_t.h"
#include "prepared_multiinsert_t.h"
#include "prepared_move_t.h"
#include "prepared_remove_t.h"
#include "prepared_resize_t.h"
#include "prepared_fallocate_t.h"
#include "locks.h"
#include "../../ebmlVoid.h"
#include "../../repr.h"

#define EN_IF_INST(T, R) std::enable_if_t<!std::is_same<ebmlElement, T>::value && std::is_base_of<ebmlElement, std::decay_t<T>>::value, R>
#define EN_IF_CLS(T, R) std::enable_if_t<std::is_base_of<ebmlElementType, std::decay_t<T>>::value, R>
#define EN_IFN_INST(T, R) std::enable_if_t<!std::is_base_of<ebmlElement, std::decay_t<T>>::value, R>
#define EN_IFN_CLS(T, R) std::enable_if_t<!std::is_base_of<ebmlElementType, std::decay_t<T>>::value, R>
#define KEY_ARGS typename K, typename H = std::hash<K>, typename E = std::equal_to<K>

namespace ebml {
    typedef std::pair<ebmlElement_sp, seekData_t*> elem_seek_pair;

    extern template class ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>;

    /**
     * @ingroup type_definitions
     * @ingroup type_implementations
     * @ingroup master_element_type_definitions
     * @ingroup master_element_type_implementations
     * @class ebmlLazyLoadType
     * @brief A template class representing a lazy-loading EBML master element.
     *
     * This template defines a master element that optimizes file parsing by only decoding its children when they are explicitly requested.
     * This is particularly useful for handling very large EBML files, as it reduces memory usage and improves performance.
     */
    class ebmlLazyLoadType : public ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType> {
    protected:
        const childElemHelper_t* _childHelper = &childHelper;
    public:
        ebmlLazyLoadType(const char*, const std::wstring&, const childTypeSpecArg_l&);
        ebmlLazyLoadType(ebmlID_t, const std::wstring&, const childTypeSpecArg_l&);
        ebmlLazyLoadType(const char*, const std::wstring&, const childTypeSpecArg_l&, const seekHelper_t*);
        ebmlLazyLoadType(ebmlID_t, const std::wstring&, const childTypeSpecArg_l&, const seekHelper_t*);

        ebmlLazyLoadType(const char*, const std::wstring&, const childTypeSpecArg_l&, const childElemHelper_t*);
        ebmlLazyLoadType(ebmlID_t, const std::wstring&, const childTypeSpecArg_l&, const childElemHelper_t*);
        ebmlLazyLoadType(const char*, const std::wstring&, const childTypeSpecArg_l&, const seekHelper_t*, const childElemHelper_t*);
        ebmlLazyLoadType(ebmlID_t, const std::wstring&, const childTypeSpecArg_l&, const seekHelper_t*, const childElemHelper_t*);

        friend class ebmlSchema;
        friend class ebmlLazyLoad;
    };

    extern template class ebmlElementCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElement>;

    /**
     * @class ebmlLazyLoad
     * @brief Represents an EBML Master Element with on-disk, transactional child management.
     * @details The `ebmlLazyLoad` class is a high-performance, thread-safe implementation of an EBML Master Element.
     * It is designed for managing large or sparse EBML files by avoiding the need to load all child elements into memory at once.
     *
     * This class operates as a transactional manager for its children, providing a robust and consistent interface for
     * modifying the on-disk file structure. It uses a dual-map system to balance performance and usability:
     * - A physical map (`children_m`) is used to manage children based on their on-disk offset, which is crucial for
     * efficient file I/O operations such as inserts, removals, and resizes.
     * - A logical index (`_children_by_key`) provides fast, in-memory lookups of children using a user-defined key.
     *
     * The class uses an explicit locking mechanism through `readLock_t` and `writeLock_t` instances. This design enables
     * callers to perform a series of atomic, multi-step operations (e.g., a move or a multi-insert) while holding a single
     * lock, thereby preventing race conditions and ensuring on-disk data integrity.
     *
     * @see ebmlLazyLoadType
     * @see ebmlMasterElement
     */
    class ebmlLazyLoad : public ebmlElementCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElement> {
        friend class ebmlTypeCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElementType>;
        friend class ebmlElementCRTP<ebmlLazyLoadType, ebmlLazyLoad, ebmlMasterElement>;

    protected:
        mutable std::shared_mutex _mutex;
        ioBase_sp _file;
        ioBase* _file_raw = nullptr;
        size_t _dataSize;

        /* Children management members */
        children_m _children_by_offset;
        std::unordered_map<
            ebmlID_t,
            std::map<off_t, seekData_t*>
        > _children_by_ebmlID;
        std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>> _children_by_key;

        ebmlLazyLoad(const ebmlLazyLoadType *);

        ebmlLazyLoad(const ebmlLazyLoadType *, ioBase&, off_t, vintWidth_t, size_t);
        ebmlLazyLoad(const ebmlLazyLoadType *, ioBase&, off_t, vintWidth_t, size_t, const ebmlElement_l&);

        ebmlLazyLoad(const ebmlLazyLoadType *, ebmlLazyLoad&, off_t, vintWidth_t, size_t);
        ebmlLazyLoad(const ebmlLazyLoadType *, ebmlLazyLoad&, off_t, vintWidth_t, size_t, const ebmlElement_l&);

        size_t _initNew(ioBase&, off_t, vintWidth_t, size_t);

    public:
        const children_m& seeksByOffset() const;
        const std::unordered_map<
                ebmlID_t,
                std::map<off_t, seekData_t*>
            >& seeksByEBMLID() const;
        const std::unordered_map<ebmlID_t, std::unique_ptr<seekMapBase>>& seeksByKey() const;
        ioBase& file() const;

        /* SYNCHRONIZATIOn*/
    public:
        readLock_t getRLock() const;
        readLock_t getRLock(const readLock_t&) const;

        writeLock_t getWLock() const;
        writeLock_t getWLock(const readLock_t&) const;
        writeLock_t getWLock(const writeLock_t&) const;

        writeLock_t getGlobalLock() const;

    private:
        void _verifyLock(const writeLock_t& lock) const;

        /* TODO: Find another place in the declaration for this member*/
    public:
        off_t endOfData() const;

        /* RETRIEVAL */
    public:
        inline ebml::ptr<ebmlElement> get(off_t);
        inline ebml::ptr<ebmlElement> get(off_t, const readLock_t&);
        inline ebml::ptr<ebmlElement> get(off_t, const writeLock_t&);

    private:
        ebml::ptr<ebmlElement> _get(off_t);

    public:
        template<typename T> inline ebml::ptr<T> get(off_t);
        template<typename T> inline ebml::ptr<T> get(off_t, const readLock_t&);
        template<typename T> inline ebml::ptr<T> get(off_t, const writeLock_t&);

    private:
        template<typename T> inline ebml::ptr<T> _get(off_t);

    public:
        inline ebml::ptr<ebmlElement> get(ebmlID_t, size_t);
        inline ebml::ptr<ebmlElement> get(ebmlID_t, size_t, const readLock_t&);
        inline ebml::ptr<ebmlElement> get(ebmlID_t, size_t, const writeLock_t&);

    private:
        ebml::ptr<ebmlElement> _get(ebmlID_t, size_t);

    public:
        template<typename T> ebml::ptr<T> get(ebmlID_t, size_t);
        template<typename T> ebml::ptr<T> get(ebmlID_t, size_t, const readLock_t&);
        template<typename T> ebml::ptr<T> get(ebmlID_t, size_t, const writeLock_t&);

    private:
        template<typename T> ebml::ptr<T> _get(ebmlID_t, size_t);

    public:
        /**
         * @brief Gets a child element based on a user-defined key.
         * @tparam T The type of the child element to retrieve.
         * @tparam Key The type of the key used for the lookup.
         * @param lock A reference to an existing readLock_t or writeLock_t instance.
         * @param key The key of the child element to find.
         * @return A shared pointer to the found child element.
         * @throws std::runtime_error If a child with the specified key does not exist.
         * @note This method requires an explicit lock instance from the caller, which enables
         * atomic, multi-step transactions.
         */
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<ebmlElement>) getByKey(ebmlID_t, const K&);
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<ebmlElement>) getByKey(ebmlID_t, const K&, const readLock_t&);
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<ebmlElement>) getByKey(ebmlID_t, const K&, const writeLock_t&);

    private:
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<ebmlElement>) _getByKey(ebmlID_t, const K&);

    public:
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>) getByKey(ebmlID_t, const K&);
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>) getByKey(ebmlID_t, const K&, const readLock_t&);
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>) getByKey(ebmlID_t, const K&, const writeLock_t&);

    private:
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>)  _getByKey(ebmlID_t, const K&);

    public:
        inline ebml::ptr<const ebmlElement> cget(off_t) const;
        inline ebml::ptr<const ebmlElement> cget(off_t, const readLock_t&) const;
        inline ebml::ptr<const ebmlElement> cget(off_t, const writeLock_t&) const;

    private:
        ebml::ptr<const ebmlElement> _cget(off_t) const;

    public:
        template<typename T> ebml::ptr<T> cget(off_t) const;
        template<typename T> ebml::ptr<T> cget(off_t, const readLock_t&) const;
        template<typename T> ebml::ptr<T> cget(off_t, const writeLock_t&) const;

    private:
        template<typename T> ebml::ptr<T> _cget(off_t) const;

    public:
        inline ebml::ptr<const ebmlElement> cget(ebmlID_t, size_t) const;
        inline ebml::ptr<const ebmlElement> cget(ebmlID_t, size_t, const readLock_t&) const;
        inline ebml::ptr<const ebmlElement> cget(ebmlID_t, size_t, const writeLock_t&) const;

    private:
        ebml::ptr<const ebmlElement> _cget(ebmlID_t, size_t) const;

    public:
        template<typename T> ebml::ptr<T> cget(ebmlID_t, size_t) const;
        template<typename T> ebml::ptr<T> cget(ebmlID_t, size_t, const readLock_t&) const;
        template<typename T> ebml::ptr<T> cget(ebmlID_t, size_t, const writeLock_t&) const;

    private:
        template<typename T> ebml::ptr<T> _cget(ebmlID_t, size_t) const;

    public:
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<const ebmlElement>) cgetByKey(ebmlID_t, const K&) const;
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<const ebmlElement>) cgetByKey(ebmlID_t, const K&, const readLock_t&) const;
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<const ebmlElement>) cgetByKey(ebmlID_t, const K&, const writeLock_t&) const;

    private:
        template<KEY_ARGS> EN_IFN_INST(K, ebml::ptr<const ebmlElement>) _cgetByKey(ebmlID_t, const K&) const;

    public:
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>) cgetByKey(ebmlID_t, const K&) const;
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>) cgetByKey(ebmlID_t, const K&, const readLock_t&) const;
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>) cgetByKey(ebmlID_t, const K&, const writeLock_t&) const;

    private:
        template<typename T, KEY_ARGS> EN_IF_INST(T, ebml::ptr<T>)  _cgetByKey(ebmlID_t, const K&) const;

    /* INSERTION */

    public:
        status_t<prepared_insert_t> canInsert(off_t, sizetree_t&&, const writeLock_t&);
        status_t<prepared_multiinsert_t> canInsert(off_t, std::vector<sizetree_t>&&, const writeLock_t&);

    private:
        status_t<prepared_insert_t> _canInsert(off_t, sizetree_t&&);
        status_t<prepared_multiinsert_t> _canInsert(off_t, std::vector<sizetree_t>&&);

        // Insertion functions

        // Insert single ebmlElement instance
    public:
        inline status_t<prepared_insert_t> canInsert(off_t, const ebml::ptr<ebmlElement>&, const writeLock_t&);

        inline seekData_t* insert(off_t, const ebml::ptr<ebmlElement>&);
        inline seekData_t* insert(off_t, const ebml::ptr<ebmlElement>&, const readLock_t&);
        inline seekData_t* insert(off_t, const ebml::ptr<ebmlElement>&, const writeLock_t&);
        inline seekData_t* insert(off_t, const ebml::ptr<ebmlElement>&, const writeLock_t&, const status_t<prepared_insert_t>&);

    private:
        status_t<prepared_insert_t> _canInsert(off_t, const ebml::ptr<ebmlElement>&);

        inline seekData_t* _insert(off_t, const ebml::ptr<ebmlElement>&);
        seekData_t* _insert(off_t, const ebml::ptr<ebmlElement>&, const status_t<prepared_insert_t>&);

        // Insert multiple ebmlElement instances
    public:
        status_t<prepared_multiinsert_t> canInsert(off_t, const ebmlElement_l&, const writeLock_t&);

        std::vector<seekData_t*> insert(off_t, const ebmlElement_l&);
        std::vector<seekData_t*> insert(off_t, const ebmlElement_l&, const readLock_t&);
        std::vector<seekData_t*> insert(off_t, const ebmlElement_l&, const writeLock_t&);
        std::vector<seekData_t*> insert(off_t, const ebmlElement_l&, const writeLock_t&, const status_t<prepared_multiinsert_t>&);

    private:
        status_t<prepared_multiinsert_t> _canInsert(off_t, const ebmlElement_l&);
        std::vector<seekData_t*> _insert(off_t, const ebmlElement_l& children, const status_t<prepared_multiinsert_t>&);

        // Insert data from std::string (WARNING: The only validation check is to check the ebmlID and data size. Use responsibly!)
    public:
        // TODO: Support multiple elements in single string.
        status_t<prepared_insert_t> canInsert(off_t, const std::string&, const writeLock_t&);

        seekData_t* insert(off_t, const std::string&);
        seekData_t* insert(off_t, const std::string&, const readLock_t&);
        seekData_t* insert(off_t, const std::string&, const writeLock_t&);
        seekData_t* insert(off_t, const std::string&, const writeLock_t&, const status_t<prepared_insert_t>&);

    private:
        status_t<prepared_insert_t> _canInsert(off_t, const std::string&);
        inline seekData_t* _insert(off_t, const std::string&, const status_t<prepared_insert_t>&);

        // Insert data from const char* (WARNING: The only validation check is to check the ebmlID and data size. Use responsibly!)
    public:
        inline status_t<prepared_insert_t> canInsert(off_t, const char*, const writeLock_t&);

        inline seekData_t* insert(off_t, const char* data);
        inline seekData_t* insert(off_t, const char* data, const readLock_t&);
        inline seekData_t* insert(off_t, const char* data, const writeLock_t&);
        inline seekData_t* insert(off_t, const char* data, const writeLock_t&, const status_t<prepared_insert_t>&);

    private:
        inline status_t<prepared_insert_t> _canInsert(off_t, const char*);
        seekData_t* _insert(off_t, const char* data, const status_t<prepared_insert_t>&);

    public:
        template<typename T, typename... Args> inline EN_IF_CLS(T, elem_seek_pair) insert(off_t, const T& cls, Args&&...);
        template<typename T, typename... Args> inline EN_IF_CLS(T, elem_seek_pair) insert(off_t, const readLock_t&, const T& cls, Args&&...);
        /**
         * @brief Inserts a new child element at a specified offset.
         * @tparam T The type of the child element class.
         * @param lock A reference to an existing writeLock_t instance.
         * @param offset The on-disk offset where the new element should be inserted.
         * @param cls A reference to the ebmlElementType instance for the new child.
         * @param args Arguments to pass to the new child's constructor.
         * @return A pair containing a shared pointer to the new element and its seekData_t object.
         * @throws std::runtime_error If the insertion is not possible due to file constraints or an invalid offset.
         * @note This method is part of a transactional pattern and requires the caller to hold a write lock.
         */
        template<typename T, typename... Args> inline EN_IF_CLS(T, elem_seek_pair) insert(off_t, const writeLock_t&, const T& cls, Args&&...);
        template<typename T, typename... Args> inline EN_IF_CLS(T, elem_seek_pair) insert(off_t, const writeLock_t&, const status_t<prepared_insert_t>&, const T& cls, Args&&...);

    private:
        template<typename T, typename... Args>
        inline std::enable_if_t<std::is_base_of<ebmlLazyLoadType, T>::value, std::pair<ebmlElement_sp, seekData_t*>> _insert(off_t offset, const T& cls, vintWidth_t sizeWidth, size_t dataSize, Args... args);

        template<typename T, typename... Args>
        inline std::enable_if_t<!std::is_base_of<ebmlLazyLoadType, T>::value && std::is_base_of<ebmlElementType, T>::value, std::pair<ebmlElement_sp, seekData_t*>>_insert(off_t, const T& cls, Args...);

        /* MOVING */

    public:
        status_t<prepared_move_t> canMove(off_t, off_t, const writeLock_t&);
        void move(off_t, off_t);
        void move(off_t, off_t, const readLock_t&);
        void move(off_t, off_t, const writeLock_t&);
        void move(off_t, off_t, const writeLock_t&, const status_t<prepared_move_t>&);

    private:
        status_t<prepared_move_t> _canMove(off_t, off_t);
        void _move(off_t, off_t, const status_t<prepared_move_t>&);

        /* RESIZING */

    public:
        inline bool sizeSupported(size_t size, const writeLock_t&);
        inline status_t<prepared_resize_t> canResize(size_t, const writeLock_t&);
        inline void resize(size_t size);
        inline void resize(size_t size, const readLock_t&);
        inline void resize(size_t size, const writeLock_t&);
        inline void resize(size_t size, const writeLock_t&, const status_t<prepared_resize_t>&);

    private:
        inline bool _sizeSupported(size_t size);
        status_t<prepared_resize_t> _canResize(size_t);
        inline void _resize(size_t size);
        void _resize(size_t size, const status_t<prepared_resize_t>&);

        /* REMOVAL */

    public:
        // prepared_remove_t prepareRemove(off_t, off_t, const writeLock_t&);
        inline status_t<prepared_remove_t> canRemove(off_t, const writeLock_t&);
        inline void remove(off_t offset);
        inline void remove(off_t offset, const readLock_t&);
        inline void remove(off_t offset, const writeLock_t&);
        inline void remove(off_t offset, const status_t<prepared_remove_t>&, const writeLock_t&);
        // void remove(off_t offset, const writeLock_t&, const prepared_remove_t&);

    private:
        status_t<prepared_remove_t> _canRemove(off_t);
        inline void _remove(off_t offset);
        void _remove(off_t offset, const status_t<prepared_remove_t>&);

        /* BLOCK BOUNDARY SEARCHING */

    public:
        off_t findBoundary(off_t offset);
        off_t findBoundary(off_t offset, const readLock_t&);
        off_t findBoundary(off_t offset, const writeLock_t&);

    private:
        off_t _findBoundary(off_t offset);

    public:
        off_t rfindBoundary(off_t offset);
        off_t rfindBoundary(off_t offset, const readLock_t&);
        off_t rfindBoundary(off_t offset, const writeLock_t&);

    private:
        off_t _rfindBoundary(off_t offset);

        /* CHILD NAVIGATION*/

    public:
        seekData_t* firstChild();
        seekData_t* firstChild(const readLock_t&);
        seekData_t* firstChild(const writeLock_t&);

    private:
        seekData_t* _firstChild();

    public:
        seekData_t* lastChild();
        seekData_t* lastChild(const readLock_t&);
        seekData_t* lastChild(const writeLock_t&);

    private:
        seekData_t* _lastChild();

    public:
        seekData_t* nextChild(off_t offset);
        seekData_t* nextChild(off_t offset, const readLock_t&);
        seekData_t* nextChild(off_t offset, const writeLock_t&);

    private:
        seekData_t* _nextChild(off_t offset);

    public:
        seekData_t* prevChild(off_t offset);
        seekData_t* prevChild(off_t offset, const readLock_t&);
        seekData_t* prevChild(off_t offset, const writeLock_t&);

    private:
        seekData_t* _prevChild(off_t offset);

        /* SPACE MANAGEMENT */

        off_t findFree(off_t offset, size_t size, const writeLock_t&);
        off_t rfindFree(off_t offset, size_t size, const writeLock_t&);

        // status_t<prepared_insert_t> canResizeChild(const seekData_t&, size_t size);
        // status_t<prepared_insert_t> canResizeChild(ebmlElement* child, size_t size);

#if HAS_FALLOCATE
        // Range insertion functions
    public:
        inline status_t<prepared_fallocate_t> canInsertRange(off_t offset, size_t size, const writeLock_t&);
        inline void insertRange(off_t offset, size_t size);
        inline void insertRange(off_t offset, size_t size, const readLock_t&);
        inline void insertRange(off_t offset, size_t size, const writeLock_t&);
        inline void insertRange(off_t offset, size_t size, const status_t<prepared_fallocate_t>&, const writeLock_t&);

    private:
        status_t<prepared_fallocate_t> _canInsertRange(off_t offset, size_t size);
        inline void _insertRange(off_t offset, size_t size);
        void _insertRange(off_t offset, size_t size, const status_t<prepared_fallocate_t>&);

        // Range collapse functions
    public:
        inline status_t<prepared_fallocate_t> canCollapseRange(off_t offset, size_t size, const writeLock_t&);
        inline void collapseRange(off_t offset, size_t size);
        inline void collapseRange(off_t offset, size_t size, const readLock_t&);
        inline void collapseRange(off_t offset, size_t size, const writeLock_t&);
        inline void collapseRange(off_t offset, size_t size, const status_t<prepared_fallocate_t>&, const writeLock_t&);

    private:
        status_t<prepared_fallocate_t> _canCollapseRange(off_t offset, size_t size);
        inline void _collapseRange(off_t offset, size_t size);
        void _collapseRange(off_t offset, size_t size, const status_t<prepared_fallocate_t>&);

        // Hole punching functions
    public:
        inline status_t<prepared_fallocate_t> canPunchHole(off_t offset, size_t size, const writeLock_t&);
        inline void punchHole(off_t offset, size_t size);
        inline void punchHole(off_t offset, size_t size, const readLock_t&);
        inline void punchHole(off_t offset, size_t size, const writeLock_t&);
        inline void punchHole(off_t offset, size_t size, const status_t<prepared_fallocate_t>&, const writeLock_t&);

    private:
        status_t<prepared_fallocate_t> _canPunchHole(off_t offset, size_t size);
        inline void _punchHole(off_t offset, size_t size);
        void _punchHole(off_t offset, size_t size, const status_t<prepared_fallocate_t>&);
#endif

    public:
        // Members from ebmlElement
        std::wstring minirepr() const override;
        size_t dataSize() const override;

        // Child navigation functions
        seekData_t* _firstChildSeek() const;
        seekData_t* _lastChildSeek() const;
        seekData_t* _prevChildSeek(off_t offset, bool strict=true) const;
        seekData_t* _nextChildSeek(off_t offset, bool strict=true) const;

        std::pair<children_m::iterator, children_m::iterator> _find(off_t offset, bool left=true);
        children_m::iterator _findNext(off_t offset, bool strict=true);
        children_m::iterator _findPrev(off_t offset, bool strict=true);
        // std::pair<seekData_t*, seekData_t*> _adjacentChildSeeks(off_t, bool left=true) const;

        // Space management functions
        off_t _findFree(off_t offset, size_t size);
        off_t _rfindFree(off_t offset, size_t size);

        prepared_resize_t _canResizeChild(off_t, size_t size);
        prepared_resize_t _canResizeChild(const seekData_t&, size_t size);

    protected:
        void _handleParseFile(const parseFile&) override;
        void _handleSeekData(std::unique_ptr<seekData_t>&&);
        virtual void _handleSeekData(std::unique_ptr<seekData_t>&&, const children_m::iterator&);
        void _addChild(const ebmlElement_sp&) override;
        void _addChild(ebmlElement_sp&&) override;

        ebmlMasterElement::_iterator* _begin() override;
        ebmlMasterElement::_iterator* _end() override;
        ebmlMasterElement::_const_iterator* _cbegin() const override;
        ebmlMasterElement::_const_iterator* _cend() const override;

        ebml::ptr<ebmlElement> _read_elem(seekData_t& seekData);
        ebml::ptr<const ebmlElement> _read_elem(seekData_t& seekData) const;

        friend class ebmlLazyLoadType;
        friend class seekData_t;
    };

    template<SEEKMAPDEFAULTS>
    class ebmlLazyLoadSeekHelper : public seekHelperByEBMLID<K,  H,  E,  A> {
    public:
        seekData_t* makeSeekData(ebmlElement& elem) const = 0;
        seekData_t* makeSeekData(const ebmlElement& elem) const = 0;
        // seekData_t* makeSeekData(ebmlElement& elem, ebmlElement& child) const = 0;
        seekData_t* makeSeekData(const ebmlElement& elem, const ebmlElement& child) const;
    };

    inline ebml::ptr<ebmlElement> ebmlLazyLoad::get(off_t offset) {
        auto lock = getRLock();
        return _get(offset);
    }

    inline ebml::ptr<ebmlElement> ebmlLazyLoad::get(off_t offset, const readLock_t& lock) {
        auto newlocks = getRLock(lock);
        return _get(offset);
    }

    inline ebml::ptr<ebmlElement> ebmlLazyLoad::get(off_t offset, const writeLock_t& lock) {
        _verifyLock(lock);
        return _get(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::get(off_t offset) {
        auto lock = getRLock();
        return _get<T>(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::get(off_t offset, const readLock_t& lock) {
        auto newlocks = getRLock(lock);
        return _get<T>(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::get(off_t offset, const writeLock_t& lock) {
        _verifyLock(lock);
        return _get<T>(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::_get(off_t offset) {
        return ebml_dynamic_pointer_cast<T>(_get(offset));
    }

    inline ebml::ptr<ebmlElement> ebmlLazyLoad::get(ebmlID_t ebmlID, size_t index) {
        auto lock = getRLock();
        return _get(ebmlID, index);
    }

    inline ebml::ptr<ebmlElement> ebmlLazyLoad::get(ebmlID_t ebmlID, size_t index, const readLock_t& lock) {
        auto newlocks = getRLock(lock);
        return _get(ebmlID, index);
    }

    inline ebml::ptr<ebmlElement> ebmlLazyLoad::get(ebmlID_t ebmlID, size_t index, const writeLock_t& lock) {
        _verifyLock(lock);
        return _get(ebmlID, index);
    }

    inline ebml::ptr<const ebmlElement> ebmlLazyLoad::cget(off_t offset) const {
        auto lock = getRLock();
        return _cget(offset);
    }

    inline ebml::ptr<const ebmlElement> ebmlLazyLoad::cget(off_t offset, const readLock_t& lock) const {
        auto newlocks = getRLock(lock);
        return _cget(offset);
    }

    inline ebml::ptr<const ebmlElement> ebmlLazyLoad::cget(off_t offset, const writeLock_t& lock) const {
        _verifyLock(lock);
        return _cget(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::cget(off_t offset) const {
        auto lock = getRLock();
        return _cget<T>(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::cget(off_t offset, const readLock_t& lock) const {
        auto newlocks = getRLock(lock);
        return _cget<T>(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::cget(off_t offset, const writeLock_t& lock) const {
        _verifyLock(lock);
        return _cget<T>(offset);
    }

    template<typename T> inline ebml::ptr<T> ebmlLazyLoad::_cget(off_t offset) const {
        return ebml_dynamic_pointer_cast<T>(_cget(offset));
    }

    inline ebml::ptr<const ebmlElement> ebmlLazyLoad::cget(ebmlID_t ebmlID, size_t index) const {
        auto lock = getRLock();
        return _cget(ebmlID, index);
    }

    inline ebml::ptr<const ebmlElement> ebmlLazyLoad::cget(ebmlID_t ebmlID, size_t index, const readLock_t& lock) const {
        auto newlocks = getRLock(lock);
        return _cget(ebmlID, index);
    }

    inline ebml::ptr<const ebmlElement> ebmlLazyLoad::cget(ebmlID_t ebmlID, size_t index, const writeLock_t& lock) const {
        _verifyLock(lock);
        return _cget(ebmlID, index);
    }

    template<typename K, typename H, typename E>
    inline EN_IFN_INST(K, ebml::ptr<ebmlElement>) ebmlLazyLoad::getByKey(ebmlID_t ebmlID, const K& key) {
        auto lock = getRLock();
        return _getByKey<K, H, E>(ebmlID, key);
    }

    template<typename K, typename H, typename E>
    inline EN_IFN_INST(K, ebml::ptr<ebmlElement>) ebmlLazyLoad::getByKey(ebmlID_t ebmlID, const K& key, const readLock_t& rlock) {
        auto lock = getRLock(rlock);
        return _getByKey<K, H, E>(ebmlID, key);
    }

    template<typename K, typename H, typename E>
    inline EN_IFN_INST(K, ebml::ptr<ebmlElement>) ebmlLazyLoad::getByKey(ebmlID_t ebmlID, const K& key, const writeLock_t& wlock) {
        _verifyLock(wlock);
        return _getByKey<K, H, E>(ebmlID, key);
    }

    template<typename K, typename H, typename E>
    EN_IFN_INST(K, ebml::ptr<ebmlElement>) ebmlLazyLoad::_getByKey(ebmlID_t ebmlID, const K& key) {
        if (this->_children_by_key.count(ebmlID) == 0) {
            std::string errmsg = "element not found with ebmlID ";
            errmsg += std::to_string(ebmlID);
            errmsg += " and key ";
            errmsg += pack(ebml::repr(key));
            throw ebmlDecodeError(errmsg, DECODE_ERR_DEFAULT, -1);
        }

        auto& seekmap = this->_children_by_key.at(ebmlID)->withKeyType<K, H, E>();

        if (seekmap.count(key) == 0) {
            std::string errmsg = "element not found with ebmlID ";
            errmsg += std::to_string(ebmlID);
            errmsg += " and key ";
            errmsg += pack(ebml::repr(key));
            throw ebmlDecodeError(errmsg, DECODE_ERR_DEFAULT, -1);
        }

        auto seekData = seekmap.at(key);

        return seekData->get();
    }

    template<typename T, typename K, typename H, typename E>
    inline EN_IF_INST(T, ebml::ptr<T>) ebmlLazyLoad::getByKey(ebmlID_t ebmlID, const K& key) {
        auto lock = getRLock();
        return _getByKey<T, K, H, E>(ebmlID, key);
    }

    template<typename T, typename K, typename H, typename E>
    inline EN_IF_INST(T, ebml::ptr<T>)ebmlLazyLoad::_getByKey(ebmlID_t ebmlID, const K& key) {
        return ebml_dynamic_pointer_cast<T>(this->_getByKey<K, H, E>(ebmlID, key));
    }

    inline status_t<prepared_insert_t> ebmlLazyLoad::canInsert(off_t offset, const ebml::ptr<ebmlElement>& elem_p, const writeLock_t& lock) {
        _verifyLock(lock);
        return _canInsert(offset, elem_p);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const ebml::ptr<ebmlElement>& elem_p) {
        auto lock = getWLock();
        return _insert(offset, elem_p);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const ebml::ptr<ebmlElement>& elem_p, const readLock_t& lock) {
        auto _lock = getWLock(lock);
        return _insert(offset, elem_p);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const ebml::ptr<ebmlElement>& elem_p, const writeLock_t& lock) {
        _verifyLock(lock);
        return _insert(offset, elem_p);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const ebml::ptr<ebmlElement>& elem_p, const writeLock_t& lock, const status_t<prepared_insert_t>& prepared) {
        _verifyLock(lock);
        return _insert(offset, elem_p, prepared);
    }

    inline seekData_t* ebmlLazyLoad::_insert(off_t offset, const ebml::ptr<ebmlElement>& elem_p) {
        auto prepared = _canInsert(offset, elem_p);
        return _insert(offset, elem_p, prepared);
    }

    // Insert from std::string

    inline status_t<prepared_insert_t> ebmlLazyLoad::canInsert(off_t offset, const std::string& data, const writeLock_t& lock) {
        _verifyLock(lock);
        return _canInsert(offset, data);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const std::string& data) {
        auto lock = getWLock();
        auto prepared = _canInsert(offset, data);
        return _insert(offset, data, prepared);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const std::string& data, const readLock_t& lock) {
        auto _lock = getWLock(lock);
        auto prepared = _canInsert(offset, data);
        return _insert(offset, data, prepared);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const std::string& data, const writeLock_t& lock) {
        _verifyLock(lock);
        auto prepared = _canInsert(offset, data);
        return _insert(offset, data, prepared);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const std::string& data, const writeLock_t& lock, const status_t<prepared_insert_t>& status) {
        _verifyLock(lock);
        return _insert(offset, data, status );
    }

    inline seekData_t* ebmlLazyLoad::_insert(off_t offset, const std::string& data, const status_t<prepared_insert_t>& status) {
        return _insert(offset, data.data(), status);
    }

    // Insert from const char*

    inline status_t<prepared_insert_t> ebmlLazyLoad::canInsert(off_t offset, const char* data, const writeLock_t& lock) {
        _verifyLock(lock);
        return _canInsert(offset, data);
    }

    inline status_t<prepared_insert_t> ebmlLazyLoad::_canInsert(off_t offset, const char* data) {
        auto parsed = parseString(data, UNKNOWN);
        auto sizetree = sizetree_t(parsed.ebmlIDWidth, parsed.sizeWidth, parsed.dataSize);
        return _canInsert(offset, std::move(sizetree));
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const char* data) {
        auto lock = getWLock();
        auto prepared = _canInsert(offset, data);
        return _insert(offset, data, prepared);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const char* data, const readLock_t& lock) {
        auto _lock = getWLock(lock);
        auto prepared = _canInsert(offset, data);
        return _insert(offset, data, prepared);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const char* data, const writeLock_t& lock) {
        _verifyLock(lock);
        auto prepared = _canInsert(offset, data);
        return _insert(offset, data, prepared);
    }

    inline seekData_t* ebmlLazyLoad::insert(off_t offset, const char* data, const writeLock_t& lock, const status_t<prepared_insert_t>& status) {
        _verifyLock(lock);
        return _insert(offset, data, status );
    }

    inline status_t<prepared_remove_t> ebmlLazyLoad::canRemove(off_t offset, const writeLock_t& wlock) {
        _verifyLock(wlock);
        return _canRemove(offset);
    }

    inline void ebmlLazyLoad::remove(off_t offset) {
        auto lock = getWLock();
        _remove(offset);
    }

    inline void ebmlLazyLoad::remove(off_t offset, const readLock_t& rlock) {
        auto lock = getWLock(rlock);
        _remove(offset);
    }

    inline void ebmlLazyLoad::remove(off_t offset, const writeLock_t& wlock) {
        _verifyLock(wlock);
        _remove(offset);
    }

    inline void ebmlLazyLoad::remove(off_t offset, const status_t<prepared_remove_t>& status, const writeLock_t& wlock) {
        _verifyLock(wlock);
        _remove(offset, status);
    }

    inline void ebmlLazyLoad::_remove(off_t offset) {
        auto status = _canRemove(offset);
        _remove(offset, status);
    }

    inline bool ebmlLazyLoad::sizeSupported(size_t size, const writeLock_t& lock) {
        _verifyLock(lock);
        return _sizeSupported(size);
    }

    inline bool ebmlLazyLoad::_sizeSupported(size_t size) {
        if (size == UNKNOWN) {
            return true;
        }

        return widthAsVint(size) <= this->_sizeWidth;
    }

    inline void ebmlLazyLoad::resize(size_t size) {
        auto lock = getWLock();
        _resize(size);
    }

    inline void ebmlLazyLoad::resize(size_t size, const readLock_t& rlock) {
        auto lock = getWLock(rlock);
        _resize(size);
    }

    inline void ebmlLazyLoad::_resize(size_t size) {
        auto status = _canResize(size);
        _resize(size, status);
    }

    template<typename T, typename... Args>
    inline EN_IF_CLS(T, elem_seek_pair) ebmlLazyLoad::insert(off_t offset, const T& cls, Args&&... args) {
        auto lock = getWLock();
        return _insert(offset, cls, args...);
    }

    template<typename T, typename... Args>
    inline EN_IF_CLS(T, elem_seek_pair) ebmlLazyLoad::insert(off_t offset, const readLock_t& rlock, const T& cls, Args&&... args) {
        auto lock = getWLock(rlock);
        return _insert(offset, cls, args...);
    }

    template<typename T, typename... Args>
    inline EN_IF_CLS(T, elem_seek_pair) ebmlLazyLoad::insert(off_t offset, const writeLock_t& wlock, const T& cls, Args&&... args) {
        _verifyLock(wlock);
        return _insert(offset, cls, args...);
    }

    template<typename T, typename... Args>
    inline std::enable_if_t<std::is_base_of<ebmlLazyLoadType, T>::value, std::pair<ebmlElement_sp, seekData_t*>>
    ebmlLazyLoad::_insert(off_t offset, const T& cls, vintWidth_t sizeWidth, size_t dataSize, Args... args) {
        sizetree_t sizetree = {widthAsVint(cls.ebmlID), sizeWidth, dataSize};
        auto query = _canInsert(offset, std::move(sizetree));
        auto& data = query.data();

        if (!query) {
            auto elem = ebml_static_pointer_cast<const ebmlElement>(sp());
            query.throw_exc(elem);
            // ebmlInsertionError exc = query;
            // throw exc;
        }

        auto& f = file();
        auto poffset = this->dataOffsetInFile();

        if (data.prev.endOffset < offset) {
            Void.writeVoid(f, poffset + data.prev.endOffset, offset - data.prev.endOffset);
        }

        auto elem = ebml_static_pointer_cast<ebmlElement>(cls(*this, offset, sizeWidth, dataSize, args...));
        // this->_attachChild(elem);
        // elem->_offsetInParent = offset;

        if (data.extent.endOffset < data.next) {
            Void.writeVoid(f, poffset + data.extent.endOffset, data.next - data.extent.endOffset);
        }

        seekData_t* seekData = elem->makeSeekData();
        auto seekData_up = std::unique_ptr<seekData_t>(seekData);
        this->_handleSeekData(std::move(seekData_up));
        return {std::move(elem), seekData};
    }

    template<typename T, typename... Args>
    inline std::enable_if_t<!std::is_base_of<ebmlLazyLoadType, T>::value && std::is_base_of<ebmlElementType, T>::value, std::pair<ebmlElement_sp, seekData_t*>>
    ebmlLazyLoad::_insert(off_t offset, const T& cls, Args... args) {
        auto elem = ebml_static_pointer_cast<ebmlElement>(cls(args...));
        status_t<prepared_insert_t> status = _canInsert(offset, elem);
        auto seek = _insert(offset, elem, status);
        return {std::move(elem), seek};
    }

    // template<typename T>
    // inline EN_IF_CLS(T, elem_seek_pair) ebmlLazyLoad::insert(off_t offset, const ebml::ptr<T>& child) {
    //     return this->insert(offset, ebml_static_pointer_cast<ebmlElement>(child));
    // }
    //
    // template<typename T>
    // inline std::enable_if_t<!std::is_base_of<ebmlLazyLoad, T>::value && std::is_base_of<ebmlElement, T>::value, seekData_t*>
    // ebmlLazyLoad::_insert(off_t offset, const ebml::ptr<T>& child) {
    //     return this->_insert(offset, ebml_static_pointer_cast<ebmlElement>(child));
    // }
}

#endif
