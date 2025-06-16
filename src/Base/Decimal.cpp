
// Implementation file for System.Decimal
// Contains static constant definitions

#include "System/Decimal.h"

namespace System {

// Static constant definitions
const Decimal Decimal::MaxValue = Decimal("79228162514264337593543950335");
const Decimal Decimal::MinValue = Decimal("-79228162514264337593543950335");
const Decimal Decimal::Zero = Decimal(0);
const Decimal Decimal::One = Decimal(1);
const Decimal Decimal::MinusOne = Decimal(-1);

} // namespace System
