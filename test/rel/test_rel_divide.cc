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
      //new Tuple{nullptr, 300},
      //new Tuple{300, nullptr},
      //new Tuple{nullptr, nullptr},
      //new Tuple{300, 3600}
      //new Tuple{0, 300},
  };
}

static Data MakeDataLong() {
  return Data{
      //new Tuple{nullptr, 300L},
      //new Tuple{300L, nullptr},
      //new Tuple{nullptr, nullptr},
      //new Tuple{300L, 3600L},
      //new Tuple{0L, 300L},
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

class PipeOpDevideTest : public testing::TestWithParam<std::tuple<std::string, Data, Data>> {};

TEST_P(PipeOpDevideTest, Devide) {
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
    PipeOpInt,
    PipeOpDevideTest,
    testing::Values(
        // FILTER(input, $[2] > 50)
        std::make_tuple(
            "72310011901C860500",
            MakeData(),
            Data{
                //new Tuple{nullptr},
                //new Tuple{nullptr},
                //new Tuple{nullptr},
                //new Tuple{0.0833333},
                //new Tuple{0}
}  // Why code formatted to here?
        )
    )
);

INSTANTIATE_TEST_SUITE_P(
    PipeOpInt1,
    PipeOpDevideTest,
    testing::Values(
        // FILTER(input, $[2] > 50)
        std::make_tuple(
            "72310011901C860100",
            MakeData(),
            Data{
                //new Tuple{nullptr},
                //new Tuple{nullptr},
                //new Tuple{nullptr},
                //new Tuple{0.0833333}
                //new Tuple{0}
            }  // Why code formatted to here?
        )
    )
);

INSTANTIATE_TEST_SUITE_P(
    PipeOpLong,
    PipeOpDevideTest,
    testing::Values(
        // FILTER(input, $[2] > 50)
        std::make_tuple(
            "72320011901C860500",
            MakeDataLong(),
            Data{
                //new Tuple{nullptr},
                //new Tuple{nullptr},
                //new Tuple{nullptr},
                //new Tuple{0.0833333},
                //new Tuple{0}
            }  // Why code formatted to here?
        )
    )
);


