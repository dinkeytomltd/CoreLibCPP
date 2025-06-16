
#pragma once
/*
 * LinkedList.h
 * C++ implementation of .NET's System.Collections.Generic.LinkedList<T> class
 * 
 * Key .NET LinkedList<T> characteristics implemented:
 * - Doubly-linked list collection for efficient insertion/removal
 * - O(1) insertion/removal at any position with node reference
 * - LinkedListNode<T> class for node-based operations
 * - Type-safe generic implementation
 */
#include <stdexcept>
#include <memory>
#include <functional>
#include "../../../System/Object.h"
#include "../../../System/String.h"
#include "LinkedListNode.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename T>
class LinkedList {
private:
    LinkedListNode<T>* m_head;
    LinkedListNode<T>* m_tail;
    size_t m_count;

    // Helper method to create a new node
    LinkedListNode<T>* CreateNode(const T& value) {
        auto node = new LinkedListNode<T>(value);
        node->m_list = this;
        return node;
    }
    
    LinkedListNode<T>* CreateNode(T&& value) {
        auto node = new LinkedListNode<T>(std::move(value));
        node->m_list = this;
        return node;
    }

public:
    // Constructors
    LinkedList() : m_head(nullptr), m_tail(nullptr), m_count(0) {}
    
    // Constructor with initializer list
    LinkedList(std::initializer_list<T> items) : LinkedList() {
        for (const auto& item : items) {
            AddLast(item);
        }
    }
    
    // Copy constructor
    LinkedList(const LinkedList<T>& other) : LinkedList() {
        for (auto current = other.m_head; current != nullptr; current = current->m_next) {
            AddLast(current->m_value);
        }
    }
    
    // Assignment operator
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            Clear();
            for (auto current = other.m_head; current != nullptr; current = current->m_next) {
                AddLast(current->m_value);
            }
        }
        return *this;
    }
    
    // Move constructor
    LinkedList(LinkedList<T>&& other) noexcept 
        : m_head(other.m_head), m_tail(other.m_tail), m_count(other.m_count) {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_count = 0;
        // Update list references
        for (auto current = m_head; current != nullptr; current = current->m_next) {
            current->m_list = this;
        }
    }
    
    // Move assignment operator
    LinkedList<T>& operator=(LinkedList<T>&& other) noexcept {
        if (this != &other) {
            Clear();
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_count = other.m_count;
            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_count = 0;
            // Update list references
            for (auto current = m_head; current != nullptr; current = current->m_next) {
                current->m_list = this;
            }
        }
        return *this;
    }
    
    // Destructor
    ~LinkedList() { Clear(); }
    
    // Properties
    
    // Count property (equivalent to .NET LinkedList<T>.Count)
    [[nodiscard]] size_t Count() const {
        return m_count;
    }
    
    // First property (equivalent to .NET LinkedList<T>.First)
    [[nodiscard]] LinkedListNode<T>* First() const {
        return m_head;
    }
    
    // Last property (equivalent to .NET LinkedList<T>.Last)
    [[nodiscard]] LinkedListNode<T>* Last() const {
        return m_tail;
    }
    
    // Methods
    
    // AddFirst method (equivalent to .NET LinkedList<T>.AddFirst)
    LinkedListNode<T>* AddFirst(const T& value) {
        auto newNode = CreateNode(value);
        
        if (m_head == nullptr) {
            m_head = newNode;
            m_tail = newNode;
        } else {
            newNode->m_next = m_head;
            m_head->m_previous = newNode;
            m_head = newNode;
        }
        
        ++m_count;
        return newNode;
    }
    
    LinkedListNode<T>* AddFirst(T&& value) {
        auto newNode = CreateNode(std::move(value));
        
        if (m_head == nullptr) {
            m_head = newNode;
            m_tail = newNode;
        } else {
            newNode->m_next = m_head;
            m_head->m_previous = newNode;
            m_head = newNode;
        }
        
        ++m_count;
        return newNode;
    }
    
    // AddLast method (equivalent to .NET LinkedList<T>.AddLast)
    LinkedListNode<T>* AddLast(const T& value) {
        auto newNode = CreateNode(value);
        
        if (m_tail == nullptr) {
            m_head = newNode;
            m_tail = newNode;
        } else {
            newNode->m_previous = m_tail;
            m_tail->m_next = newNode;
            m_tail = newNode;
        }
        
        ++m_count;
        return newNode;
    }
    
    LinkedListNode<T>* AddLast(T&& value) {
        auto newNode = CreateNode(std::move(value));
        
        if (m_tail == nullptr) {
            m_head = newNode;
            m_tail = newNode;
        } else {
            newNode->m_previous = m_tail;
            m_tail->m_next = newNode;
            m_tail = newNode;
        }
        
        ++m_count;
        return newNode;
    }
    
    // AddBefore method (equivalent to .NET LinkedList<T>.AddBefore)
    LinkedListNode<T>* AddBefore(LinkedListNode<T>* node, const T& value) {
        if (node == nullptr || node->m_list != this) {
            throw std::invalid_argument("Node is not in the current LinkedList");
        }
        
        auto newNode = CreateNode(value);
        
        newNode->m_next = node;
        newNode->m_previous = node->m_previous;
        
        if (node->m_previous != nullptr) {
            node->m_previous->m_next = newNode;
        } else {
            m_head = newNode;
        }
        
        node->m_previous = newNode;
        ++m_count;
        return newNode;
    }
    
    // AddAfter method (equivalent to .NET LinkedList<T>.AddAfter)
    LinkedListNode<T>* AddAfter(LinkedListNode<T>* node, const T& value) {
        if (node == nullptr || node->m_list != this) {
            throw std::invalid_argument("Node is not in the current LinkedList");
        }
        
        auto newNode = CreateNode(value);
        
        newNode->m_previous = node;
        newNode->m_next = node->m_next;
        
        if (node->m_next != nullptr) {
            node->m_next->m_previous = newNode;
        } else {
            m_tail = newNode;
        }
        
        node->m_next = newNode;
        ++m_count;
        return newNode;
    }
    
    // Remove method (equivalent to .NET LinkedList<T>.Remove)
    bool Remove(const T& value) {
        auto node = Find(value);
        if (node != nullptr) {
            Remove(node);
            return true;
        }
        return false;
    }
    
    void Remove(LinkedListNode<T>* node) {
        if (node == nullptr || node->m_list != this) {
            throw std::invalid_argument("Node is not in the current LinkedList");
        }
        
        if (node->m_previous != nullptr) {
            node->m_previous->m_next = node->m_next;
        } else {
            m_head = node->m_next;
        }
        
        if (node->m_next != nullptr) {
            node->m_next->m_previous = node->m_previous;
        } else {
            m_tail = node->m_previous;
        }
        
        node->m_list = nullptr;
        --m_count;
        delete node;
    }
    
    // RemoveFirst method (equivalent to .NET LinkedList<T>.RemoveFirst)
    void RemoveFirst() {
        if (m_head == nullptr) {
            throw std::runtime_error("LinkedList is empty");
        }
        Remove(m_head);
    }
    
    // RemoveLast method (equivalent to .NET LinkedList<T>.RemoveLast)
    void RemoveLast() {
        if (m_tail == nullptr) {
            throw std::runtime_error("LinkedList is empty");
        }
        Remove(m_tail);
    }
    
    // Find method (equivalent to .NET LinkedList<T>.Find)
    [[nodiscard]] LinkedListNode<T>* Find(const T& value) const {
        for (auto current = m_head; current != nullptr; current = current->m_next) {
            if (current->m_value == value) {
                return current;
            }
        }
        return nullptr;
    }
    
    // FindLast method (equivalent to .NET LinkedList<T>.FindLast)
    [[nodiscard]] LinkedListNode<T>* FindLast(const T& value) const {
        for (auto current = m_tail; current != nullptr; current = current->m_previous) {
            if (current->m_value == value) {
                return current;
            }
        }
        return nullptr;
    }
    
    // Contains method (equivalent to .NET LinkedList<T>.Contains)
    [[nodiscard]] bool Contains(const T& value) const {
        return Find(value) != nullptr;
    }
    
    // Clear method (equivalent to .NET LinkedList<T>.Clear)
    void Clear() {
        while (m_head != nullptr) {
            auto next = m_head->m_next;
            m_head->m_list = nullptr;
            delete m_head;
            m_head = next;
        }
        m_tail = nullptr;
        m_count = 0;
    }
    
    // Iterator class for range-based for loops
    class iterator {
    private:
        LinkedListNode<T>* m_current;
        
    public:
        explicit iterator(LinkedListNode<T>* node) : m_current(node) {}
        
        T& operator*() { return m_current->m_value; }
        const T& operator*() const { return m_current->m_value; }
        
        iterator& operator++() {
            m_current = m_current->m_next;
            return *this;
        }
        
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const iterator& other) const {
            return m_current == other.m_current;
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };
    
    class const_iterator {
    private:
        const LinkedListNode<T>* m_current;
        
    public:
        explicit const_iterator(const LinkedListNode<T>* node) : m_current(node) {}
        
        const T& operator*() const { return m_current->m_value; }
        
        const_iterator& operator++() {
            m_current = m_current->m_next;
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const const_iterator& other) const {
            return m_current == other.m_current;
        }
        
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
    };
    
    // Iterator support (for range-based for loops)
    [[nodiscard]] iterator begin() { return iterator(m_head); }
    [[nodiscard]] iterator end() { return iterator(nullptr); }
    [[nodiscard]] const_iterator begin() const { return const_iterator(m_head); }
    [[nodiscard]] const_iterator end() const { return const_iterator(nullptr); }
    [[nodiscard]] const_iterator cbegin() const { return const_iterator(m_head); }
    [[nodiscard]] const_iterator cend() const { return const_iterator(nullptr); }
    
    // ToString method (for debugging)
    [[nodiscard]] System::String ToString() const {
        return System::String("System.Collections.Generic.LinkedList`1[T]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
