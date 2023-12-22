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

#ifndef _EXPR_CODEC_H_
#define _EXPR_CODEC_H_

#include <cstddef>

#include "types.h"

namespace dingodb::expr {

// T can be int32_t or int64_t
template <typename T>
const Byte *DecodeVarint(T &value, const Byte *data) {
  value = 0;
  const Byte *p;
  int shift = 0;
  for (p = data; ((*p) & 0x80) != 0; ++p) {
    value |= ((T)(*p & 0x7F) << shift);
    shift += 7;
  }
  value |= ((T)(*p) << shift);
  return p + 1;
}

const Byte *DecodeFloat(float &value, const Byte *data);

const Byte *DecodeDouble(double &value, const Byte *data);

const Byte *DecodeString(String &value, const Byte *data);

int HexToNibble(char hex);

char NibbleToHex(int nibble);

void HexToBytes(Byte *buf, const char *hex, size_t len);

void BytesToHex(char *hex, const Byte *buf, size_t len);

}  // namespace dingodb::expr

#endif /* _EXPR_CODEC_H_ */
