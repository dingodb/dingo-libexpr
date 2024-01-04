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

#ifndef _EXPR_CALC_SPECIAL_H_
#define _EXPR_CALC_SPECIAL_H_

#include "../operand.h"

namespace dingodb::expr::calc {

template <typename T>
bool IsNull(const Operand &v) {
  return v == nullptr;
}

template <typename T>
bool IsTrue(const Operand &v) {
  return v != nullptr && v.GetValue<T>();
}

template <>
bool IsTrue<String>(const Operand &v);

template <typename T>
bool IsFalse(const Operand &v) {
  return v != nullptr && !v.GetValue<T>();
}

template <>
bool IsFalse<String>(const Operand &v);

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_SPECIAL_H_ */
