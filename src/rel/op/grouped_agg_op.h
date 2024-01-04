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

#ifndef _REL_OP_GROUPED_AGG_OP_H_
#define _REL_OP_GROUPED_AGG_OP_H_

#include <unordered_map>

#include "agg.h"
#include "agg_op.h"

namespace dingodb::rel::op {

class GroupedAggOp : public AggOp {
 public:
  GroupedAggOp(const int *group_indices, size_t group_indices_size, const std::vector<const Agg *> *aggs);

  ~GroupedAggOp() override;

  const expr::Tuple *Put(const expr::Tuple *tuple) const override;

  const expr::Tuple *Get() const override;

 private:
  const int *m_group_indices;
  size_t m_groupe_indices_size;

  mutable std::unordered_map<expr::Tuple, expr::Tuple *, std::hash<expr::Tuple>> m_caches;
};

}  // namespace dingodb::rel::op

#endif /* _REL_OP_GROUPED_AGG_OP_H_ */
