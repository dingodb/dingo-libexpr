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

#ifndef _REL_OP_AGG_OP_H_
#define _REL_OP_AGG_OP_H_

#include "agg.h"
#include "rel_op.h"

namespace dingodb::rel::op {

class AggOp : public RelOp {
 protected:
  AggOp(const std::vector<const Agg *> *aggs);

 public:
  ~AggOp() override;

 protected:
  const std::vector<const Agg *> *m_aggs;

  void AddToCache(expr::Tuple *&cache, const expr::Tuple *tuple) const;
};

}  // namespace dingodb::rel::op

#endif /* _REL_OP_AGG_OP_H_ */
