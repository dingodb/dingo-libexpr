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

#ifndef _EXPR_OPERATOR_H_
#define _EXPR_OPERATOR_H_

#include <functional>

#include "calc/casting.h"
#include "operand_stack.h"

namespace dingodb::expr {

class Operator {
 public:
  virtual ~Operator() = default;

  virtual void operator()(OperandStack &stack) const = 0;

  virtual Byte GetType() const = 0;
};

template <Byte R>
class OperatorBase : public Operator {
 public:
  Byte GetType() const override {
    return R;
  }
};

template <Byte R>
class NullOperator : public OperatorBase<R> {
 public:
  void operator()(OperandStack &stack) const override {
    stack.Push<TypeOf<R>>();
  }
};

template <Byte R>
class ConstOperator : public OperatorBase<R> {
 public:
  ConstOperator(TypeOf<R> value) : m_value(value) {
  }

  void operator()(OperandStack &stack) const override {
    stack.Push(m_value);
  }

 private:
  TypeOf<R> m_value;
};

template <bool V>
class ConstBoolOperator : public OperatorBase<TYPE_BOOL> {
 public:
  ConstBoolOperator() = default;

  void operator()(OperandStack &stack) const override {
    stack.Push(V);
  }
};

template <Byte R>
class IndexedVarOperator : public OperatorBase<R> {
 public:
  IndexedVarOperator(int32_t index) : m_index(index) {
  }

  void operator()(OperandStack &stack) const override {
    stack.PushVar(m_index);
  }

 private:
  int32_t m_index;
};

template <Byte R, Byte T, TypeOf<R> (*Calc)(TypeOf<T>)>
class UnaryOperator : public OperatorBase<R> {
 public:
  void operator()(OperandStack &stack) const override {
    auto v = stack.Get();
    stack.Pop();
    if (v != nullptr) {
      stack.Push<TypeOf<R>>(Calc(v.GetValue<TypeOf<T>>()));
    } else {
      stack.Push<TypeOf<R>>();
    }
  }
};

template <Byte R, Byte T>
using CastOperator = UnaryOperator<R, T, calc::Cast>;

template <Byte R, Byte T>
using CastCheckOperator = UnaryOperator<R, T, calc::CastCheck>;

template <Byte R, TypeOf<R> (*Calc)(TypeOf<R>)>
using UnaryArithmeticOperator = UnaryOperator<R, R, Calc>;

template <bool (*Calc)(const Operand &)>
class UnarySpecialOperator : public OperatorBase<TYPE_BOOL> {
 public:
  void operator()(OperandStack &stack) const override {
    auto v = stack.Get();
    stack.Pop();
    stack.Push<bool>(Calc(v));
  }
};

template <Byte R, Byte T0, Byte T1, TypeOf<R> (*Calc)(TypeOf<T0>, TypeOf<T1>)>
class BinaryOperator : public OperatorBase<R> {
 public:
  void operator()(OperandStack &stack) const override {
    auto v1 = stack.Get();
    stack.Pop();
    auto v0 = stack.Get();
    stack.Pop();
    if (v0 != nullptr && v1 != nullptr) {
      stack.Push(Calc(v0.GetValue<TypeOf<T0>>(), v1.GetValue<TypeOf<T1>>()));
    } else {
      stack.Push<TypeOf<R>>();
    }
  }
};

template <Byte R, Byte T0, Byte T1, Operand (*Calc)(TypeOf<T0>, TypeOf<T1>)>
class BinaryOperatorV2 : public OperatorBase<R> {
 public:
  void operator()(OperandStack &stack) const override {
    auto v1 = stack.Get();
    stack.Pop();
    auto v0 = stack.Get();
    stack.Pop();
    if (v0 != nullptr && v1 != nullptr) {
      stack.Push(Calc(v0.GetValue<TypeOf<T0>>(), v1.GetValue<TypeOf<T1>>()));
    } else {
      stack.Push<TypeOf<R>>();
    }
  }
};

template <Byte R, TypeOf<R> (*Calc)(TypeOf<R>, TypeOf<R>)>
using BinaryArithmeticOperator = BinaryOperator<R, R, R, Calc>;

template <Byte T, bool (*Calc)(TypeOf<T>, TypeOf<T>)>
using BinaryRelationOperator = BinaryOperator<TYPE_BOOL, T, T, Calc>;

template <Byte R, Byte T0, Byte T1, Byte T2, TypeOf<R> (*Calc)(TypeOf<T0>, TypeOf<T1>, TypeOf<T2>)>
class TertiaryOperator : public OperatorBase<R> {
 public:
  void operator()(OperandStack &stack) const override {
    auto v2 = stack.Get();
    stack.Pop();
    auto v1 = stack.Get();
    stack.Pop();
    auto v0 = stack.Get();
    stack.Pop();
    if (v0 != nullptr && v1 != nullptr && v2 != nullptr) {
      stack.Push(Calc(v0.GetValue<TypeOf<T0>>(), v1.GetValue<TypeOf<T1>>(), v2.GetValue<TypeOf<T2>>()));
    } else {
      stack.Push<TypeOf<R>>();
    }
  }
};

class NotOperator : public OperatorBase<TYPE_BOOL> {
 public:
  void operator()(OperandStack &stack) const override;
};

class AndOperator : public OperatorBase<TYPE_BOOL> {
 public:
  void operator()(OperandStack &stack) const override;
};

class OrOperator : public OperatorBase<TYPE_BOOL> {
 public:
  void operator()(OperandStack &stack) const override;
};

}  // namespace dingodb::expr

#endif /* _EXPR_OPERATOR_H_ */
