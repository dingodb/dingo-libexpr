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

#include "rel_runner.h"

#include "expr/runner.h"
#include "expr/utils.h"
#include "op/filter_op.h"
#include "op/project_op.h"
#include "op/tandem_op.h"

#define FILTER_OP  0x71
#define PROJECT_OP 0x72

namespace dingodb::rel {

using namespace expr;

RelRunner::RelRunner() : m_op(nullptr) {
}

RelRunner::~RelRunner() {
  Release();
}

const expr::Byte *RelRunner::Decode(const expr::Byte *code, size_t len) {
  Release();
  bool successful = true;
  const Byte *p = code;
  const Byte *b;
  while (successful && p < code + len) {
    b = p;
    switch (*p) {
    case FILTER_OP: {
      ++p;
      auto *filter = new expr::Runner();
      p = filter->Decode(p, code + len - p);
      AppendOp(new op::FilterOp(filter));
      break;
    }
    case PROJECT_OP: {
      ++p;
      auto *projects = new expr::Runner();
      p = projects->Decode(p, code + len - p);
      AppendOp(new op::ProjectOp(projects));
      break;
    }
    default:
      successful = false;
      break;
    }
  }
  if (successful) {
    return p;
  }
  throw std::runtime_error("Unknown instruction, bytes = " + expr::HexOfBytes(b, len - (b - code)));
}

expr::Tuple *RelRunner::Put(expr::Tuple *tuple) {
  return m_op->Put(tuple);
}

void RelRunner::AppendOp(RelOp *op) {
  if (m_op != nullptr) {
    m_op = new op::TandemOp(m_op, op);
  } else {
    m_op = op;
  }
}

}  // namespace dingodb::rel
