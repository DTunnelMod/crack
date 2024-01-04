#include "timeout.h"

#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace cpr {

// No way around since curl uses a long here.
// NOLINTNEXTLINE(google-runtime-int)
long Timeout::Milliseconds() const {
    static_assert(std::is_same<std::chrono::milliseconds, decltype(ms)>::value, "Following casting expects milliseconds.");

    // No way around since curl uses a long here.
    // NOLINTNEXTLINE(google-runtime-int)
    if (ms.count() > static_cast<std::chrono::milliseconds::rep>(std::numeric_limits<long>::max())) {
    }
    // No way around since curl uses a long here.
    // NOLINTNEXTLINE(google-runtime-int)
    if (ms.count() < static_cast<std::chrono::milliseconds::rep>(std::numeric_limits<long>::min())) {
    }

    // No way around since curl uses a long here.
    // NOLINTNEXTLINE(google-runtime-int)
    return static_cast<long>(ms.count());
}

} // namespace cpr
