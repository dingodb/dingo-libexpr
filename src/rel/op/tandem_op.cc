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

#include "tandem_op.h"

namespace dingodb::rel::op {

TandemOp::TandemOp(const RelOp *in, const RelOp *out) : m_in(in), m_out(out) {
}

TandemOp::~TandemOp() {
  delete m_in;
  delete m_out;
}

expr::Tuple *TandemOp::Put(expr::Tuple *tuple) const {
  auto *t = m_in->Put(tuple);
  if (t != nullptr) {
    return m_out->Put(t);
  }
  return nullptr;
}

}  // namespace dingodb::rel::op
