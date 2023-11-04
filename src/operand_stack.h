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

#ifndef _OPERAND_STACK_H_
#define _OPERAND_STACK_H_

#include <stack>
#include <stdexcept>

#include "operand.h"

namespace dingodb::expr
{

class OperandStack
{
public:
    OperandStack() : m_stack(), m_tuple(nullptr), m_ownTuple(false)
    {
    }

    virtual ~OperandStack()
    {
        Release();
    }

    void Pop()
    {
        m_stack.pop();
    }

    Operand GetRaw() const
    {
        return m_stack.top();
    }

    template <typename T> wrap<T> Get() const
    {
        return std::any_cast<wrap<T>>(m_stack.top());
    }

    void PushRaw(const Operand &v)
    {
        m_stack.push(v);
    }

    template <typename T> void Push(T v)
    {
        m_stack.push(Operand(wrap<T>(v)));
    }

    template <typename T> void Push()
    {
        m_stack.push(Operand(wrap<T>()));
    }

    template <typename T> void SetWrapped(const wrap<T> &v)
    {
        m_stack.top().emplace<wrap<T>>(v);
    }

    template <typename T> void Set(T v)
    {
        m_stack.top().emplace<wrap<T>>(wrap<T>(v));
    }

    template <typename T> void Set()
    {
        m_stack.top().emplace<wrap<T>>(wrap<T>());
    }

    void BindTuple(const Tuple *tuple, bool ownTuple)
    {
        Release();
        m_tuple = tuple;
        m_ownTuple = ownTuple;
    }

    void PushVar(uint32_t index)
    {
        if (m_tuple != nullptr) {
            m_stack.push((*m_tuple)[index]);
        } else {
            throw std::runtime_error("No tuple provided.");
        }
    }

    void Clear()
    {
        while (!m_stack.empty()) {
            m_stack.pop();
        }
    }

private:
    std::stack<Operand> m_stack;
    const Tuple *m_tuple;
    bool m_ownTuple;

    void Release()
    {
        if (m_ownTuple) {
            delete m_tuple;
        }
    }
};

} // namespace dingodb::expr

#endif /* _OPERAND_STACK_H_ */
