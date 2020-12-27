#pragma once

#include "templex_common_export.h"

namespace nostd {

template <typename T>
struct TEMPLEX_COMMON_EXPORT less_pointee {
    constexpr bool operator()(const std::shared_ptr<T>& lhs,
                              const std::shared_ptr<T>& rhs) const
    {
        return *lhs < *rhs;
    }
};

} // namespace nostd
