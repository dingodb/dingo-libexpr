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

#include "runner.h"

#include <algorithm>

namespace dingodb::expr {

void Runner::Run() const {
  m_operand_stack.Clear();
  for (const auto *op : m_operator_vector) {
    (*op)(m_operand_stack);
  }
}

Tuple *Runner::GetAll() const {
  auto *tuple = new Tuple();
  std::copy(m_operand_stack.begin(), m_operand_stack.end(), std::back_inserter(*tuple));
  return tuple;
}

}  // namespace dingodb::expr
