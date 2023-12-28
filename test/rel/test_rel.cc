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

#include "assertions.h"
#include "expr/codec.h"
#include "expr/utils.h"
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
  auto *tuple1 = MakeTuple(1, MakeString("Alice"), 10.0f);
  auto *out1 = rel->Put(tuple1);
  EXPECT_EQ(out1, nullptr);
  auto *tuple2 = MakeTuple(2, MakeString("Betty"), 60.0f);
  auto *out2 = rel->Put(tuple2);
  EXPECT_EQ(out2, tuple2);
  delete out2;
  delete rel;
}

TEST(RelRunnerTest, Project) {
  // PROJECT(input, $[0], $[1], $[2] / 10)
  const auto *rel = MakeRunner("723100370134021441200000860400");
  int32_t types[]{TYPE_INT32, TYPE_STRING, TYPE_FLOAT};
  auto *tuple1 = MakeTuple(1, MakeString("Alice"), 10.0f);
  auto *out1 = rel->Put(tuple1);
  auto *result1 = MakeTuple(1, MakeString("Alice"), 1.0f);
  EXPECT_TRUE(TupleEquals(types, out1, result1));
  delete out1;
  delete result1;
  auto *tuple2 = MakeTuple(2, MakeString("Betty"), 60.0f);
  auto *out2 = rel->Put(tuple2);
  auto *result2 = MakeTuple(2, MakeString("Betty"), 6.0f);
  EXPECT_TRUE(TupleEquals(types, out2, result2));
  delete out2;
  delete result2;
  delete rel;
}

TEST(RelRunnerTest, Filter_Project) {
  // PROJECT(FILTER(input, $[2] > 50), $[0], $[1], $[2] / 10)
  const auto *rel = MakeRunner("7134021442480000930400723100370134021441200000860400");
  int32_t types[]{TYPE_INT32, TYPE_STRING, TYPE_FLOAT};
  auto *tuple1 = MakeTuple(1, MakeString("Alice"), 10.0f);
  auto *out1 = rel->Put(tuple1);
  EXPECT_EQ(out1, nullptr);
  auto *tuple2 = MakeTuple(2, MakeString("Betty"), 60.0f);
  auto *out2 = rel->Put(tuple2);
  auto *result2 = MakeTuple(2, MakeString("Betty"), 6.0f);
  EXPECT_TRUE(TupleEquals(types, out2, result2));
  delete out2;
  delete result2;
  delete rel;
}

TEST(RelRunnerTest, Project_Filter) {
  // FILTER(PROJECT(input, $[0], $[1], $[2] / 10), $[2] > 50)
  const auto *rel = MakeRunner("7231003701340214412000008604007134021442480000930400");
  int32_t types[]{TYPE_INT32, TYPE_STRING, TYPE_FLOAT};
  auto *tuple1 = MakeTuple(1, MakeString("Alice"), 100.0f);
  auto *out1 = rel->Put(tuple1);
  EXPECT_EQ(out1, nullptr);
  auto *tuple2 = MakeTuple(2, MakeString("Betty"), 600.0f);
  auto *out2 = rel->Put(tuple2);
  auto *result2 = MakeTuple(2, MakeString("Betty"), 60.0f);
  EXPECT_TRUE(TupleEquals(types, out2, result2));
  delete out2;
  delete result2;
  delete rel;
}

TEST(RelRunnerTest, Agg) {
  // AGG(input, COUNT(), COUNT($[1]), SUM($[2]))
  const auto *rel = MakeRunner("74031017012402");
  int32_t types[]{TYPE_INT32, TYPE_STRING, TYPE_FLOAT};
  auto *tuple1 = MakeTuple(1, MakeString("Alice"), 100.0f);
  auto *out1 = rel->Put(tuple1);
  EXPECT_EQ(out1, nullptr);
  auto *tuple2 = MakeTuple(2, MakeString("Betty"), 600.0f);
  auto *out2 = rel->Put(tuple2);
  EXPECT_EQ(out2, nullptr);
  delete rel;
}
