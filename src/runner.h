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

#ifndef _RUNNER_H_
#define _RUNNER_H_

#include "operand_stack.h"
#include "operator_vector.h"
#include "types.h"

namespace dingodb::expr
{

class Runner
{
public:
    Runner() : m_operandStack(), m_operatorVector()
    {
    }

    virtual ~Runner()
    {
    }

    void Decode(const Byte *code, size_t len)
    {
        m_operatorVector.Decode(code, len);
    }

    void BindTuple(const Tuple *tuple, bool ownTuple = false)
    {
        m_operandStack.BindTuple(tuple, ownTuple);
    }

    void Run()
    {
        m_operandStack.Clear();
        for (auto op : m_operatorVector) {
            (*op)(m_operandStack);
        }
    }

    Operand GetRawResult() const
    {
        return m_operandStack.GetRaw();
    }

    template <typename T> wrap<T> GetResult() const
    {
        return m_operandStack.Get<T>();
    }

    Byte GetType()
    {
        return m_operatorVector.GetType();
    }

private:
    OperandStack m_operandStack;
    OperatorVector m_operatorVector;
};

} // namespace dingodb::expr

#endif /* _RUNNER_H_ */
