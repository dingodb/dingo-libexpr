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

#include "agg.h"

namespace dingodb::rel::op {

expr::Operand CountAllAgg::Add(const expr::Operand &var, [[maybe_unused]] const expr::Tuple *tuple) const {
  if (expr::NotNull<int64_t>(var)) {
    return expr::MakeOperand(expr::GetValue<int64_t>(var) + 1LL);
  }
  return expr::MakeOperand(1LL);
}

}  // namespace dingodb::rel::op
