
#pragma once

namespace System::Reflection {

class TypeRegistrar {
public:
    static void RegisterAllTypes();
    
private:
    static void RegisterPrimitiveTypes();
    static void RegisterReferenceTypes();
    static void RegisterCollectionTypes();
    static void RegisterDateTimeTypes();
    static void RegisterTextTypes();
    static void RegisterExceptionTypes();
    static void RegisterIOTypes();
    static void RegisterReflectionTypes();
};

} // namespace System::Reflection

