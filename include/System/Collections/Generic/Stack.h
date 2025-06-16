
#pragma once
/*
 * Stack.h
 * C++ implementation of .NET's System.Collections.Generic.Stack<T> class
 * 
 * Key .NET Stack<T> characteristics implemented:
 * - Last-in-first-out (LIFO) collection
 * - Type-safe generic implementation
 * - Core collection operations (Push, Pop, Peek, etc.)
 * - Exception handling for empty stack operations
 */
#include <stack>
#include <stdexcept>
#include <vector>
#include <memory>
#include "../../../System/Object.h"
#include "../../../System/String.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename T>
class Stack {
private:
    std::stack<T> m_items;

public:
    // Constructors
    Stack() = default;
    
    // Constructor with initial capacity (note: std::stack doesn't support capacity, but we keep for API compatibility)
    explicit Stack(size_t capacity) {
        // std::stack doesn't support reserve, but we keep this constructor for API compatibility
    }
    
    // Constructor with initializer list
    Stack(std::initializer_list<T> items) {
        for (const auto& item : items) {
            m_items.push(item);
        }
    }
    
    // Copy constructor
    Stack(const Stack<T>& other) = default;
    
    // Assignment operator
    Stack<T>& operator=(const Stack<T>& other) = default;
    
    // Move constructor
    Stack(Stack<T>&& other) noexcept = default;
    
    // Move assignment operator
    Stack<T>& operator=(Stack<T>&& other) noexcept = default;
    
    // Destructor
    ~Stack() = default;
    
    // Properties
    
    // Count property (equivalent to .NET Stack<T>.Count)
    [[nodiscard]] size_t Count() const {
        return m_items.size();
    }
    
    // Methods
    
    // Push method (equivalent to .NET Stack<T>.Push)
    void Push(const T& item) {
        m_items.push(item);
    }
    
    void Push(T&& item) {
        m_items.push(std::move(item));
    }
    
    // Pop method (equivalent to .NET Stack<T>.Pop)
    T Pop() {
        if (m_items.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        T item = std::move(m_items.top());
        m_items.pop();
        return item;
    }
    
    // Peek method (equivalent to .NET Stack<T>.Peek)
    [[nodiscard]] const T& Peek() const {
        if (m_items.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return m_items.top();
    }
    
    [[nodiscard]] T& Peek() {
        if (m_items.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return m_items.top();
    }
    
    // Clear method (equivalent to .NET Stack<T>.Clear)
    void Clear() {
        while (!m_items.empty()) {
            m_items.pop();
        }
    }
    
    // Contains method (equivalent to .NET Stack<T>.Contains)
    [[nodiscard]] bool Contains(const T& item) const {
        // Note: std::stack doesn't provide iterators, so we need to copy to check
        std::stack<T> temp = m_items;
        while (!temp.empty()) {
            if (temp.top() == item) {
                return true;
            }
            temp.pop();
        }
        return false;
    }
    
    // ToArray method (equivalent to .NET Stack<T>.ToArray)
    [[nodiscard]] std::vector<T> ToArray() const {
        std::vector<T> result;
        std::stack<T> temp = m_items;
        while (!temp.empty()) {
            result.push_back(temp.top());
            temp.pop();
        }
        return result;
    }
    
    // TryPop method (not in .NET but useful)
    bool TryPop(T& item) {
        if (m_items.empty()) {
            return false;
        }
        item = std::move(m_items.top());
        m_items.pop();
        return true;
    }
    
    // TryPeek method (not in .NET but useful)
    bool TryPeek(T& item) const {
        if (m_items.empty()) {
            return false;
        }
        item = m_items.top();
        return true;
    }
    
    // ToString method (for debugging)
    [[nodiscard]] System::String ToString() const {
        return System::String("System.Collections.Generic.Stack`1[T]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
