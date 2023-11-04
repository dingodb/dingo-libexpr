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

#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#include <functional>

#include "operand_stack.h"
#include "types.h"

#include "calc/casting.h"

namespace dingodb::expr
{

class Operator
{
public:
    virtual ~Operator()
    {
    }

    virtual void operator()(OperandStack &stack) const = 0;

    virtual Byte GetType() const = 0;
};

template <Byte R> class OperatorBase : public Operator
{
public:
    Byte GetType() const override
    {
        return R;
    }
};

template <Byte R> class NullOperator : public OperatorBase<R>
{
public:
    void operator()(OperandStack &stack) const override
    {
        stack.Push<TypeOf<R>>();
    }
};

template <Byte R> class ConstOperator : public OperatorBase<R>
{
public:
    ConstOperator(TypeOf<R> value) : m_value(value)
    {
    }

    void operator()(OperandStack &stack) const override
    {
        stack.Push(m_value);
    }

private:
    TypeOf<R> m_value;
};

template <bool V> class ConstBoolOperator : public OperatorBase<TYPE_BOOL>
{
public:
    ConstBoolOperator()
    {
    }

    void operator()(OperandStack &stack) const override
    {
        stack.Push(V);
    }
};

template <Byte R> class IndexedVarOperator : public OperatorBase<R>
{
public:
    IndexedVarOperator(uint32_t index) : m_index(index)
    {
    }

    void operator()(OperandStack &stack) const override
    {
        stack.PushVar(m_index);
    }

private:
    uint32_t m_index;
};

template <Byte R, Byte T, TypeOf<R> (*Calc)(TypeOf<T>)> class UnaryOperator : public OperatorBase<R>
{
public:
    void operator()(OperandStack &stack) const override
    {
        auto v = stack.Get<TypeOf<T>>();
        if (v.has_value()) {
            stack.Set<TypeOf<R>>(Calc(*v));
        } else {
            stack.Set<TypeOf<R>>();
        }
    }
};

template <Byte R, Byte T> using CastOperator = UnaryOperator<R, T, calc::Cast>;

template <Byte R, TypeOf<R> (*Calc)(TypeOf<R>)> using UnaryArithmeticOperator = UnaryOperator<R, R, Calc>;

template <Byte T, bool (*Calc)(const wrap<TypeOf<T>> &)> class UnarySpecialOperator : public OperatorBase<TYPE_BOOL>
{
public:
    void operator()(OperandStack &stack) const override
    {
        auto v = stack.Get<TypeOf<T>>();
        stack.Set<bool>(Calc(v));
    }
};

template <Byte R, Byte T0, Byte T1, TypeOf<R> (*Calc)(TypeOf<T0>, TypeOf<T1>)>
class BinaryOperator : public OperatorBase<R>
{
public:
    void operator()(OperandStack &stack) const override
    {
        auto v1 = stack.Get<TypeOf<T1>>();
        stack.Pop();
        auto v0 = stack.Get<TypeOf<T0>>();
        if (v0.has_value() && v1.has_value()) {
            stack.Set<TypeOf<R>>(Calc(*v0, *v1));
        } else {
            stack.Set<TypeOf<R>>();
        }
    }
};

template <Byte R, Byte T0, Byte T1, wrap<TypeOf<R>> (*Calc)(const wrap<TypeOf<T0>> &, const wrap<TypeOf<T1>> &)>
class BinaryOperatorV1 : public OperatorBase<R>
{
public:
    void operator()(OperandStack &stack) const override
    {
        auto v1 = stack.Get<TypeOf<T1>>();
        stack.Pop();
        auto v0 = stack.Get<TypeOf<T0>>();
        stack.SetWrapped<TypeOf<R>>(Calc(v0, v1));
    }
};

template <Byte R, TypeOf<R> (*Calc)(TypeOf<R>, TypeOf<R>)>
using BinaryArithmeticOperator = BinaryOperator<R, R, R, Calc>;

template <Byte T, bool (*Calc)(TypeOf<T>, TypeOf<T>)>
using BinaryRelationOperator = BinaryOperator<TYPE_BOOL, T, T, Calc>;

class NotOperator : public OperatorBase<TYPE_BOOL>
{
public:
    void operator()(OperandStack &stack) const override;
};

class AndOperator : public OperatorBase<TYPE_BOOL>
{
public:
    void operator()(OperandStack &stack) const override;
};

class OrOperator : public OperatorBase<TYPE_BOOL>
{
public:
    void operator()(OperandStack &stack) const override;
};

} // namespace dingodb::expr

#endif /* _OPERATOR_H_ */
