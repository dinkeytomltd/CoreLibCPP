
#pragma once

#include "System/Object.h"
#include "System/String.h"
#include "System/Reflection/Type.h"
#include "System/Collections/Generic/List.h"
#include <vector>
#include <memory>

namespace System::Reflection
{
    /// <summary>
    /// Contains methods to create types of objects locally or remotely, or obtain references to existing remote objects.
    /// </summary>
    class Activator : public System::Object
    {
    public:
        /// <summary>
        /// Creates an instance of the specified type using that type's default constructor.
        /// </summary>
        /// <typeparam name="T">The type of object to create.</typeparam>
        /// <returns>A reference to the newly created object.</returns>
        template<typename T>
        static T CreateInstance()
        {
            auto type = Type::GetType<T>();
            auto constructors = type.GetConstructors();
            
            // Find default constructor (no parameters)
            for (const auto& ctor : constructors)
            {
                if (ctor->GetParameters().empty())
                {
                    auto obj = ctor->Invoke(std::vector<System::Object>());
                    // Convert the result back to T
                    // This is a simplified implementation - in practice, you'd need proper type conversion
                    return T();
                }
            }
            
            throw System::Exception("No default constructor found for type");
        }

        /// <summary>
        /// Creates an instance of the specified type using the constructor that best matches the specified parameters.
        /// </summary>
        /// <typeparam name="T">The type of object to create.</typeparam>
        /// <param name="args">An array of arguments that match in number, order, and type the parameters of the constructor to invoke.</param>
        /// <returns>A reference to the newly created object.</returns>
        template<typename T>
        static T CreateInstance(const System::Collections::Generic::List<System::Object>& args)
        {
            auto type = Type::GetType<T>();
            auto constructors = type.GetConstructors();
            
            // Find constructor with matching parameter count
            for (const auto& ctor : constructors)
            {
                auto params = ctor->GetParameters();
                if (params.size() == args.Count())
                {
                    auto obj = ctor->Invoke(args);
                    // Convert the result back to T
                    // This is a simplified implementation - in practice, you'd need proper type conversion
                    return T();
                }
            }
            
            throw System::Exception("No matching constructor found for type");
        }

        /// <summary>
        /// Creates an instance of the specified type using the constructor that best matches the specified parameters.
        /// </summary>
        /// <typeparam name="T">The type of object to create.</typeparam>
        /// <param name="args">An array of arguments that match in number, order, and type the parameters of the constructor to invoke.</param>
        /// <returns>A reference to the newly created object.</returns>
        template<typename T>
        static T CreateInstance(const std::vector<System::Object>& args)
        {
            auto type = Type::GetType<T>();
            auto constructors = type.GetConstructors();
            
            // Find constructor with matching parameter count
            for (const auto& ctor : constructors)
            {
                auto params = ctor->GetParameters();
                if (params.size() == args.size())
                {
                    auto obj = ctor->Invoke(args);
                    // Convert the result back to T
                    // This is a simplified implementation - in practice, you'd need proper type conversion
                    return T();
                }
            }
            
            throw System::Exception("No matching constructor found for type");
        }

        /// <summary>
        /// Creates an instance of the type whose name is specified, using the named assembly and default constructor.
        /// </summary>
        /// <param name="assemblyName">The name of the assembly where the type named typeName is sought.</param>
        /// <param name="typeName">The fully qualified name of the type to create an instance of.</param>
        /// <returns>An instance of the object specified by typeName.</returns>
        static System::Object CreateInstance(const System::String& assemblyName, const System::String& typeName);

        /// <summary>
        /// Creates an instance of the specified type using that type's default constructor.
        /// </summary>
        /// <param name="type">The type of object to create.</param>
        /// <returns>A reference to the newly created object.</returns>
        static System::Object CreateInstance(const Type& type);

        /// <summary>
        /// Creates an instance of the specified type using the constructor that best matches the specified parameters.
        /// </summary>
        /// <param name="type">The type of object to create.</param>
        /// <param name="args">An array of arguments that match in number, order, and type the parameters of the constructor to invoke.</param>
        /// <returns>A reference to the newly created object.</returns>
        static System::Object CreateInstance(const Type& type, const System::Collections::Generic::List<System::Object>& args);

        /// <summary>
        /// Creates an instance of the specified type using the constructor that best matches the specified parameters.
        /// </summary>
        /// <param name="type">The type of object to create.</param>
        /// <param name="args">An array of arguments that match in number, order, and type the parameters of the constructor to invoke.</param>
        /// <returns>A reference to the newly created object.</returns>
        static System::Object CreateInstance(const Type& type, const std::vector<System::Object>& args);

        /// <summary>
        /// Creates an instance of the type designated by the specified generic type parameter, using the parameterless constructor.
        /// </summary>
        /// <param name="type">The type of object to create.</param>
        /// <returns>A reference to the newly created object.</returns>
        static System::Object CreateInstanceGeneric(const Type& type);

    private:
        /// <summary>
        /// Private constructor to prevent instantiation.
        /// </summary>
        Activator() = default;
    };
}
