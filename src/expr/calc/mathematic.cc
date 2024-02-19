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

#include "mathematic.h"

#include <cmath>

#include "../exception.h"

namespace dingodb::expr::calc {

template <>
String Min(String v0, String v1) {
  return std::min(*v0, *v1);
}

template <>
String Max(String v0, String v1) {
  return std::max(*v0, *v1);
}

template <>
int32_t Abs(int32_t v) {
  static_assert(std::is_same_v<int32_t, int>, "`int32_t` must be an alias of `int`.");
  return abs(v);
}

template <>
int64_t Abs(int64_t v) {
  static_assert(
      std::is_same_v<int64_t, long> || std::is_same_v<int64_t, long long>,
      "`int64_t` must be an alias of `long` or `long long`.");
  if (std::is_same_v<int64_t, long>) {
    return labs(v);
  }
  return llabs(v);
}

template <>
float Abs(float v) {
  return fabsf(v);
}

template <>
double Abs(double v) {
  return fabs(v);
}

template <>
int32_t AbsCheck(int32_t v) {
  if (v == std::numeric_limits<int32_t>::min()) {
    throw ExceedsLimits<TYPE_INT32>();
  }
  return abs(v);
}

template <>
int64_t AbsCheck(int64_t v) {
  if (v == std::numeric_limits<int64_t>::min()) {
    throw ExceedsLimits<TYPE_INT64>();
  }
  return llabs(v);
}

}  // namespace dingodb::expr::calc
