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

#ifndef _EXPR_EXPR_STRING_H_
#define _EXPR_EXPR_STRING_H_

#include <memory>
#include <ostream>
#include <string>

namespace dingodb::expr {

/**
 * @brief Type to hold a string.
 *
 */
class String {
 public:
  using ValueType = std::shared_ptr<std::string>;

  String(const std::shared_ptr<std::string> &ptr) : m_ptr(ptr) {
  }

  String(const std::string &str) : m_ptr(std::make_shared<std::string>(str)) {
  }

  String(const char *str) : m_ptr(std::make_shared<std::string>(str)) {
  }

  String(const char *str, size_t len) : m_ptr(std::make_shared<std::string>(str, len)) {
  }

  String() : m_ptr(std::make_shared<std::string>()) {
  }

  ValueType GetPtr() const {
    return m_ptr;
  }

  const std::string &operator*() const {
    return *m_ptr;
  }

  const std::string *operator->() const {
    return &*m_ptr;
  }

  String operator+(const String &v) const {
    return *m_ptr + *v.m_ptr;
  }

  bool operator==(const String &v) const {
    return *m_ptr == *v.m_ptr;
  }

  bool operator!=(const String &v) const {
    return *m_ptr != *v.m_ptr;
  }

  bool operator<(const String &v) const {
    return *m_ptr < *v.m_ptr;
  }

  bool operator<=(const String &v) const {
    return *m_ptr <= *v.m_ptr;
  }

  bool operator>(const String &v) const {
    return *m_ptr > *v.m_ptr;
  }

  bool operator>=(const String &v) const {
    return *m_ptr >= *v.m_ptr;
  }

 private:
  ValueType m_ptr;

  friend class Operand;

  friend std::ostream &operator<<(std::ostream &os, const String &v);
};

}  // namespace dingodb::expr

namespace std {

template <>
struct hash<::dingodb::expr::String> {
  size_t operator()(const ::dingodb::expr::String &val) const noexcept {
    return hash<std::string>()(*val);
  }
};

}  // namespace std

#endif /* _EXPR_EXPR_STRING_H_ */
