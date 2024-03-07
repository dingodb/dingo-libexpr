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

#include "../exception.h"
#include "casting.h"

TEST(TestToString, std) {
  ASSERT_EQ(std::to_string(1), "1");
  ASSERT_EQ(std::to_string(1LL), "1");
  ASSERT_EQ(std::to_string(1.0f), "1.000000");
  ASSERT_EQ(std::to_string(1.0), "1.000000");
}

using namespace dingodb::expr;

TEST(TestToString, Cast) {
  ASSERT_EQ(*((calc::Cast<String>(1.0f))), "1.0");
  ASSERT_EQ(*((calc::Cast<String>(2.0))), "2.0");
  ASSERT_EQ(*((calc::Cast<String>(2.30f))), "2.299999952316284");
  ASSERT_EQ(*((calc::Cast<String>(2.30))), "2.3");
  ASSERT_EQ(*((calc::Cast<String>(2.299999952316284))), "2.299999952316284");
}

TEST(TestToInt32, Cast) {
  ASSERT_THROW(calc::CastCheck<int32_t>((int64_t)std::numeric_limits<int32_t>::max() + 1), ExceedsLimits<TYPE_INT32>);
  ASSERT_THROW(calc::CastCheck<int32_t>((int64_t)std::numeric_limits<int32_t>::min() - 1), ExceedsLimits<TYPE_INT32>);
  ASSERT_THROW(calc::CastCheck<int32_t>((float)std::numeric_limits<int32_t>::max() + 1000.0f), ExceedsLimits<TYPE_INT32>);
  ASSERT_THROW(calc::CastCheck<int32_t>((float)std::numeric_limits<int32_t>::min() - 1000.0f), ExceedsLimits<TYPE_INT32>);
  ASSERT_THROW(calc::CastCheck<int64_t>((double)std::numeric_limits<int64_t>::max() + 10000.0), ExceedsLimits<TYPE_INT64>);
  ASSERT_THROW(calc::CastCheck<int64_t>((double)std::numeric_limits<int64_t>::min() - 10000.0), ExceedsLimits<TYPE_INT64>);
}
