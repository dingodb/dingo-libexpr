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
      new Tuple{1, "Alice",   10.0f},
      new Tuple{2, "Betty",   20.0f},
      new Tuple{3, "Cindy",   30.0f},
      new Tuple{4, "Doris",   40.0f},
      new Tuple{5, "Emily",   50.0f},
      new Tuple{6, "Alice",   60.0f},
      new Tuple{7, "Betty",   70.0f},
      new Tuple{8, "Alice",   80.0f},
      new Tuple{9, "Cindy", nullptr},
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

class PipeOpTest : public testing::TestWithParam<std::tuple<std::string, Data, Data>> {};

TEST_P(PipeOpTest, Put) {
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
    PipeOpTest,
    testing::Values(
        // FILTER(input, $[2] > 50)
        std::make_tuple(
            "7134021442480000930400",
            MakeData(),
            Data{
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                new Tuple{6, "Alice", 60.0f},
                new Tuple{7, "Betty", 70.0f},
                new Tuple{8, "Alice", 80.0f},
                nullptr,
}  // Why code formatted to here?
        ),
        // PROJECT(input, $[0], $[1], $[2] / 10)
        std::make_tuple(
            "723100370134021441200000860400",
            MakeData(),
            Data{
                new Tuple{1, "Alice", 1.0f},
                new Tuple{2, "Betty", 2.0f},
                new Tuple{3, "Cindy", 3.0f},
                new Tuple{4, "Doris", 4.0f},
                new Tuple{5, "Emily", 5.0f},
                new Tuple{6, "Alice", 6.0f},
                new Tuple{7, "Betty", 7.0f},
                new Tuple{8, "Alice", 8.0f},
                new Tuple{9, "Cindy", nullptr},
            }
        ),
        // PROJECT(FILTER(input, $[2] > 50), $[0], $[1], $[2] / 10)
        std::make_tuple(
            "7134021442480000930400723100370134021441200000860400",
            MakeData(),
            Data{
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                new Tuple{6, "Alice", 6.0f},
                new Tuple{7, "Betty", 7.0f},
                new Tuple{8, "Alice", 8.0f},
                nullptr,
            }
        ),
        // FILTER(PROJECT(input, $[0], $[1], $[2] / 10), $[2] > 50)
        std::make_tuple(
            "7231003701340214412000008604007134021442480000930400",
            MakeData(),
            Data{
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
            }
        ),
        // FILTER(input, $[0] >= 2001 AND $[0] <= 2001)
        std::make_tuple(
            "71310011d10f9201310011d10f94015200",
            Data{
                new Tuple{2001, "Alice", 2001.0f},
            },
            Data{
                new Tuple{2001, "Alice", 2001.0f},
            }
        ),
        // PROJECT(input, STRING($[2]))
        std::make_tuple(
            "723402F07400",
            Data{
                new Tuple{1, "Alice", 1.2345678f},
            },
            Data{
                new Tuple{"1.234567761421204"},
            }
        )
    )
);

class CacheOpTest : public testing::TestWithParam<std::tuple<std::string, Data, int, Data>> {};

TEST_P(CacheOpTest, PutGet) {
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
    CacheOpTest,
    testing::Values(
        // AGG(input, COUNT(), COUNT($[2]), SUM($[2]))
        std::make_tuple(
            "74031014022402",
            MakeData(),
            1,
            Data{
                new Tuple{9LL, 8LL, 360.0f},
}  // Why code formatted to here?
        ),
        // AGG(input, GROUP(1), COUNT(), SUM($[2]))
        std::make_tuple(
            "7361010102102402",
            MakeData(),
            5,
            Data{
                new Tuple{"Alice", 3LL, 150.0f},
                new Tuple{"Betty", 2LL, 90.0f},
                new Tuple{"Cindy", 2LL, 30.0f},
                new Tuple{"Doris", 1LL, 40.0f},
                new Tuple{"Emily", 1LL, 50.0f},
            }
        ),
        // AGG(PROJECT($[2]), COUNT($[0]))
        std::make_tuple(
            "7235020074011500",
            MakeData(),
            1,
            Data{
                new Tuple{8LL},
            }
        )
    )
);
