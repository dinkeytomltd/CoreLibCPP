
#pragma once
/*
 * Queue.h
 * C++ implementation of .NET's System.Collections.Generic.Queue<T> class
 * 
 * Key .NET Queue<T> characteristics implemented:
 * - First-in-first-out (FIFO) collection
 * - Type-safe generic implementation
 * - Core collection operations (Enqueue, Dequeue, Peek, etc.)
 * - Exception handling for empty queue operations
 */
#include <queue>
#include <stdexcept>
#include <vector>
#include <memory>
#include "../../../System/Object.h"
#include "../../../System/String.h"

namespace System {
namespace Collections {
namespace Generic {

template <typename T>
class Queue {
private:
    std::queue<T> m_items;

public:
    // Constructors
    Queue() = default;
    
    // Constructor with initial capacity (note: std::queue doesn't support capacity, but we keep for API compatibility)
    explicit Queue(size_t capacity) {
        // std::queue doesn't support reserve, but we keep this constructor for API compatibility
    }
    
    // Constructor with initializer list
    Queue(std::initializer_list<T> items) {
        for (const auto& item : items) {
            m_items.push(item);
        }
    }
    
    // Copy constructor
    Queue(const Queue<T>& other) = default;
    
    // Assignment operator
    Queue<T>& operator=(const Queue<T>& other) = default;
    
    // Move constructor
    Queue(Queue<T>&& other) noexcept = default;
    
    // Move assignment operator
    Queue<T>& operator=(Queue<T>&& other) noexcept = default;
    
    // Destructor
    ~Queue() = default;
    
    // Properties
    
    // Count property (equivalent to .NET Queue<T>.Count)
    [[nodiscard]] size_t Count() const {
        return m_items.size();
    }
    
    // Methods
    
    // Enqueue method (equivalent to .NET Queue<T>.Enqueue)
    void Enqueue(const T& item) {
        m_items.push(item);
    }
    
    void Enqueue(T&& item) {
        m_items.push(std::move(item));
    }
    
    // Dequeue method (equivalent to .NET Queue<T>.Dequeue)
    T Dequeue() {
        if (m_items.empty()) {
            throw std::runtime_error("Queue is empty");
        }
        T item = std::move(m_items.front());
        m_items.pop();
        return item;
    }
    
    // Peek method (equivalent to .NET Queue<T>.Peek)
    [[nodiscard]] const T& Peek() const {
        if (m_items.empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return m_items.front();
    }
    
    [[nodiscard]] T& Peek() {
        if (m_items.empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return m_items.front();
    }
    
    // Clear method (equivalent to .NET Queue<T>.Clear)
    void Clear() {
        while (!m_items.empty()) {
            m_items.pop();
        }
    }
    
    // Contains method (equivalent to .NET Queue<T>.Contains)
    [[nodiscard]] bool Contains(const T& item) const {
        // Note: std::queue doesn't provide iterators, so we need to copy to check
        std::queue<T> temp = m_items;
        while (!temp.empty()) {
            if (temp.front() == item) {
                return true;
            }
            temp.pop();
        }
        return false;
    }
    
    // ToArray method (equivalent to .NET Queue<T>.ToArray)
    [[nodiscard]] std::vector<T> ToArray() const {
        std::vector<T> result;
        std::queue<T> temp = m_items;
        while (!temp.empty()) {
            result.push_back(temp.front());
            temp.pop();
        }
        return result;
    }
    
    // TryDequeue method (not in .NET but useful)
    bool TryDequeue(T& item) {
        if (m_items.empty()) {
            return false;
        }
        item = std::move(m_items.front());
        m_items.pop();
        return true;
    }
    
    // TryPeek method (not in .NET but useful)
    bool TryPeek(T& item) const {
        if (m_items.empty()) {
            return false;
        }
        item = m_items.front();
        return true;
    }
    
    // ToString method (for debugging)
    [[nodiscard]] System::String ToString() const {
        return System::String("System.Collections.Generic.Queue`1[T]");
    }
};

} // namespace Generic
} // namespace Collections
} // namespace System
