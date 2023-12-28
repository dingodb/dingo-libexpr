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

#ifndef _EXPR_OPERAND_H_
#define _EXPR_OPERAND_H_

#include <any>
#include <optional>
#include <vector>

namespace dingodb::expr {

namespace operand_is_any_optional {

using Operand = std::any;

template <typename T>
inline Operand MakeOperand(T v) {
  return std::make_any<std::optional<T>>(std::optional<T>(v));
}

template <typename T>
inline Operand MakeNull() {
  return std::make_any<std::optional<T>>(std::optional<T>());
}

template <typename T>
inline bool NotNull(const Operand &v) {
  const std::optional<T> vv = std::any_cast<const std::optional<T>>(v);
  return vv.has_value();
}

template <typename T>
inline T GetValue(const Operand &v) {
  const std::optional<T> vv = std::any_cast<const std::optional<T>>(v);
  return *vv;
}

}  // namespace operand_is_any_optional

namespace operand_is_any {

using Operand = std::any;

template <typename T>
inline Operand MakeOperand(T v) {
  return std::make_any<T>(v);
}

template <typename T>
inline Operand MakeNull() {
  return std::any();
}

template <typename T>
inline bool NotNull(const Operand &v) {
  return v.has_value();
}

template <typename T>
inline T GetValue(const Operand &v) {
  return std::any_cast<T>(v);
}

}  // namespace operand_is_any

using namespace operand_is_any;

using Tuple = std::vector<Operand>;

}  // namespace dingodb::expr

#endif /* _EXPR_OPERAND_H_ */
