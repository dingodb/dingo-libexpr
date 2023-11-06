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

#include <climits>
#include <memory>
#include <tuple>

#include "assertions.h"
#include "codec.h"
#include "runner.h"

using namespace dingodb::expr;

class ExprTest : public testing::TestWithParam<std::tuple<std::string, Tuple *, std::any>>
{
};

TEST_P(ExprTest, Run)
{
    const auto &para = GetParam();
    Runner runner;
    auto input = std::get<0>(para);
    auto len = input.size() / 2;
    Byte buf[len];
    HexToBytes(buf, input.data(), input.size());
    runner.Decode(buf, len);
    runner.BindTuple(std::get<1>(para));
    runner.Run();
    auto result = runner.GetRawResult();
    EXPECT_TRUE(EqualsByType(runner.GetType(), result, std::get<2>(para)));
}

// Test cases with consts
INSTANTIATE_TEST_SUITE_P(
    ConstExpr,
    ExprTest,
    testing::Values(
        std::make_tuple("1101", nullptr, Wrap<int32_t>(1)),                      // 1
        std::make_tuple("2101", nullptr, Wrap<int32_t>(-1)),                     // -1
        std::make_tuple("119601", nullptr, Wrap<int32_t>(150)),                  // 150
        std::make_tuple("219601", nullptr, Wrap<int32_t>(-150)),                 // -150
        std::make_tuple("13", nullptr, Wrap<bool>(true)),                        // true
        std::make_tuple("23", nullptr, Wrap<bool>(false)),                       // false
        std::make_tuple("15401F333333333333", nullptr, Wrap<double>(7.8)),       // 7.8
        std::make_tuple("15400921FB4D12D84A", nullptr, Wrap<double>(3.1415926)), // 3.1415926
        std::make_tuple("1541B1E1A300000000", nullptr, Wrap<double>(3E8)),       // 3E8

        std::make_tuple(
            "1703616263",
            nullptr,
            Wrap<std::shared_ptr<std::string>>(std::make_shared<std::string>("abc"))
        ), // 'abc'

        std::make_tuple("110111018301", nullptr, Wrap<int32_t>(2)),          // 1 + 1
        std::make_tuple("110211038301", nullptr, Wrap<int32_t>(5)),          // 2 + 3
        std::make_tuple("120112018302", nullptr, Wrap<int64_t>(2)),          // 1L + 1L
        std::make_tuple("120212038302", nullptr, Wrap<int64_t>(5)),          // 2L + 3L
        std::make_tuple("11031104110685018301", nullptr, Wrap<int32_t>(27)), // 3 + 4 * 6
        std::make_tuple("110511068301110B9101", nullptr, Wrap<bool>(true)),  // 5 + 6 = 11
        std::make_tuple("17036162631701619307", nullptr, Wrap<bool>(true)),  // 'abc' > 'a'

        std::make_tuple(
            "110711088301110E930111061105950152",
            nullptr,
            Wrap<bool>(false)
        ), // 7 + 8 > 14 && 6 < 5

        std::make_tuple("1115F021", nullptr, Wrap<int64_t>(21)), // int64(21)
        std::make_tuple("230352", nullptr, Wrap<bool>(false)),   // false && null
        std::make_tuple("130352", nullptr, Wrap<bool>()),        // true && null
        std::make_tuple("01A101", nullptr, Wrap<bool>(true)),    // is_null(null)
        std::make_tuple("1101A201", nullptr, Wrap<bool>(true)),  // is_true(1)

        std::make_tuple("218080808008B301", nullptr, Wrap<int32_t>(INT_MIN)) // abs(-INT32_MAX)
    )
);

static Tuple tuple1{Wrap<int32_t>(1), Wrap<int32_t>(2)};
static Tuple tuple2{Wrap<int64_t>(35), Wrap<int64_t>(46)};
static Tuple tuple3{Wrap<double>(3.5), Wrap<double>(4.6)};
static Tuple tuple4{
    Wrap<std::shared_ptr<std::string>>(std::make_shared<std::string>("abc")),
    Wrap<std::shared_ptr<std::string>>(std::make_shared<std::string>("aBc"))
};

// Test cases with vars
INSTANTIATE_TEST_SUITE_P(
    VarExpr,
    ExprTest,
    testing::Values(
        std::make_tuple("3100", &tuple1, Wrap<int32_t>(1)),                     // t0
        std::make_tuple("3101", &tuple1, Wrap<int32_t>(2)),                     // t1
        std::make_tuple("310031018301", &tuple1, Wrap<int32_t>(3)),             // t0 + t1
        std::make_tuple("3200", &tuple2, Wrap<int64_t>(35)),                    // t0
        std::make_tuple("3201", &tuple2, Wrap<int64_t>(46)),                    // t1
        std::make_tuple("320032018302", &tuple2, Wrap<int64_t>(81)),            // t0 + t1
        std::make_tuple("3500", &tuple3, Wrap<double>(3.5)),                    // t0
        std::make_tuple("3501", &tuple3, Wrap<double>(4.6)),                    // t1
        std::make_tuple("350035018305", &tuple3, Wrap<double>(8.1)),            // t0 + t1
        std::make_tuple("3501128080808008f0529505", &tuple3, Wrap<bool>(true)), // t1 < 2147483648

        std::make_tuple(
            "3700",
            &tuple4,
            Wrap<std::shared_ptr<std::string>>(std::make_shared<std::string>("abc"))
        ), // t0
        std::make_tuple(
            "3701",
            &tuple4,
            Wrap<std::shared_ptr<std::string>>(std::make_shared<std::string>("aBc"))
        ), // t1

        std::make_tuple("370037019307", &tuple4, Wrap<bool>(true)) // t0 < t1
    )
);
