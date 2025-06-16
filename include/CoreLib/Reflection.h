
#pragma once

// CoreLib.Reflection - RTTR-based reflection system
// This header provides a convenient way to include all Reflection module functionality

#include "CoreLib/Base.h"
#include "CoreLib/Collections.h"

// Core reflection types
#include "System/Reflection/Type.h"
#include "System/Reflection/MemberInfo.h"
#include "System/Reflection/MethodInfo.h"
#include "System/Reflection/PropertyInfo.h"
#include "System/Reflection/FieldInfo.h"
#include "System/Reflection/ConstructorInfo.h"
#include "System/Reflection/ParameterInfo.h"
#include "System/Reflection/Assembly.h"

// Reflection utilities
#include "System/Reflection/Activator.h"
#include "System/Reflection/Attribute.h"
#include "System/Reflection/TypeRegistration.h"
#include "System/Reflection/TypeRegistrar.h"

// Reflection enumerations
#include "System/Reflection/MemberTypes.h"
#include "System/Reflection/BindingFlags.h"
#include "System/Reflection/CallingConventions.h"

// Reflection exceptions
#include "System/Reflection/ReflectionExceptions.h"
