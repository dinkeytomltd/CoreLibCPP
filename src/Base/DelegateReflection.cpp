
#include "System/DelegateReflection.h"
#ifdef CORELIB_REFLECTION_AVAILABLE
#include <rttr/registration>

using namespace rttr;
#endif

namespace System
{
    namespace Reflection
    {
#ifdef CORELIB_REFLECTION_AVAILABLE
        void DelegateReflection::RegisterDelegateTypes()
        {
            // Register base Delegate class
            registration::class_<Delegate>("System.Delegate")
                .method("IsNull", &Delegate::IsNull)
                .method("GetMethodInfo", &Delegate::GetMethodInfo)
                .method("ToString", &Delegate::ToString);

            // Register common delegate types
            registration::class_<Action>("System.Action")
                .constructor<>()
                .method("Invoke", &Action::Invoke)
                .method("IsNull", &Action::IsNull);

            registration::class_<Action1<int>>("System.Action1<int>")
                .constructor<>()
                .method("Invoke", &Action1<int>::Invoke)
                .method("IsNull", &Action1<int>::IsNull);

            registration::class_<Action1<String>>("System.Action1<String>")
                .constructor<>()
                .method("Invoke", &Action1<String>::Invoke)
                .method("IsNull", &Action1<String>::IsNull);

            registration::class_<Func<int>>("System.Func<int>")
                .constructor<>()
                .method("Invoke", &Func<int>::Invoke)
                .method("IsNull", &Func<int>::IsNull);

            registration::class_<Func1<int, String>>("System.Func1<int,String>")
                .constructor<>()
                .method("Invoke", &Func1<int, String>::Invoke)
                .method("IsNull", &Func1<int, String>::IsNull);
        }
#endif // CORELIB_REFLECTION_AVAILABLE
    }
}

#ifdef CORELIB_REFLECTION_AVAILABLE
// Register delegate types with RTTR
RTTR_REGISTRATION
{
    System::Reflection::DelegateReflection::RegisterDelegateTypes();
}
#endif
