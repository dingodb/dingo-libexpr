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

#ifndef _OPERATORS_H_
#define _OPERATORS_H_

#include "operator.h"

namespace dingodb::expr
{

extern const Operator *const OP_NULL[TYPE_NUM];

extern const Operator *const OP_CONST_TRUE;
extern const Operator *const OP_CONST_FALSE;

extern const Operator *const OP_CAST[TYPE_NUM][TYPE_NUM];

extern const Operator *const OP_POS[TYPE_NUM];
extern const Operator *const OP_NEG[TYPE_NUM];
extern const Operator *const OP_ADD[TYPE_NUM];
extern const Operator *const OP_SUB[TYPE_NUM];
extern const Operator *const OP_MUL[TYPE_NUM];
extern const Operator *const OP_DIV[TYPE_NUM];
extern const Operator *const OP_EQ[TYPE_NUM];
extern const Operator *const OP_NE[TYPE_NUM];
extern const Operator *const OP_GT[TYPE_NUM];
extern const Operator *const OP_GE[TYPE_NUM];
extern const Operator *const OP_LT[TYPE_NUM];
extern const Operator *const OP_LE[TYPE_NUM];

extern const Operator *const OP_IS_NULL[TYPE_NUM];
extern const Operator *const OP_IS_TRUE[TYPE_NUM];
extern const Operator *const OP_IS_FALSE[TYPE_NUM];

extern const Operator *const OP_NOT;
extern const Operator *const OP_AND;
extern const Operator *const OP_OR;

extern const Operator *const OP_FUN[];

} // namespace dingodb::expr

#endif /* _OPERATORS_H_ */
