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

#include "casting.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "../exception.h"

namespace dingodb::expr::calc {

template <>
int32_t Cast(float v) {
  return lround(v);
}

template <>
int32_t Cast(double v) {
  return lround(v);
}

template <>
int32_t Cast(String v) {
  return std::stoi(*v);
}

template <>
int64_t Cast(float v) {
  return llround(v);
}

template <>
int64_t Cast(double v) {
  return llround(v);
}

template <>
int64_t Cast(String v) {
  return std::stoll(*v);
}

template <>
float Cast(String v) {
  return std::stof(*v);
}

template <>
double Cast(String v) {
  return std::stod(*v);
}

template <>
String Cast(int32_t v) {
  return std::to_string(v);
}

template <>
String Cast(int64_t v) {
  return std::to_string(v);
}

template <>
String Cast(bool v) {
  return v ? "true" : "false";
}

template <typename T>
static String CastF(T v) {
  // precision is 6 by default
  // auto s = std::to_string(v);
  std::stringstream ss;
  ss << std::fixed << std::setprecision(15) << v;
  auto s = ss.str();
  s.erase(s.find_last_not_of('0') + 1);
  if (s.back() == '.') {
    s += '0';
  }
  return s;
}

template <>
String Cast(float v) {
  return CastF(v);
}

template <>
String Cast(double v) {
  return CastF(v);
}

template <>
int32_t CastCheck(int64_t v) {
  auto r = Cast<int32_t>(v);
  if (r == v) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT32>();
}

static inline bool ErrorAcceptable(float a, float b) {
  return fabsf(a - b) <= 0.5f;
}

static inline bool ErrorAcceptable(double a, double b) {
  return fabs(a - b) <= 0.5;
}

template <>
int32_t CastCheck(float v) {
  auto r = Cast<int32_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT32>();
}

template <>
int32_t CastCheck(double v) {
  auto r = Cast<int32_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT32>();
}

template <>
int64_t CastCheck(float v) {
  auto r = Cast<int64_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT64>();
}

template <>
int64_t CastCheck(double v) {
  auto r = Cast<int64_t>(v);
  if (ErrorAcceptable(r, v)) {
    return r;
  }
  throw ExceedsLimits<TYPE_INT64>();
}

}  // namespace dingodb::expr::calc
