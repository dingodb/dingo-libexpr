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

#ifndef _REL_REL_RUNNER_H_
#define _REL_REL_RUNNER_H_

#include "../expr/codec.h"
#include "../expr/types.h"
#include "op/agg.h"
#include "op/rel_op.h"

namespace dingodb::rel {

class RelRunner {
 public:
  RelRunner();
  virtual ~RelRunner();

  const expr::Byte *Decode(const expr::Byte *code, size_t len);

  const expr::Tuple *Put(const expr::Tuple *tuple) const;

  const expr::Tuple *Get() const;

 private:
  RelOp *m_op;

  void Release() {
    delete m_op;
    m_op = nullptr;
  }

  void AppendOp(RelOp *op);
};

}  // namespace dingodb::rel

namespace dingodb::expr {

template <class AGG>
const Byte *DecodeAgg(const rel::op::Agg *&agg, const Byte *data) {
  const Byte *p = data;
  ++p;
  int32_t index;
  p = DecodeValue(index, p);
  agg = new AGG(index);
  return p;
}

template <>
const Byte *DecodeAgg<rel::op::CountAllAgg>(const rel::op::Agg *&agg, const Byte *data);

template <>
const Byte *DecodeValue<const rel::op::Agg *>(const rel::op::Agg *&value, const Byte *data);

}  // namespace dingodb::expr

#endif /* _REL_REL_RUNNER_H_ */
