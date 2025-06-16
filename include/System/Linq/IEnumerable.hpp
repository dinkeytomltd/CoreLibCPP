
#pragma once

#include "../Exception.h"
#include <memory>

namespace System::Linq {

    // Forward declarations
    template<typename T>
    class IEnumerator;

    /**
     * @brief Core enumerable interface that supports iteration over a collection of a specified type.
     * 
     * This interface is the foundation of LINQ operations and provides deferred execution semantics.
     * It exposes an enumerator which supports simple iteration over a collection.
     * 
     * @tparam T The type of objects to enumerate
     */
    template<typename T>
    class IEnumerable {
    public:
        virtual ~IEnumerable() = default;

        /**
         * @brief Returns an enumerator that iterates through the collection.
         * 
         * @return std::unique_ptr<IEnumerator<T>> An enumerator for the collection
         */
        virtual std::unique_ptr<IEnumerator<T>> GetEnumerator() = 0;

        /**
         * @brief Returns a const enumerator that iterates through the collection.
         * 
         * @return std::unique_ptr<IEnumerator<T>> A const enumerator for the collection
         */
        virtual std::unique_ptr<IEnumerator<T>> GetEnumerator() const = 0;
    };

} // namespace System::Linq
