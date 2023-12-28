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

#include "mathematic.h"

#include <cmath>

namespace dingodb::expr::calc {

template <>
String Min(String v0, String v1) {
  return MakeString(std::min(*v0, *v1));
}

template <>
String Max(String v0, String v1) {
  return MakeString(std::max(*v0, *v1));
}

}  // namespace dingodb::expr::calc
