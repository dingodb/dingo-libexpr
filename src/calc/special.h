// Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _CALC_SPECIAL_H_
#define _CALC_SPECIAL_H_

#include <cstdint>

#include "../operand.h"
#include "../types.h"

namespace dingodb::expr::calc
{

template <typename T> bool IsNull(const wrap<T> &v)
{
    return !v.has_value();
}

template <typename T> bool IsTrue(const wrap<T> &v)
{
    return v.has_value() && *v;
}

template <> bool IsTrue(const wrap<String> &v);

template <typename T> bool IsFalse(const wrap<T> &v)
{
    return v.has_value() && !*v;
}

template <> bool IsFalse(const wrap<String> &v);

} // namespace dingodb::expr::calc

#endif /* _CALC_SPECIAL_H_ */
