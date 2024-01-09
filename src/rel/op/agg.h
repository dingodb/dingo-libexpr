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

#ifndef _REL_OP_AGG_H_
#define _REL_OP_AGG_H_

#include "../../expr/calc/arithmetic.h"
#include "../../expr/calc/mathematic.h"
#include "../../expr/operand.h"

namespace dingodb::rel::op {

class Agg {
 public:
  Agg() = default;
  virtual ~Agg() = default;

  virtual expr::Operand Add(const expr::Operand &var, const expr::Tuple *tuple) const = 0;
};

class UnityAgg : public Agg {
 public:
  UnityAgg(int32_t index) : m_index(index) {
  }

  ~UnityAgg() override = default;

 protected:
  int32_t m_index;
};

class CountAllAgg : public Agg {
 public:
  CountAllAgg() = default;
  ~CountAllAgg() override = default;

  expr::Operand Add(const expr::Operand &var, const expr::Tuple *tuple) const override;
};

template <typename T>
class CountAgg : public UnityAgg {
 public:
  CountAgg(int32_t index) : UnityAgg(index) {
  }

  ~CountAgg() override = default;

  expr::Operand Add(const expr::Operand &var, const expr::Tuple *tuple) const override {
    if ((*tuple)[m_index] != nullptr) {
      if (var != nullptr) {
        return var.GetValue<int64_t>() + 1LL;
      }
      return 1LL;
    }
    return var;
  }
};

template <typename T, T (*Calc)(T, T)>
class CalcAgg : public UnityAgg {
 public:
  CalcAgg(int32_t index) : UnityAgg(index) {
  }

  ~CalcAgg() override = default;

  expr::Operand Add(const expr::Operand &var, const expr::Tuple *tuple) const override {
    if ((*tuple)[m_index] != nullptr) {
      auto v = ((*tuple)[m_index]).template GetValue<T>();
      if (var != nullptr) {
        return Calc(var.GetValue<T>(), v);
      }
      return v;
    }
    return var;
  }
};

template <typename T>
using SumAgg = CalcAgg<T, expr::calc::Add>;

template <typename T>
using MinAgg = CalcAgg<T, expr::calc::Min>;

template <typename T>
using MaxAgg = CalcAgg<T, expr::calc::Max>;

}  // namespace dingodb::rel::op

#endif /* _REL_OP_AGG_H_ */
