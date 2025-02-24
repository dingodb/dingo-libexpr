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

#ifndef DINGO_LIBEXPR_DECIMAL_P_H
#define DINGO_LIBEXPR_DECIMAL_P_H

#include <memory>
#include "decimal.h"

namespace dingodb {
namespace types {

class DecimalP {
 public:
  using ValueType = std::shared_ptr<Decimal>;

  DecimalP(const std::shared_ptr<Decimal> &ptr) : m_ptr(ptr) {
  }

  DecimalP(const Decimal &dec) : m_ptr(std::make_shared<Decimal>(dec)) {
  }

  DecimalP(const long var) : m_ptr(std::make_shared<Decimal>(var)){
  }

  DecimalP(const double var) : m_ptr(std::make_shared<Decimal>(var)) {
  }

  DecimalP(const std::string& str) : m_ptr(std::make_shared<Decimal>(str)) {
  }

  DecimalP() : m_ptr(std::make_shared<Decimal>()) {
  }

  ValueType GetPtr() const {
    return m_ptr;
  }

  const Decimal &operator*() const {
    return *m_ptr;
  }

  const Decimal *operator->() const {
    return &*m_ptr;
  }

  int32_t toInt() const {
    return m_ptr->toInt();
  }

  int64_t toLong() const {
    return m_ptr->toLong();
  }

  double toDouble() const {
    return m_ptr->toDouble();
  }

  const std::string ToString() const {
    return std::move(m_ptr->toString());
  }

  DecimalP operator+(const DecimalP &v) const {
    return *m_ptr + *v.m_ptr;
  }

  DecimalP operator-(const DecimalP &v) const {
    return *m_ptr - *v.m_ptr;
  }

  DecimalP operator*(const DecimalP &v) const {
    return *m_ptr * *v.m_ptr;
  }

  DecimalP operator/(const DecimalP &v) const {
    return *m_ptr / *v.m_ptr;
  }

  DecimalP operator-() const {
    return -(*m_ptr);
  }

  bool operator==(const DecimalP &v) const {
    return *m_ptr == *v.m_ptr;
  }

  bool operator!=(const DecimalP &v) const {
    return *m_ptr != *v.m_ptr;
  }

  bool operator<(const DecimalP &v) const {
    return *m_ptr < *v.m_ptr;
  }

  bool operator<=(const DecimalP &v) const {
    return *m_ptr <= *v.m_ptr;
  }

  bool operator>(const DecimalP &v) const {
    return *m_ptr > *v.m_ptr;
  }

  bool operator>=(const DecimalP &v) const {
    return *m_ptr >= *v.m_ptr;
  }

  DecimalP Abs() {
    return DecimalP((*m_ptr).Abs());
  }

 private:
  ValueType m_ptr;

  friend class Operand;

  friend std::ostream &operator<<(std::ostream &os, const DecimalP &v);
};

}  // namespace types
}  // namespace dingodb

//using namespace dingodb::expr;
namespace std {

    template <>
    struct hash<::dingodb::types::DecimalP> {
        size_t operator()(const ::dingodb::types::DecimalP &val) const noexcept {
            return hash<std::string>()(val->toString());
        }
    };
}  // namespace std
#endif  // DINGO_LIBEXPR_DECIMAL_P_H
