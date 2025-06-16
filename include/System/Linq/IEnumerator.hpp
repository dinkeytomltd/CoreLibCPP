
#pragma once

#include "../InvalidOperationException.h"
#include "../NotSupportedException.h"
#include <memory>

namespace System::Linq {

    /**
     * @brief Supports iteration over a generic collection.
     * 
     * This interface provides the mechanism to iterate through a collection.
     * It maintains the state of enumeration and provides methods to advance
     * through the collection and access the current element.
     * 
     * @tparam T The type of objects to enumerate
     */
    template<typename T>
    class IEnumerator {
    public:
        virtual ~IEnumerator() = default;

        /**
         * @brief Gets the element in the collection at the current position of the enumerator.
         * 
         * @return const T& Reference to the current element
         * @throws InvalidOperationException If the enumerator is positioned before the first element or after the last element
         */
        virtual const T& Current() const = 0;

        /**
         * @brief Gets the element in the collection at the current position of the enumerator (non-const version).
         * 
         * @return T& Reference to the current element
         * @throws InvalidOperationException If the enumerator is positioned before the first element or after the last element
         */
        virtual T& Current() = 0;

        /**
         * @brief Advances the enumerator to the next element of the collection.
         * 
         * @return true if the enumerator was successfully advanced to the next element; 
         *         false if the enumerator has passed the end of the collection
         */
        virtual bool MoveNext() = 0;

        /**
         * @brief Sets the enumerator to its initial position, which is before the first element in the collection.
         * 
         * @throws NotSupportedException If the enumerator does not support resetting
         */
        virtual void Reset() = 0;

        /**
         * @brief Performs application-defined tasks associated with freeing, releasing, or resetting resources.
         */
        virtual void Dispose() {}
    };

} // namespace System::Linq
