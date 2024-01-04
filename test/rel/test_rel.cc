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

static const RelRunner *MakeRunner(const std::string &code) {
  auto len = code.size() / 2;
  Byte buf[len];
  HexToBytes(buf, code.data(), code.size());
  auto *rel = new RelRunner();
  rel->Decode(buf, len);
  return rel;
}

TEST(RelRunnerTest, Filter) {
  // FILTER(input, $[2] > 50)
  const auto *rel = MakeRunner("7134021442480000930400");
  auto *tuple = new Tuple{1, "Alice", 10.0f};
  const auto *out = rel->Put(tuple);
  EXPECT_EQ(out, nullptr);
  tuple = new Tuple{2, "Betty", 60.0f};
  out = rel->Put(tuple);
  EXPECT_EQ(out, tuple);
  delete out;
  delete rel;
}

TEST(RelRunnerTest, Project) {
  // PROJECT(input, $[0], $[1], $[2] / 10)
  const auto *rel = MakeRunner("723100370134021441200000860400");
  auto *tuple = new Tuple{1, "Alice", 10.0f};
  const auto *out = rel->Put(tuple);
  auto *result = new Tuple{1, "Alice", 1.0f};
  EXPECT_EQ(*out, *result);
  delete out;
  delete result;
  tuple = new Tuple{2, "Betty", 60.0f};
  out = rel->Put(tuple);
  result = new Tuple{2, "Betty", 6.0f};
  EXPECT_EQ(*out, *result);
  delete out;
  delete result;
  delete rel;
}

TEST(RelRunnerTest, Filter_Project) {
  // PROJECT(FILTER(input, $[2] > 50), $[0], $[1], $[2] / 10)
  const auto *rel = MakeRunner("7134021442480000930400723100370134021441200000860400");
  auto *tuple = new Tuple{1, "Alice", 10.0f};
  const auto *out = rel->Put(tuple);
  EXPECT_EQ(out, nullptr);
  tuple = new Tuple{2, "Betty", 60.0f};
  out = rel->Put(tuple);
  auto *result = new Tuple{2, "Betty", 6.0f};
  EXPECT_EQ(*out, *result);
  delete out;
  delete result;
  delete rel;
}

TEST(RelRunnerTest, Project_Filter) {
  // FILTER(PROJECT(input, $[0], $[1], $[2] / 10), $[2] > 50)
  const auto *rel = MakeRunner("7231003701340214412000008604007134021442480000930400");
  auto *tuple = new Tuple{1, "Alice", 100.0f};
  const auto *out = rel->Put(tuple);
  EXPECT_EQ(out, nullptr);
  tuple = new Tuple{2, "Betty", 600.0f};
  out = rel->Put(tuple);
  auto *result = new Tuple{2, "Betty", 60.0f};
  EXPECT_EQ(*out, *result);
  delete out;
  delete result;
  delete rel;
}

TEST(RelRunnerTest, UngroupedAgg) {
  // AGG(input, COUNT(), COUNT($[1]), SUM($[2]))
  const auto *rel = MakeRunner("74031017012402");
  for (int id = 1; id < 10; ++id) {
    auto *tuple = new Tuple{1, "ID_" + std::to_string(id), id * 10.0f};
    const auto *out = rel->Put(tuple);
    EXPECT_EQ(out, nullptr);
  }
  auto *tuple = new Tuple{10, nullptr, 100.0f};
  const auto *out = rel->Put(tuple);
  EXPECT_EQ(out, nullptr);
  tuple = new Tuple{11, "ID_11", nullptr};
  out = rel->Put(tuple);
  EXPECT_EQ(out, nullptr);
  out = rel->Get();
  auto *result = new Tuple{11LL, 10LL, 550.0f};
  EXPECT_EQ(*out, *result);
  delete out;
  delete result;
  delete rel;
}

TEST(RelRunnerTest, GroupedAgg) {
  // AGG(input, GROUP(1), COUNT(), SUM($[2]))
  const auto *rel = MakeRunner("7361010102102402");
  Tuple *tuples[]{
      new Tuple{1, "Alice", 10.0f},
      new Tuple{2, "Betty", 20.0f},
      new Tuple{3, "Cindy", 30.0f},
      new Tuple{4, "Doris", 40.0f},
      new Tuple{5, "Emily", 50.0f},
      new Tuple{6, "Alice", 60.0f},
      new Tuple{7, "Betty", 70.0f},
      new Tuple{8, "Alice", 80.0f},
      new Tuple{9, "Cindy", 90.0f},
  };
  for (const auto *tuple : tuples) {
    const auto *out = rel->Put(tuple);
    EXPECT_EQ(out, nullptr);
  }
  std::array<Tuple, 5> results{
      {
       {"Alice", 3LL, 150.0f},
       {"Betty", 2LL, 90.0f},
       {"Cindy", 2LL, 120.0f},
       {"Doris", 1LL, 40.0f},
       {"Emily", 1LL, 50.0f},
       }
  };
  for (int i = 0; i < 5; ++i) {
    const auto *out = rel->Get();
    EXPECT_TRUE(std::any_of(results.cbegin(), results.cend(), [out](const Tuple &t) { return t == *out; }));
    delete out;
  }
  const auto *out1 = rel->Get();
  EXPECT_EQ(out1, nullptr);
  delete rel;
}
