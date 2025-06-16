
#pragma once

#include "System/Delegate.h"
#ifdef CORELIB_REFLECTION_AVAILABLE
#include <rttr/registration>
#include <rttr/type>
#endif

namespace System
{
    namespace Reflection
    {
#ifdef CORELIB_REFLECTION_AVAILABLE
        /// <summary>
        /// Provides reflection support for delegates, allowing dynamic method binding.
        /// </summary>
        class DelegateReflection
        {
        public:
            /// <summary>
            /// Creates a delegate from a reflected method.
            /// </summary>
            /// <typeparam name="TSignature">The delegate signature.</typeparam>
            /// <param name="methodName">The name of the method to bind.</param>
            /// <param name="instance">Optional instance for member methods.</param>
            /// <returns>A delegate bound to the specified method.</returns>
            template<typename TSignature>
            static TypedDelegate<TSignature> CreateFromMethod(const String& methodName, Object* instance = nullptr);

            /// <summary>
            /// Creates a delegate from a reflected type and method.
            /// </summary>
            /// <typeparam name="TSignature">The delegate signature.</typeparam>
            /// <param name="typeName">The name of the type containing the method.</param>
            /// <param name="methodName">The name of the method to bind.</param>
            /// <param name="instance">Optional instance for member methods.</param>
            /// <returns>A delegate bound to the specified method.</returns>
            template<typename TSignature>
            static TypedDelegate<TSignature> CreateFromTypeMethod(const String& typeName, const String& methodName, Object* instance = nullptr);

            /// <summary>
            /// Registers delegate types with the RTTR reflection system.
            /// </summary>
            static void RegisterDelegateTypes();

        private:
            /// <summary>
            /// Helper to invoke a reflected method with delegate signature.
            /// </summary>
            template<typename TReturn, typename... TArgs>
            static TReturn InvokeReflectedMethod(const rttr::method& method, Object* instance, TArgs... args);
        };

        // Template implementations
        template<typename TReturn, typename... TArgs>
        TypedDelegate<TReturn(TArgs...)> DelegateReflection::CreateFromMethod(const String& methodName, Object* instance)
        {
            rttr::method method = rttr::type::get_global_method(methodName.c_str());
            if (!method.is_valid())
            {
                throw InvalidOperationException(String("Method not found: ") + methodName);
            }

            return TypedDelegate<TReturn(TArgs...)>::Create([method, instance](TArgs... args) -> TReturn {
                return InvokeReflectedMethod<TReturn, TArgs...>(method, instance, args...);
            });
        }

        template<typename TReturn, typename... TArgs>
        TypedDelegate<TReturn(TArgs...)> DelegateReflection::CreateFromTypeMethod(const String& typeName, const String& methodName, Object* instance)
        {
            rttr::type type = rttr::type::get_by_name(typeName.c_str());
            if (!type.is_valid())
            {
                throw InvalidOperationException(String("Type not found: ") + typeName);
            }

            rttr::method method = type.get_method(methodName.c_str());
            if (!method.is_valid())
            {
                throw InvalidOperationException(String("Method not found: ") + methodName + String(" in type: ") + typeName);
            }

            return TypedDelegate<TReturn(TArgs...)>::Create([method, instance](TArgs... args) -> TReturn {
                return InvokeReflectedMethod<TReturn, TArgs...>(method, instance, args...);
            });
        }

        template<typename TReturn, typename... TArgs>
        TReturn DelegateReflection::InvokeReflectedMethod(const rttr::method& method, Object* instance, TArgs... args)
        {
            std::vector<rttr::argument> arguments;
            (arguments.emplace_back(args), ...);

            rttr::variant result;
            if (instance)
            {
                result = method.invoke(*instance, arguments);
            }
            else
            {
                result = method.invoke({}, arguments);
            }

            if (!result.is_valid())
            {
                throw InvalidOperationException("Method invocation failed");
            }

            if constexpr (std::is_void_v<TReturn>)
            {
                return;
            }
            else
            {
                if (result.can_convert<TReturn>())
                {
                    return result.get_value<TReturn>();
                }
                else
                {
                    throw InvalidOperationException("Return type conversion failed");
                }
            }
        }
#endif // CORELIB_REFLECTION_AVAILABLE
    }
}
