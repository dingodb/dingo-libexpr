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

#include "ungrouped_agg_op.h"

namespace dingodb::rel::op {

UngroupedAggOp::UngroupedAggOp(const std::vector<const Agg *> *aggs) : AggOp(aggs), m_cache(nullptr) {
}

UngroupedAggOp::~UngroupedAggOp() {
  delete m_cache;
}

const expr::Tuple *UngroupedAggOp::Put(const expr::Tuple *tuple) const {
  AddToCache(m_cache, tuple);
  return nullptr;
}

const expr::Tuple *UngroupedAggOp::Get() const {
  if (m_cache != nullptr) {
    auto *p = m_cache;
    m_cache = nullptr;
    return p;
  }
  return nullptr;
}

}  // namespace dingodb::rel::op
