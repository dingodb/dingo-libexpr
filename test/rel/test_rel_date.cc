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

#include <array>

#include "expr/codec.h"
#include "rel/rel_runner.h"

using namespace dingodb::expr;
using namespace dingodb::rel;

using Data = std::vector<Tuple *>;

static void ReleaseData(const Data &data) {
  for (const auto *t : data) {
    delete t;
  }
}

static Data MakeData() {
  return Data{
      new Tuple{1, 1,   1701388800000},   //2023-12-01
      new Tuple{2, 2,   1701475200000},   //2023-12-02
      new Tuple{3, 3,   1701561600000},   //2023-12-03
      new Tuple{4, 4,   1701648000000},   //2023-12-04
      new Tuple{5, 5,   1704067200000},   //2024-01-01
      new Tuple{6, 6,   1704153600000},   //2024-02-02
      new Tuple{7, 7,   1735689600000},   //2025-01-01
      new Tuple{8, 8,   1735776000000},   //2025-01-02
      new Tuple{9, 9,   nullptr},
  };
}

static const RelRunner *MakeRunner(const std::string &code) {
  auto len = code.size() / 2;
  Byte buf[len];
  HexToBytes(buf, code.data(), code.size());
  auto *rel = new RelRunner();
  rel->Decode(buf, len);
  return rel;
}

class PipeOpDateTest : public testing::TestWithParam<std::tuple<std::string, Data, Data>> {};

TEST_P(PipeOpDateTest, Put) {
  const auto &para   = GetParam();
  const auto *rel    = MakeRunner(std::get<0>(para));
  const auto &data   = std::get<1>(para);
  const auto &result = std::get<2>(para);
  for (int i = 0; i < data.size(); ++i) {
    const auto *out = rel->Put(data[i]);
    if (out == nullptr) {
      EXPECT_EQ(out, result[i]);
    } else {
      EXPECT_EQ(*out, *(result[i]));
    }
    delete out;
  }
  delete rel;
  ReleaseData(result);
}

INSTANTIATE_TEST_SUITE_P(
    PipeOp,
    PipeOpDateTest,
    testing::Values(
        // FILTER(input, $[2] = '2024-01-01')
        std::make_tuple(
            "7138021880E8C792CC31910800",
            MakeData(),
            Data{
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                new Tuple{5, 5, 1704067200000},
                nullptr,
                nullptr,
                nullptr,
                nullptr,
            }
        ),
        // PROJECT(input, $[2])
        std::make_tuple(
            "72380200",
            MakeData(),
            Data{
                new Tuple{1701388800000},   //2023-12-01
                new Tuple{1701475200000},   //2023-12-02
                new Tuple{1701561600000},   //2023-12-03
                new Tuple{1701648000000},   //2023-12-04
                new Tuple{1704067200000},   //2024-01-01
                new Tuple{1704153600000},   //2024-02-02
                new Tuple{1735689600000},   //2025-01-01
                new Tuple{1735776000000},   //2025-01-02
                new Tuple{nullptr},
            }
        )
    )
);

class CacheOpDateTest : public testing::TestWithParam<std::tuple<std::string, Data, int, Data>> {};

TEST_P(CacheOpDateTest, PutGet) {
  const auto &para   = GetParam();
  const auto *rel    = MakeRunner(std::get<0>(para));
  const auto &data   = std::get<1>(para);
  int num            = std::get<2>(para);
  const auto &result = std::get<3>(para);
  for (const auto *tuple : data) {
    const auto *out = rel->Put(tuple);
    EXPECT_EQ(out, nullptr);
  }
  for (int i = 0; i < num; ++i) {
    const auto *out = rel->Get();
    EXPECT_TRUE(std::any_of(result.cbegin(), result.cend(), [out](const Tuple *t) { return *t == *out; }));
    delete out;
  }
  const auto *out1 = rel->Get();
  EXPECT_EQ(out1, nullptr);
  delete rel;
  ReleaseData(result);
}

INSTANTIATE_TEST_SUITE_P(
    CacheOp,
    CacheOpDateTest,
    testing::Values(
        // min(date)
        std::make_tuple(
            "7238020074014800",
            MakeData(),
            1,
            Data{
                new Tuple{1701388800000},
            }
        ),
        // max(date)
        std::make_tuple(
            "7238020074013800",
            MakeData(),
            1,
            Data{
                new Tuple{1735776000000},
            }
        ),
        // count(date)
        std::make_tuple(
            "7238020074011800",
            MakeData(),
            1,
            Data{
                new Tuple{8LL},
            }
        ),
        // date = null
        std::make_tuple(
            "71380208910800",
            MakeData(),
            0,    //The result is 0 rows.
            Data{
                }
        )
    )
);
