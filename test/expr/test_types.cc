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

#include <gtest/gtest.h>

#include <functional>

#include "expr/types.h"

using namespace dingodb::expr;

TEST(TestTypes, StringEquals) {
  String s0{"Alice"};
  String s1{"Alice"};
  ASSERT_EQ(std::hash<std::string>()(*s0), std::hash<std::string>()(*s1));
  ASSERT_TRUE(std::equal_to()(*s0, *s1));
  ASSERT_EQ(*s0, *s1);
  ASSERT_EQ(std::hash<String>()(s0), std::hash<String>()(s1));
  ASSERT_TRUE(std::equal_to()(s0, s1));
  ASSERT_EQ(s0, s1);
}
