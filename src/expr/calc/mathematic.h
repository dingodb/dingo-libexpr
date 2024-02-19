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

#ifndef _EXPR_CALC_MATHEMATIC_H_
#define _EXPR_CALC_MATHEMATIC_H_

#include "../types.h"

namespace dingodb::expr::calc {

template <typename T>
T Min(T v0, T v1) {
  return std::min(v0, v1);
}

template <>
String Min(String v0, String v1);

template <typename T>
T Max(T v0, T v1) {
  return std::max(v0, v1);
}

template <>
String Max(String v0, String v1);

template <typename T>
T Abs(T v);

template <>
int32_t Abs(int32_t v);

template <>
int64_t Abs(int64_t v);

template <>
float Abs(float v);

template <>
double Abs(double v);

template <typename T>
T AbsCheck(T v) {
  return Abs(v);
}

template <>
int32_t AbsCheck(int32_t v);

template <>
int64_t AbsCheck(int64_t v);

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_MATHEMATIC_H_ */
