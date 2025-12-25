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

#include <cstdint>

#include "arithmetic.h"
#include "../exception.h"

typedef __int128 int128;
#define INT64CONST(x)  (x##L)
#define EXPR_INT64_MIN	(-INT64CONST(0x7FFFFFFFFFFFFFFF) - 1)
#define EXPR_INT64_MAX	INT64CONST(0x7FFFFFFFFFFFFFFF)

namespace dingodb::expr::calc {
template <>
Operand Div<int>(int v0, int v1) {
  if (v1 != 0) {
    return (double)v0 / (double)v1;
  }
  return nullptr;
}

template <>
Operand Div<long>(long v0, long v1) {
  if (v1 != 0) {
    return (double)v0 / (double)v1;
  }
  return nullptr;
}

template <>
Operand Div<::dingodb::types::DecimalP>(
    ::dingodb::types::DecimalP v0,
    ::dingodb::types::DecimalP v1) {
  if (v1 != DecimalP(std::string("0"))) {
    return v0 / v1;
  }
  return nullptr;
}

template <>
long Add<long>(long v0, long v1) {
  int128 result = (int128)v0 + (int128)v1;

  if (result > EXPR_INT64_MAX || result < EXPR_INT64_MIN) {
    throw ExceedsLimits<TYPE_INT64>();
  }

  return (long)result;
}

template <>
long Sub<long>(long v0, long v1) {
  int128 result = (int128)v0 - (int128)v1;

  if (result > EXPR_INT64_MAX || result < EXPR_INT64_MIN) {
    throw ExceedsLimits<TYPE_INT64>();
  }

  return (long)result;
}

template <>
long Mul<long>(long v0, long v1) {
  int128 result = (int128)v0 * (int128)v1;

  if (result > EXPR_INT64_MAX || result < EXPR_INT64_MIN) {
    throw ExceedsLimits<TYPE_INT64>();
  }

  return (long)result;
}

}  // namespace dingodb::expr::calc