
#pragma once

#include "System/EventArgs.h"
#include "System/Event.h"

namespace System::Interfaces
{
    /// <summary>
    /// Notifies clients that a property value has changed.
    /// </summary>
    class INotifyPropertyChanged
    {
    public:
        /// <summary>
        /// Virtual destructor.
        /// </summary>
        virtual ~INotifyPropertyChanged() = default;

        /// <summary>
        /// Adds a PropertyChanged event handler.
        /// </summary>
        /// <param name="handler">The event handler to add.</param>
        virtual void add_PropertyChanged(const PropertyChangedEventHandler& handler) = 0;

        /// <summary>
        /// Removes a PropertyChanged event handler.
        /// </summary>
        /// <param name="handler">The event handler to remove.</param>
        virtual void remove_PropertyChanged(const PropertyChangedEventHandler& handler) = 0;

    protected:
        /// <summary>
        /// Raises the PropertyChanged event.
        /// </summary>
        /// <param name="args">The event arguments containing the property name.</param>
        virtual void OnPropertyChanged(PropertyChangedEventArgs* args) = 0;
    };

    /// <summary>
    /// Notifies listeners of dynamic changes, such as when items get added and removed or the whole list is refreshed.
    /// </summary>
    class INotifyCollectionChanged
    {
    public:
        /// <summary>
        /// Virtual destructor.
        /// </summary>
        virtual ~INotifyCollectionChanged() = default;

        /// <summary>
        /// Adds a CollectionChanged event handler.
        /// </summary>
        /// <param name="handler">The event handler to add.</param>
        virtual void add_CollectionChanged(const CollectionChangedEventHandler& handler) = 0;

        /// <summary>
        /// Removes a CollectionChanged event handler.
        /// </summary>
        /// <param name="handler">The event handler to remove.</param>
        virtual void remove_CollectionChanged(const CollectionChangedEventHandler& handler) = 0;

    protected:
        /// <summary>
        /// Raises the CollectionChanged event.
        /// </summary>
        /// <param name="args">The event arguments containing the change information.</param>
        virtual void OnCollectionChanged(CollectionChangedEventArgs* args) = 0;
    };
}
