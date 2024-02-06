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

#include <climits>
#include <memory>
#include <tuple>

#include "codec.h"
#include "runner.h"

using namespace dingodb::expr;

class ExprTest : public testing::TestWithParam<std::tuple<std::string, Tuple *, Operand>> {};

TEST_P(ExprTest, Run) {
  const auto &para = GetParam();
  Runner runner;
  auto input = std::get<0>(para);
  auto len = input.size() / 2;
  Byte buf[len];
  HexToBytes(buf, input.data(), input.size());
  runner.Decode(buf, len);
  runner.BindTuple(std::get<1>(para));
  runner.Run();
  auto result = runner.Get();
  EXPECT_EQ(result, std::get<2>(para));
}

// Test cases with consts
INSTANTIATE_TEST_SUITE_P(
    ConstExpr,
    ExprTest,
    testing::Values(
        std::make_tuple("1101", nullptr, 1),                                    // 1
        std::make_tuple("2101", nullptr, -1),                                   // -1
        std::make_tuple("119601", nullptr, 150),                                // 150
        std::make_tuple("219601", nullptr, -150),                               // -150
        std::make_tuple("13", nullptr, true),                                   // true
        std::make_tuple("23", nullptr, false),                                  // false
        std::make_tuple("15401F333333333333", nullptr, 7.8),                    // 7.8
        std::make_tuple("15400921FB4D12D84A", nullptr, 3.1415926),              // 3.1415926
        std::make_tuple("1541B1E1A300000000", nullptr, 3E8),                    // 3E8
        std::make_tuple("1703616263", nullptr, "abc"),                          // 'abc'
        std::make_tuple("110111018301", nullptr, 2),                            // 1 + 1
        std::make_tuple("110211038301", nullptr, 5),                            // 2 + 3
        std::make_tuple("120112018302", nullptr, 2LL),                          // 1L + 1L
        std::make_tuple("120212038302", nullptr, 5LL),                          // 2L + 3L
        std::make_tuple("11031104110685018301", nullptr, 27),                   // 3 + 4 * 6
        std::make_tuple("110511068301110B9101", nullptr, true),                 // 5 + 6 = 11
        std::make_tuple("17036162631701619307", nullptr, true),                 // 'abc' > 'a'
        std::make_tuple("110711088301110E930111061105950152", nullptr, false),  // 7 + 8 > 14 && 6 < 5
        std::make_tuple("1115F021", nullptr, 21LL),                             // int64(21)
        std::make_tuple("230352", nullptr, false),                              // false && null
        std::make_tuple("130352", nullptr, nullptr),                            // true && null
        std::make_tuple("01A101", nullptr, true),                               // is_null(null)
        std::make_tuple("1101A201", nullptr, true),                             // is_true(1)
        std::make_tuple("218080808008B301", nullptr, INT_MIN)                   // abs(-INT32_MAX)
        ));

static Tuple tuple1{1, 2};
static Tuple tuple2{35LL, 46LL};
static Tuple tuple3{3.5, 4.6};
static Tuple tuple4{"abc", "aBc"};

// Test cases with vars
INSTANTIATE_TEST_SUITE_P(
    VarExpr,
    ExprTest,
    testing::Values(
        std::make_tuple("3100", &tuple1, 1),                         // t0
        std::make_tuple("3101", &tuple1, 2),                         // t1
        std::make_tuple("310031018301", &tuple1, 3),                 // t0 + t1
        std::make_tuple("3200", &tuple2, 35LL),                      // t0
        std::make_tuple("3201", &tuple2, 46LL),                      // t1
        std::make_tuple("320032018302", &tuple2, 81LL),              // t0 + t1
        std::make_tuple("3500", &tuple3, 3.5),                       // t0
        std::make_tuple("3501", &tuple3, 4.6),                       // t1
        std::make_tuple("350035018305", &tuple3, 8.1),               // t0 + t1
        std::make_tuple("3501128080808008f0529505", &tuple3, true),  // t1 < 2147483648
        std::make_tuple("3700", &tuple4, "abc"),                     // t0
        std::make_tuple("3701", &tuple4, "aBc"),                     // t1
        std::make_tuple("370037019307", &tuple4, true)               // t0 < t1
        ));
