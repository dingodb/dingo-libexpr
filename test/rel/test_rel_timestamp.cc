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
      new Tuple{1, 1,   1701403932000},   //2023-12-01 12:12:12
      new Tuple{2, 2,   1701490332000},   //2023-12-02 12:12:12
      new Tuple{3, 3,   1701576732000},   //2023-12-03 12:12:12
      new Tuple{4, 4,   1701663132000},   //2023-12-04 12:12:12
      new Tuple{5, 5,   1704082332000},   //2024-01-01 12:12:12
      new Tuple{6, 6,   1706847132000},   //2024-02-02 12:12:12
      new Tuple{7, 7,   1735704732000},   //2025-01-01 12:12:12
      new Tuple{8, 8,   1735791132000},   //2025-01-02 12:12:12
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

class PipeOpTimestampTest : public testing::TestWithParam<std::tuple<std::string, Data, Data>> {};

TEST_P(PipeOpTimestampTest, Put) {
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
    PipeOpTimestampTest,
    testing::Values(
        // FILTER(input, $[2] = '2024-01-01 12:12:12')
        std::make_tuple(
            "71390219E0B2E399CC31910900",
            MakeData(),
            Data{
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                new Tuple{5, 5, 1704082332000},
                nullptr,
                nullptr,
                nullptr,
                nullptr,
            }
        ),
        // PROJECT(input, $[2])
        std::make_tuple(
            "72390200",
            MakeData(),
            Data{
                new Tuple{1701403932000},   //2023-12-01 12:12:12
                new Tuple{1701490332000},   //2023-12-02 12:12:12
                new Tuple{1701576732000},   //2023-12-03 12:12:12
                new Tuple{1701663132000},   //2023-12-04 12:12:12
                new Tuple{1704082332000},   //2024-01-01 12:12:12
                new Tuple{1706847132000},   //2024-02-02 12:12:12
                new Tuple{1735704732000},   //2025-01-01 12:12:12
                new Tuple{1735791132000},   //2025-01-02 12:12:12
                new Tuple{nullptr},
            }
        )
    )
);

class CacheOpTimestampTest : public testing::TestWithParam<std::tuple<std::string, Data, int, Data>> {};

TEST_P(CacheOpTimestampTest, PutGet) {
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
    CacheOpTimestampTest,
    testing::Values(
        // min(timestamp)
        std::make_tuple(
            "7239020074014900",
            MakeData(),
            1,
            Data{
                new Tuple{1701403932000},
            }
        ),
        // max(timestamp)
        std::make_tuple(
            "7239020074013900",
            MakeData(),
            1,
            Data{
                new Tuple{1735791132000},
            }
        ),
        // count(timestamp)
        std::make_tuple(
            "7239020074011900",
            MakeData(),
            1,
            Data{
                new Tuple{8LL},
            }
        ),
        // timestamp = null
        std::make_tuple(
            "71390209910900",
            MakeData(),
            0,    //The result is 0 rows.
            Data{
                }
        )
    )
);
