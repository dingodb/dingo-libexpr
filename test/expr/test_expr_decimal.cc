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

#include <memory>
#include <tuple>

#include "codec.h"
#include "runner.h"
#include "decimal_p.h"

using namespace dingodb::expr;

class ExprDecimalTest : public testing::TestWithParam<std::tuple<std::string, Tuple *, std::string>> {};

TEST_P(ExprDecimalTest, Run) {
  const auto &para = GetParam();
  Runner runner;
  auto input = std::get<0>(para);
  auto len = input.size() / 2;
  Byte buf[len];
  HexToBytes(buf, input.data(), input.size());
  runner.Decode(buf, len);
  runner.BindTuple(std::get<1>(para));
  runner.Run();

  auto r = runner.Get();

  if (r.isBool()) {
    auto r1 = any_optional_data_adaptor::FromOperand<bool>(r);
    auto r2 = (*std::any_cast<std::optional<bool>>(r1));
    const std::string result = r2 ? "true" : "false";

    EXPECT_EQ(result, std::get<2>(para));
  } else {
    auto r1 = any_optional_data_adaptor::FromOperand<DecimalP>(r);
    auto r2 = (*std::any_cast<std::optional<DecimalP>>(r1))->toString();

    EXPECT_EQ(r2, std::get<2>(para));
}
}

// Test cases with consts
INSTANTIATE_TEST_SUITE_P(
    ConstDecExpr,
    ExprDecimalTest,
    testing::Values(
        //Decimal const value.
        std::make_tuple("160531322E3334", nullptr, std::string("12.34")),

        //Decimal const + Decimal const.
        std::make_tuple("160531322E3334160531322E33348306", nullptr, std::string("24.68")),
        //Decimal const - Decimal const.
        std::make_tuple("160531322E3334160531322E33348406", nullptr, std::string("0")),
        //Decimal const * Decimal const.
        std::make_tuple("160531322E3334160531322E33348506", nullptr, std::string("152.2756")),
        //Decimal const / Decimal const.
        std::make_tuple("160531322E33341601328606", nullptr, std::string("6.17")),

        //Decimal const > Decimal const.
        std::make_tuple("160531322E33341604352E36379306", nullptr, "true"),
        //Decimal const >= Decimal const.
        std::make_tuple("160531322E33341604352E36379206", nullptr, "true"),
        //Decimal const = Decimal const.
        std::make_tuple("160531322E33341604352E36379106", nullptr, "false"),
        //Decimal const < Decimal const.
        std::make_tuple("160531322E33341604352E36379406", nullptr, "false"),
        //Decimal const <= Decimal const.
        std::make_tuple("160531322E33341604352E36379506", nullptr, "false"),
        std::make_tuple("160531322E33341604352E36379606", nullptr, "true"),

        // decimal(21)
        std::make_tuple("1115F061", nullptr, "21"),
        //Decimal: 7 + 8 > 14 && 6 < 5
        std::make_tuple("1601371601388306160231349306160136160135950652", nullptr, "false"),
        //is_ture for decimal.
        std::make_tuple("160131A206", nullptr, "true"),                             // is_true(1)
        //is_null for decimal.
        std::make_tuple("06A106", nullptr, "true")                               // is_null(null)
        ));

static Tuple tupleDec1{std::make_shared<Decimal>(Decimal("123.123")), std::make_shared<Decimal>(Decimal("456.456"))};

// Test cases with vars
INSTANTIATE_TEST_SUITE_P(
    VarDecExpr,
    ExprDecimalTest,
    testing::Values(
        //VAR | DECIMAL | The 0 index.
        std::make_tuple("3600", &tupleDec1, std::string("123.123")),
        //VAR | DECIMAL | The 1st index.
        std::make_tuple("3601", &tupleDec1, std::string("456.456")),
        //(VAR |DECIMAL) + (VAR | DECIMAL).
        std::make_tuple("360036018306", &tupleDec1, std::string("579.579")),
        //CONST | DECIMAL, POS.
        std::make_tuple("16073132332E313233", nullptr, "123.123"),
        //CONST | DECIMAL, NEG.
        std::make_tuple("16082D3132332E313233", nullptr, "-123.123")
    ));