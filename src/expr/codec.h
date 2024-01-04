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
#include <vector>

#include "exception.h"
#include "types.h"

namespace dingodb::expr {

/**
 * @brief Decode a value from code buffer.
 *
 * @tparam T type of the value
 * @param value reference to the value
 * @param data code buffer
 * @return const Byte* point to the next byte of the bytes used
 */
template <typename T>
const Byte *DecodeValue(T &value, const Byte *data);

template <>
const Byte *DecodeValue(int &value, const Byte *data);

template <>
const Byte *DecodeValue(long &value, const Byte *data);

template <>
const Byte *DecodeValue(long long &value, const Byte *data);

template <>
const Byte *DecodeValue(unsigned int &value, const Byte *data);

template <>
const Byte *DecodeValue(unsigned long &value, const Byte *data);

template <>
const Byte *DecodeValue(unsigned long long &value, const Byte *data);

template <>
const Byte *DecodeValue(float &value, const Byte *data);

template <>
const Byte *DecodeValue(double &value, const Byte *data);

template <>
const Byte *DecodeValue(String &value, const Byte *data);

template <typename T>
const Byte *DecodeElements(T &container, size_t count, const Byte *code, size_t len) {
  const Byte *p = code;
  int i;
  for (i = 0; i < count && p < code + len; ++i) {
    p = DecodeValue(container[i], p);
  }
  if (i < count) {
    throw MoreElementsRequired(count, i);
  }
  return p;
}

template <typename T>
const Byte *DecodeArray(T *&arr, size_t &count, const Byte *code, size_t len) {
  const Byte *p = code;
  p = DecodeValue(count, p);
  arr = new T[count];
  return DecodeElements(arr, count, p, code + len - p);
}

template <typename T>
const Byte *DecodeVector(std::vector<T> *&vec, const Byte *code, size_t len) {
  const Byte *p = code;
  size_t count;
  p = DecodeValue(count, p);
  vec = new std::vector<T>(count);
  return DecodeElements(*vec, count, p, code + len - p);
}

}  // namespace dingodb::expr

#endif /* _EXPR_CODEC_H_ */
