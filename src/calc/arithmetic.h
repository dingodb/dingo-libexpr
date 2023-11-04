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

#ifndef _CALC_ARITHMETIC_H_
#define _CALC_ARITHMETIC_H_

#include "../types.h"

namespace dingodb::expr::calc
{

template <typename T> T Pos(T v)
{
    return v;
}

template <typename T> T Neg(T v)
{
    return -v;
}

template <typename T> T Add(T v0, T v1)
{
    return v0 + v1;
}

template <> String Add(String v0, String v1);

template <typename T> T Sub(T v0, T v1)
{
    return v0 - v1;
}

template <typename T> T Mul(T v0, T v1)
{
    return v0 * v1;
}

template <typename T> T Div(T v0, T v1)
{
    return v0 / v1;
}

} // namespace dingodb::expr::calc

#endif /* _CALC_ARITHMETIC_H_ */
