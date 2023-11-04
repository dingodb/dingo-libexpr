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

#ifndef _OPERATOR_VECTOR_H_
#define _OPERATOR_VECTOR_H_

#include <vector>

#include "operator.h"

namespace dingodb::expr
{

class OperatorVector
{
public:
    OperatorVector() : m_vector(), m_toRelease()
    {
    }

    virtual ~OperatorVector()
    {
        Release();
    }

    void Decode(const Byte code[], size_t len);

    Byte GetType() const
    {
        return m_vector.back()->GetType();
    }

    auto begin()
    {
        return m_vector.begin();
    }

    auto end()
    {
        return m_vector.end();
    }

private:
    std::vector<const Operator *> m_vector;
    std::vector<const Operator *> m_toRelease;

    static std::string ConvertBytesToHex(const Byte *data, size_t len);

    void Add(const Operator *op)
    {
        m_vector.push_back(op);
    }

    void AddRelease(const Operator *op)
    {
        m_vector.push_back(op);
        m_toRelease.push_back(op);
    }

    void Release()
    {
        for (auto op : m_toRelease) {
            delete op;
        }
        m_toRelease.clear();
        m_vector.clear();
    }

    /**
     * @brief Add an operator of the specified type.
     *
     * @param ops The array of the operators
     * @param type The type byte
     * @return true Successful
     * @return false Failed
     */
    [[nodiscard]] bool AddOperatorByType(const Operator *const ops[], Byte type);

    /**
     * @brief  Add a cast operator of the specified type.
     *
     * @param b The byte indicating the source and target type
     * @return true Successful
     * @return false Failed
     */
    [[nodiscard]] bool AddCastOperator(Byte b);

    [[nodiscard]] bool AddFunOperator(Byte b);
};

} // namespace dingodb::expr

#endif /* _OPERATOR_VECTOR_H_ */
