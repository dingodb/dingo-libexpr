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

#include "utils.h"

namespace dingodb::expr {

int HexToNibble(const char hex) {
  if ('0' <= hex && hex <= '9') {
    return hex - '0';
  } else if ('a' <= hex && hex <= 'f') {
    return hex - 'a' + 10;
  } else if ('A' <= hex && hex <= 'F') {
    return hex - 'A' + 10;
  }
  return -1;
}

char NibbleToHex(int nibble) {
  if (0 <= nibble && nibble <= 9) {
    return nibble + '0';
  } else if (10 <= nibble && nibble <= 15) {
    return nibble - 10 + 'A';
  }
  return (char)-1;
}

void HexToBytes(Byte *buf, const char *hex, size_t len) {
  for (size_t i = 0; i < len / 2; ++i) {
    buf[i] = (Byte)(HexToNibble(hex[i + i]) << 4) | HexToNibble(hex[i + i + 1]);
  }
}

void BytesToHex(char *hex, const Byte *buf, size_t len) {
  char *p = hex;
  for (size_t i = 0; i < len; ++i) {
    *p = NibbleToHex(buf[i] >> 4);
    ++p;
    *p = NibbleToHex(buf[i] & 0x0F);
    ++p;
  }
}

std::string HexOfBytes(const Byte *data, size_t len) {
  char *buf = new char[len * 2 + 1];
  BytesToHex(buf, data, len);
  buf[len * 2] = '\0';
  std::string result(buf);
  delete[] buf;
  return result;
}

Tuple *ConcatTuple(const Tuple &t1, const Tuple &t2) {
  auto *tuple = new Tuple(t1.size() + t2.size());
  auto i = std::copy(t1.cbegin(), t1.cend(), tuple->begin());
  std::copy(t2.cbegin(), t2.cend(), i);
  return tuple;
}

Tuple *MapTuple(const Tuple &src, const int *index, size_t index_size) {
  auto *tuple = new Tuple(index_size);
  for (int i = 0; i < index_size; ++i) {
    (*tuple)[i] = src[index[i]];
  }
  return tuple;
}

}  // namespace dingodb::expr
