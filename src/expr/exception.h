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

#ifndef _EXPR_EXCEPTION_H_
#define _EXPR_EXCEPTION_H_

#include <limits>
#include <stdexcept>

#include "types.h"
#include "utils.h"

namespace dingodb::expr {

class ExprError : public std::runtime_error {
 public:
  ExprError(const std::string &msg) : std::runtime_error("EXPR ERROR: " + msg) {
  }
};

template <Byte B>
class ExceedsLimits : public ExprError {
 public:
  ExceedsLimits()
      : ExprError(
            std::string("Value exceeds limits of ") + TypeName(B) + ", which is from " +
            std::to_string(std::numeric_limits<TypeOf<B>>::min()) + " to " +
            std::to_string(std::numeric_limits<TypeOf<B>>::max()) + ".") {
  }
};

class UnknownCode : public ExprError {
 public:
  UnknownCode(const Byte *code, size_t len) : ExprError("Unknown code in expression, bytes: " + HexOfBytes(code, len)) {
  }
};

class MoreElementsRequired : public ExprError {
 public:
  MoreElementsRequired(int required, int actual)
      : ExprError(
            "Array decoding requires " + std::to_string(required) + " elements, but " + std::to_string(actual) +
            " provided.") {
  }
};

}  // namespace dingodb::expr

#endif /* _EXPR_EXCEPTION_H_ */
