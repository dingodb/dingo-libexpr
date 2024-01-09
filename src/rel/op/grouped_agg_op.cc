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

#include "grouped_agg_op.h"

#include "../../expr/utils.h"

namespace dingodb::rel::op {

GroupedAggOp::GroupedAggOp(const int *group_indices, size_t group_indices_size, const std::vector<const Agg *> *aggs)
    : AggOp(aggs)
    , m_group_indices(group_indices)
    , m_groupe_indices_size(group_indices_size) {
}

GroupedAggOp::~GroupedAggOp() {
  delete[] m_group_indices;
  for (auto &e : m_caches) {
    delete e.second;
  }
}

const expr::Tuple *GroupedAggOp::Put(const expr::Tuple *tuple) const {
  auto *key = expr::MapTuple(*tuple, m_group_indices, m_groupe_indices_size);
  auto *&cache = m_caches[*key];
  delete key;
  AddToCache(cache, tuple);
  return nullptr;
}

const expr::Tuple *GroupedAggOp::Get() const {
  if (!m_caches.empty()) {
    auto i = m_caches.begin();
    auto *tuple = expr::ConcatTuple(i->first, *(i->second));
    delete i->second;
    m_caches.erase(i);
    return tuple;
  }
  return nullptr;
}

}  // namespace dingodb::rel::op
