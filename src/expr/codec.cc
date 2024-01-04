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

#ifdef __APPLE__
#include <netinet/in.h>
#define be32toh(x) ntohl(x)
#define be64toh(x) ntohll(x)
#else
#include <endian.h>
#endif

#include "codec.h"

namespace dingodb::expr {

template <typename T>
static const Byte *DecodeVarint(T &value, const Byte *data) {
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

template <>
const Byte *DecodeValue(int &value, const Byte *data) {
  return DecodeVarint(value, data);
}

template <>
const Byte *DecodeValue(long &value, const Byte *data) {
  return DecodeVarint(value, data);
}

template <>
const Byte *DecodeValue(long long &value, const Byte *data) {
  return DecodeVarint(value, data);
}

template <>
const Byte *DecodeValue(unsigned int &value, const Byte *data) {
  return DecodeVarint(value, data);
}

template <>
const Byte *DecodeValue(unsigned long &value, const Byte *data) {
  return DecodeVarint(value, data);
}

template <>
const Byte *DecodeValue(unsigned long long &value, const Byte *data) {
  return DecodeVarint(value, data);
}

template <>
const Byte *DecodeValue(float &value, const Byte *data) {
  uint32_t l = be32toh(*(uint32_t *)data);
  value = *(float *)&l;
  return data + 4;
}

template <>
const Byte *DecodeValue(double &value, const Byte *data) {
  uint64_t l = be64toh(*(uint64_t *)data);
  value = *(double *)&l;
  return data + 8;
}

template <>
const Byte *DecodeValue(String &value, const Byte *data) {
  uint32_t len;
  const Byte *p = DecodeValue(len, data);
  value = String(reinterpret_cast<const char *>(p), len);
  return p + len;
}

}  // namespace dingodb::expr
