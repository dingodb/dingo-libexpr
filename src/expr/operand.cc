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

#include "operand.h"

namespace dingodb::expr {

std::ostream &operator<<(std::ostream &os, const Operand &v) {
  if (std::holds_alternative<int32_t>(v.m_data)) {
    os << std::get<int32_t>(v.m_data);
  } else if (std::holds_alternative<int64_t>(v.m_data)) {
    os << std::get<int64_t>(v.m_data);
  } else if (std::holds_alternative<bool>(v.m_data)) {
    os << std::get<bool>(v.m_data);
  } else if (std::holds_alternative<float>(v.m_data)) {
    os << std::get<float>(v.m_data);
  } else if (std::holds_alternative<double>(v.m_data)) {
    os << std::get<double>(v.m_data);
  } else if (std::holds_alternative<String>(v.m_data)) {
    os << std::get<String>(v.m_data);
  } else if (std::holds_alternative<std::monostate>(v.m_data)) {
    os << "(null)";
  } else {
    os << "<Unknown type>";
  }
  return os;
}
namespace any_optional_data_adaptor {

template <>
std::any FromOperand<String::ValueType>(const Operand &v) {
  std::optional<String::ValueType> opt;
  if  (v != nullptr) {
    opt = std::optional<String::ValueType>(v.GetValue<String>().GetPtr());
  } else {
    opt = std::optional<String::ValueType>();
  }
  return std::make_any<std::optional<String::ValueType>>(opt);
}

}  // namespace any_optional_data_adaptor

}  // namespace dingodb::expr
