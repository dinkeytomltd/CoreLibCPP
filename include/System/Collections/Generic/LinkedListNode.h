
#pragma once
/*
 * LinkedListNode.h
 * C++ implementation of .NET's System.Collections.Generic.LinkedListNode<T> class
 * 
 * Key .NET LinkedListNode<T> characteristics implemented:
 * - Node class for LinkedList<T>
 * - Contains Value, Next, Previous properties
 * - Provides access to the containing LinkedList
 */
#include <memory>
#include "../../../System/Object.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename T>
class LinkedList; // Forward declaration

template <typename T>
class LinkedListNode {
    friend class LinkedList<T>;

private:
    T m_value;
    LinkedListNode<T>* m_next;
    LinkedListNode<T>* m_previous;
    LinkedList<T>* m_list;

public:
    // Constructor
    explicit LinkedListNode(const T& value) 
        : m_value(value), m_next(nullptr), m_previous(nullptr), m_list(nullptr) {}
    
    explicit LinkedListNode(T&& value) 
        : m_value(std::move(value)), m_next(nullptr), m_previous(nullptr), m_list(nullptr) {}
    
    // Copy constructor
    LinkedListNode(const LinkedListNode<T>& other) = delete;
    
    // Assignment operator
    LinkedListNode<T>& operator=(const LinkedListNode<T>& other) = delete;
    
    // Move constructor
    LinkedListNode(LinkedListNode<T>&& other) = delete;
    
    // Move assignment operator
    LinkedListNode<T>& operator=(LinkedListNode<T>&& other) = delete;
    
    // Destructor
    ~LinkedListNode() = default;
    
    // Properties
    
    // Value property (equivalent to .NET LinkedListNode<T>.Value)
    [[nodiscard]] T& Value() { return m_value; }
    [[nodiscard]] const T& Value() const { return m_value; }
    
    // Next property (equivalent to .NET LinkedListNode<T>.Next)
    [[nodiscard]] LinkedListNode<T>* Next() const { return m_next; }
    
    // Previous property (equivalent to .NET LinkedListNode<T>.Previous)
    [[nodiscard]] LinkedListNode<T>* Previous() const { return m_previous; }
    
    // List property (equivalent to .NET LinkedListNode<T>.List)
    [[nodiscard]] LinkedList<T>* List() const { return m_list; }
};

} // namespace Generic
} // namespace Collections
} // namespace System
