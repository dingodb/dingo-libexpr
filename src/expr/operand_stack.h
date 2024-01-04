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

#ifndef _OPERAND_STACK_H_
#define _OPERAND_STACK_H_

#include <deque>
#include <stdexcept>

#include "operand.h"

namespace dingodb::expr {

class OperandStack {
 public:
  OperandStack() : m_tuple(nullptr) {
  }

  virtual ~OperandStack() = default;

  void Pop() {
    m_stack.pop_back();
  }

  Operand Get() const {
    return m_stack.back();
  }

  void Push(const Operand &v) {
    m_stack.push_back(v);
  }

  template <typename T>
  void Push(T v) {
    m_stack.push_back(v);
  }

  template <typename T>
  void Push() {
    m_stack.push_back(nullptr);
  }

  void BindTuple(const Tuple *tuple) {
    m_tuple = tuple;
  }

  void PushVar(int32_t index) {
    if (m_tuple != nullptr) {
      m_stack.push_back((*m_tuple)[index]);
    } else {
      throw std::runtime_error("No tuple provided.");
    }
  }

  void Clear() {
    m_stack.clear();
  }

  size_t Size() const {
    return m_stack.size();
  }

  auto begin() const  // NOLINT(readability-identifier-naming)
  {
    return m_stack.cbegin();
  }

  auto end() const  // NOLINT(readability-identifier-naming)
  {
    return m_stack.cend();
  }

 private:
  std::deque<Operand> m_stack;
  const Tuple *m_tuple;
};

}  // namespace dingodb::expr

#endif /* _OPERAND_STACK_H_ */
