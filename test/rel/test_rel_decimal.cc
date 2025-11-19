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
      new Tuple{1, nullptr},  //column contains nullptr.
  };
}

static Data MakeDataForCast() {
  return Data{
    new Tuple{1, (double)77.77},  //column contains nullptr.
};
}

static Data MakeDataInt() {
  return Data{
    new Tuple{10, nullptr},  //column contains nullptr.
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

class PipeOpDecimalTest : public testing::TestWithParam<std::tuple<std::string, Data, Data>> {};

TEST_P(PipeOpDecimalTest, DecimalPut) {
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
    DecimalPipeOp,
    PipeOpDecimalTest,
    testing::Values(
        // select all columns without where statement.
        std::make_tuple(
            "723100360100",
            MakeData(),
            Data{
              new Tuple{1, nullptr},
            }
        ),
        // table t1(id int, a double), value (1, 77.77), select * from t1 where a >= 77.77.
        std::make_tuple(
            "713501f065160537372e3737920600",
            MakeDataForCast(),
            Data{
              new Tuple{1, 77.77},
            }
        )
    )
);

/*
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
*/