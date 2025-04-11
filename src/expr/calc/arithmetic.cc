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

#include <cstdint>
#include "arithmetic.h"

namespace dingodb::expr::calc {

template <>
Operand Div<int>(int v0, int v1) {
  if (v1 != 0) {
    return (double)v0 / (double)v1;
  }
  return nullptr;
}

template <>
Operand Div<long>(long v0, long v1) {
  if (v1 != 0) {
    return (double)v0 / (double)v1;
  }
  return nullptr;
}

}  // namespace dingodb::expr::calc


