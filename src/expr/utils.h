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

#ifndef _EXPR_UTILS_H_
#define _EXPR_UTILS_H_

#include "operand.h"
#include "types.h"

namespace dingodb::expr {

template <typename... T>
expr::Tuple *MakeTuple(T... v) {
  return new expr::Tuple{std::make_any<expr::Wrap<T>>(v)...};
}

std::string HexOfBytes(const Byte *data, size_t len);

}  // namespace dingodb::expr

#endif /* _EXPR_UTILS_H_ */
