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

namespace dingodb::expr {

template <>
testing::AssertionResult Equals<TYPE_STRING>(const Operand &actual, const Operand &expected) {
  if (NotNull<String>(actual)) {
    auto a = *GetValue<String>(actual);
    if (NotNull<String>(expected)) {
      auto e = *GetValue<String>(expected);
      if (a == e) {
        return testing::AssertionSuccess();
      }
      return testing::AssertionFailure() << a << " != " << e;
    }
    return testing::AssertionFailure() << a << " != null";
  } else if (NotNull<String>(expected)) {
    auto e = *GetValue<String>(expected);
    return testing::AssertionFailure() << "null != " << e;
  }
  return testing::AssertionSuccess();
}

testing::AssertionResult EqualsByType(int type, const Operand &actual, const Operand &expected) {
  switch (type) {
  case TYPE_INT32:
    return Equals<TYPE_INT32>(actual, expected);
    break;
  case TYPE_INT64:
    return Equals<TYPE_INT64>(actual, expected);
    break;
  case TYPE_BOOL:
    return Equals<TYPE_BOOL>(actual, expected);
    break;
  case TYPE_FLOAT:
    return Equals<TYPE_FLOAT>(actual, expected);
    break;
  case TYPE_DOUBLE:
    return Equals<TYPE_DOUBLE>(actual, expected);
    break;
  case TYPE_STRING:
    return Equals<TYPE_STRING>(actual, expected);
  default:
    return testing::AssertionFailure() << "Unsupported type code " << type << " in assertion.";
    break;
  }
}

testing::AssertionResult TupleEquals(int type[], const Tuple *a, const Tuple *b) {
  if (a->size() != b->size()) {
    return testing::AssertionFailure() << "The two tuples have diffent size " << a->size() << " and " << b->size()
                                       << ".";
  }
  for (auto i = 0; i < a->size(); ++i) {
    testing::AssertionResult result = EqualsByType(type[i], (*a)[i], (*b)[i]);
    if (!result) {
      return result;
    }
  }
  return testing::AssertionSuccess();
}

}  // namespace dingodb::expr
