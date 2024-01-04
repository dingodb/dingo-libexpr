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

namespace dingodb::expr {

int HexToNibble(char hex);

char NibbleToHex(int nibble);

void HexToBytes(Byte *buf, const char *hex, size_t len);

void BytesToHex(char *hex, const Byte *buf, size_t len);

std::string HexOfBytes(const Byte *data, size_t len);

Tuple *ConcatTuple(const Tuple &t1, const Tuple &t2);

Tuple *MapTuple(const Tuple &src, const int *index, size_t index_size);

}  // namespace dingodb::expr

#endif /* _EXPR_UTILS_H_ */
