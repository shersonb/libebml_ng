#ifndef EBML_NG_MASTERELEMENT_ITERATION_CPP
#define EBML_NG_MASTERELEMENT_ITERATION_CPP

#include <utility>

#include "libebml_ng/masterelement/base.h"

namespace ebml {
    // ebmlMasterElement::iterator ebmlMasterElement::begin() {
    //     auto iter = this->_begin();
    //     return ebmlMasterElement::iterator(iter);
    // }
    //
    // ebmlMasterElement::iterator ebmlMasterElement::end() {
    //     auto iter = this->_end();
    //     return ebmlMasterElement::iterator(iter);
    // }
    //
    // ebmlMasterElement::iterator::iterator(ebmlMasterElement::_iterator* iter) {
    //     this->_iter = iter;
    // }
    //
    // ebmlMasterElement::iterator::iterator(const ebmlMasterElement::iterator& other) {
    //     this->_iter = other._iter->copy();
    // }
    //
    // ebmlMasterElement::iterator::iterator(ebmlMasterElement::iterator&& other) {
    //     this->_iter = std::exchange(other._iter, nullptr);
    // }
    //
    // ebmlElement& ebmlMasterElement::iterator::operator*() const {
    //     return **this->_iter;
    // }
    //
    // ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator++() {
    //     ++(*this->_iter);
    //     return *this;
    // }
    //
    // ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator=(const ebmlMasterElement::iterator& other) {
    //     if (this != &other) {
    //         this->_iter = other._iter->copy();
    //     }
    //
    //     return *this;
    // }
    //
    // ebmlMasterElement::iterator& ebmlMasterElement::iterator::operator=(ebmlMasterElement::iterator&& other) {
    //     if (this != &other) {
    //         this->_iter = std::exchange(other._iter, nullptr);
    //     }
    //
    //     return *this;
    // }
    //
    // bool ebmlMasterElement::iterator::operator==(const iterator& other) const {
    //     return (*this->_iter) == (*other._iter);
    // }
    //
    // bool ebmlMasterElement::iterator::operator!=(const iterator& other) const {
    //     return (*this->_iter) != (*other._iter);
    // }
    //
    // ebmlMasterElement::iterator::~iterator() {
    //     if (this->_iter != nullptr) {
    //         delete this->_iter;
    //     }
    // }
    //
    // // CONST ITERATORS
    //
    // ebmlMasterElement::const_iterator ebmlMasterElement::cbegin() const {
    //     auto iter = _cbegin();
    //     return ebmlMasterElement::const_iterator(iter);
    // }
    //
    // ebmlMasterElement::const_iterator ebmlMasterElement::cend() const {
    //     auto iter = this->_cend();
    //     return ebmlMasterElement::const_iterator(iter);
    // }
    //
    // ebmlMasterElement::iterator ebmlMasterElement::make_iter(_iterator* _iter) {
    //     return ebmlMasterElement::iterator(_iter);
    // }
    //
    // ebmlMasterElement::const_iterator ebmlMasterElement::make_iter(_const_iterator* _iter) {
    //     return ebmlMasterElement::const_iterator(_iter);
    // }
    //
    // ebmlMasterElement::const_iterator::const_iterator(ebmlMasterElement::_const_iterator* iter) {
    //     this->_iter = iter;
    // }
    //
    // ebmlMasterElement::const_iterator::const_iterator(const ebmlMasterElement::const_iterator& other) {
    //     this->_iter = other._iter->copy();
    // }
    //
    // ebmlMasterElement::const_iterator::const_iterator(ebmlMasterElement::const_iterator&& other) {
    //     this->_iter = std::exchange(other._iter, nullptr);
    // }
    //
    // const ebmlElement& ebmlMasterElement::const_iterator::operator*() const {
    //     return **this->_iter;
    // }
    //
    // ebmlMasterElement::const_iterator& ebmlMasterElement::const_iterator::operator++() {
    //     ++(*this->_iter);
    //     return *this;
    // }
    //
    // ebmlMasterElement::const_iterator& ebmlMasterElement::const_iterator::operator=(const ebmlMasterElement::const_iterator& other) {
    //     if (this != &other) {
    //         this->_iter = other._iter->copy();
    //     }
    //
    //     return *this;
    // }
    //
    // ebmlMasterElement::const_iterator& ebmlMasterElement::const_iterator::operator=(ebmlMasterElement::const_iterator&& other) {
    //     if (this != &other) {
    //         this->_iter = std::exchange(other._iter, nullptr);
    //     }
    //
    //     return *this;
    // }
    //
    // bool ebmlMasterElement::const_iterator::operator==(const const_iterator& other) const {
    //     return (*this->_iter) == (*other._iter);
    // }
    //
    // bool ebmlMasterElement::const_iterator::operator!=(const const_iterator& other) const {
    //     return (*this->_iter) != (*other._iter);
    // }
    //
    // ebmlMasterElement::const_iterator::~const_iterator() {
    //     if (this->_iter != nullptr) {
    //         delete this->_iter;
    //     }
    // }
}
#endif
