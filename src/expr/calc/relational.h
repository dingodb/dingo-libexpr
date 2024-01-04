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

#ifndef _EXPR_CALC_RELATIONAL_H_
#define _EXPR_CALC_RELATIONAL_H_

namespace dingodb::expr::calc {

template <typename T>
bool Eq(T v0, T v1) {
  return v0 == v1;
}

template <typename T>
bool Ge(T v0, T v1) {
  return v0 >= v1;
}

template <typename T>
bool Gt(T v0, T v1) {
  return v0 > v1;
}

template <typename T>
bool Le(T v0, T v1) {
  return v0 <= v1;
}

template <typename T>
bool Lt(T v0, T v1) {
  return v0 < v1;
}

template <typename T>
bool Ne(T v0, T v1) {
  return v0 != v1;
}

}  // namespace dingodb::expr::calc

#endif /* _EXPR_CALC_RELATIONAL_H_ */
