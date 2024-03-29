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

#ifndef _REL_OP_TANDEM_OP_H_
#define _REL_OP_TANDEM_OP_H_

#include "rel_op.h"

namespace dingodb::rel::op {

class TandemOp : public RelOp {
 public:
  TandemOp(const RelOp *in, const RelOp *out);

  ~TandemOp() override;

  const expr::Tuple *Put(const expr::Tuple *tuple) const override;
  const expr::Tuple *Get() const override;

 private:
  const RelOp *m_in;
  const RelOp *m_out;
};

}  // namespace dingodb::rel::op

#endif /* _REL_OP_TANDEM_OP_H_ */
