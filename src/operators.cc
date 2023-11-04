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

#include "operators.h"

#include "calc/arithmetic.h"
#include "calc/mathematic.h"
#include "calc/relational.h"
#include "calc/special.h"
#include "calc/string_fun.h"

namespace dingodb::expr
{

const Operator *const OP_NULL[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new NullOperator<TYPE_INT32>,
    [TYPE_INT64] = new NullOperator<TYPE_INT64>,
    [TYPE_BOOL] = new NullOperator<TYPE_BOOL>,
    [TYPE_FLOAT] = new NullOperator<TYPE_FLOAT>,
    [TYPE_DOUBLE] = new NullOperator<TYPE_DOUBLE>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new NullOperator<TYPE_STRING>,
};

const Operator *const OP_CONST_TRUE = new ConstBoolOperator<true>;
const Operator *const OP_CONST_FALSE = new ConstBoolOperator<false>;

/* clang-format off */
const Operator *const OP_CAST[][TYPE_NUM] = {
    [TYPE_NULL] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    [TYPE_INT32] = {
        [TYPE_NULL] = nullptr,
        [TYPE_INT32] = nullptr,
        [TYPE_INT64] = new CastOperator<TYPE_INT32, TYPE_INT64>,
        [TYPE_BOOL] = new CastOperator<TYPE_INT32, TYPE_BOOL>,
        [TYPE_FLOAT] = new CastOperator<TYPE_INT32, TYPE_FLOAT>,
        [TYPE_DOUBLE] = new CastOperator<TYPE_INT32, TYPE_DOUBLE>,
        [TYPE_DECIMAL] = nullptr,
        [TYPE_STRING] = new CastOperator<TYPE_INT32, TYPE_STRING>,
    },
    [TYPE_INT64] = {
        [TYPE_NULL] = nullptr,
        [TYPE_INT32] = new CastOperator<TYPE_INT64, TYPE_INT32>,
        [TYPE_INT64] = nullptr,
        [TYPE_BOOL] = new CastOperator<TYPE_INT64, TYPE_BOOL>,
        [TYPE_FLOAT] = new CastOperator<TYPE_INT64, TYPE_FLOAT>,
        [TYPE_DOUBLE] = new CastOperator<TYPE_INT64, TYPE_DOUBLE>,
        [TYPE_DECIMAL] = nullptr,
        [TYPE_STRING] = new CastOperator<TYPE_INT64, TYPE_STRING>,
    },
    [TYPE_BOOL] = {
        [TYPE_NULL] = nullptr,
        [TYPE_INT32] = new CastOperator<TYPE_BOOL, TYPE_INT32>,
        [TYPE_INT64] = new CastOperator<TYPE_BOOL, TYPE_INT64>,
        [TYPE_BOOL] = nullptr,
        [TYPE_FLOAT] = new CastOperator<TYPE_BOOL, TYPE_FLOAT>,
        [TYPE_DOUBLE] = new CastOperator<TYPE_BOOL, TYPE_DOUBLE>,
        [TYPE_DECIMAL] = nullptr,
        [TYPE_STRING] = nullptr,
    },
    [TYPE_FLOAT] = {
        [TYPE_NULL] = nullptr,
        [TYPE_INT32] = new CastOperator<TYPE_FLOAT, TYPE_INT32>,
        [TYPE_INT64] = new CastOperator<TYPE_FLOAT, TYPE_INT64>,
        [TYPE_BOOL] = new CastOperator<TYPE_FLOAT, TYPE_BOOL>,
        [TYPE_FLOAT] = nullptr,
        [TYPE_DOUBLE] = new CastOperator<TYPE_FLOAT, TYPE_DOUBLE>,
        [TYPE_DECIMAL] = nullptr,
        [TYPE_STRING] = new CastOperator<TYPE_FLOAT, TYPE_STRING>,
    },
    [TYPE_DOUBLE] = {
        [TYPE_NULL] = nullptr,
        [TYPE_INT32] = new CastOperator<TYPE_DOUBLE, TYPE_INT32>,
        [TYPE_INT64] = new CastOperator<TYPE_DOUBLE, TYPE_INT64>,
        [TYPE_BOOL] = new CastOperator<TYPE_DOUBLE, TYPE_BOOL>,
        [TYPE_FLOAT] = new CastOperator<TYPE_DOUBLE, TYPE_FLOAT>,
        [TYPE_DOUBLE] = nullptr,
        [TYPE_DECIMAL] = nullptr,
        [TYPE_STRING] = new CastOperator<TYPE_DOUBLE, TYPE_STRING>,
    },
    [TYPE_DECIMAL] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    [TYPE_STRING] = {
        [TYPE_NULL] = nullptr,
        [TYPE_INT32] = new CastOperator<TYPE_STRING, TYPE_INT32>,
        [TYPE_INT64] = new CastOperator<TYPE_STRING, TYPE_INT64>,
        [TYPE_BOOL] = new CastOperator<TYPE_STRING, TYPE_BOOL>,
        [TYPE_FLOAT] = new CastOperator<TYPE_STRING, TYPE_FLOAT>,
        [TYPE_DOUBLE] = new CastOperator<TYPE_STRING, TYPE_DOUBLE>,
        [TYPE_DECIMAL] = nullptr,
        [TYPE_STRING] = nullptr,
    },
};
/* clang-format on */

const Operator *const OP_POS[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new UnaryArithmeticOperator<TYPE_INT32, calc::Pos>,
    [TYPE_INT64] = new UnaryArithmeticOperator<TYPE_INT64, calc::Pos>,
    [TYPE_BOOL] = nullptr,
    [TYPE_FLOAT] = new UnaryArithmeticOperator<TYPE_FLOAT, calc::Pos>,
    [TYPE_DOUBLE] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Pos>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = nullptr,
};

const Operator *const OP_NEG[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new UnaryArithmeticOperator<TYPE_INT32, calc::Neg>,
    [TYPE_INT64] = new UnaryArithmeticOperator<TYPE_INT64, calc::Neg>,
    [TYPE_BOOL] = nullptr,
    [TYPE_FLOAT] = new UnaryArithmeticOperator<TYPE_FLOAT, calc::Neg>,
    [TYPE_DOUBLE] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Neg>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = nullptr,
};

const Operator *const OP_ADD[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryArithmeticOperator<TYPE_INT32, calc::Add>,
    [TYPE_INT64] = new BinaryArithmeticOperator<TYPE_INT64, calc::Add>,
    [TYPE_BOOL] = nullptr,
    [TYPE_FLOAT] = new BinaryArithmeticOperator<TYPE_FLOAT, calc::Add>,
    [TYPE_DOUBLE] = new BinaryArithmeticOperator<TYPE_DOUBLE, calc::Add>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryArithmeticOperator<TYPE_STRING, calc::Add>,
};

const Operator *const OP_SUB[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryArithmeticOperator<TYPE_INT32, calc::Sub>,
    [TYPE_INT64] = new BinaryArithmeticOperator<TYPE_INT64, calc::Sub>,
    [TYPE_BOOL] = nullptr,
    [TYPE_FLOAT] = new BinaryArithmeticOperator<TYPE_FLOAT, calc::Sub>,
    [TYPE_DOUBLE] = new BinaryArithmeticOperator<TYPE_DOUBLE, calc::Sub>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryArithmeticOperator<TYPE_STRING, calc::Add>,
};

const Operator *const OP_MUL[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryArithmeticOperator<TYPE_INT32, calc::Mul>,
    [TYPE_INT64] = new BinaryArithmeticOperator<TYPE_INT64, calc::Mul>,
    [TYPE_BOOL] = nullptr,
    [TYPE_FLOAT] = new BinaryArithmeticOperator<TYPE_FLOAT, calc::Mul>,
    [TYPE_DOUBLE] = new BinaryArithmeticOperator<TYPE_DOUBLE, calc::Mul>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = nullptr,
};

const Operator *const OP_DIV[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryArithmeticOperator<TYPE_INT32, calc::Div>,
    [TYPE_INT64] = new BinaryArithmeticOperator<TYPE_INT64, calc::Div>,
    [TYPE_BOOL] = nullptr,
    [TYPE_FLOAT] = new BinaryArithmeticOperator<TYPE_FLOAT, calc::Div>,
    [TYPE_DOUBLE] = new BinaryArithmeticOperator<TYPE_DOUBLE, calc::Div>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = nullptr,
};

const Operator *const OP_EQ[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryRelationOperator<TYPE_INT32, calc::Eq>,
    [TYPE_INT64] = new BinaryRelationOperator<TYPE_INT64, calc::Eq>,
    [TYPE_BOOL] = new BinaryRelationOperator<TYPE_BOOL, calc::Eq>,
    [TYPE_FLOAT] = new BinaryRelationOperator<TYPE_FLOAT, calc::Eq>,
    [TYPE_DOUBLE] = new BinaryRelationOperator<TYPE_DOUBLE, calc::Eq>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryRelationOperator<TYPE_STRING, calc::Eq>,
};

const Operator *const OP_NE[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryRelationOperator<TYPE_INT32, calc::Ne>,
    [TYPE_INT64] = new BinaryRelationOperator<TYPE_INT64, calc::Ne>,
    [TYPE_BOOL] = new BinaryRelationOperator<TYPE_BOOL, calc::Ne>,
    [TYPE_FLOAT] = new BinaryRelationOperator<TYPE_FLOAT, calc::Ne>,
    [TYPE_DOUBLE] = new BinaryRelationOperator<TYPE_DOUBLE, calc::Ne>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryRelationOperator<TYPE_STRING, calc::Ne>,
};

const Operator *const OP_GT[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryRelationOperator<TYPE_INT32, calc::Gt>,
    [TYPE_INT64] = new BinaryRelationOperator<TYPE_INT64, calc::Gt>,
    [TYPE_BOOL] = new BinaryRelationOperator<TYPE_BOOL, calc::Gt>,
    [TYPE_FLOAT] = new BinaryRelationOperator<TYPE_FLOAT, calc::Gt>,
    [TYPE_DOUBLE] = new BinaryRelationOperator<TYPE_DOUBLE, calc::Gt>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryRelationOperator<TYPE_STRING, calc::Gt>,
};

const Operator *const OP_GE[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryRelationOperator<TYPE_INT32, calc::Ge>,
    [TYPE_INT64] = new BinaryRelationOperator<TYPE_INT64, calc::Ge>,
    [TYPE_BOOL] = new BinaryRelationOperator<TYPE_BOOL, calc::Ge>,
    [TYPE_FLOAT] = new BinaryRelationOperator<TYPE_FLOAT, calc::Ge>,
    [TYPE_DOUBLE] = new BinaryRelationOperator<TYPE_DOUBLE, calc::Ge>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryRelationOperator<TYPE_STRING, calc::Ge>,
};

const Operator *const OP_LT[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryRelationOperator<TYPE_INT32, calc::Lt>,
    [TYPE_INT64] = new BinaryRelationOperator<TYPE_INT64, calc::Lt>,
    [TYPE_BOOL] = new BinaryRelationOperator<TYPE_BOOL, calc::Lt>,
    [TYPE_FLOAT] = new BinaryRelationOperator<TYPE_FLOAT, calc::Lt>,
    [TYPE_DOUBLE] = new BinaryRelationOperator<TYPE_DOUBLE, calc::Lt>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryRelationOperator<TYPE_STRING, calc::Lt>,
};

const Operator *const OP_LE[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new BinaryRelationOperator<TYPE_INT32, calc::Le>,
    [TYPE_INT64] = new BinaryRelationOperator<TYPE_INT64, calc::Le>,
    [TYPE_BOOL] = new BinaryRelationOperator<TYPE_BOOL, calc::Le>,
    [TYPE_FLOAT] = new BinaryRelationOperator<TYPE_FLOAT, calc::Le>,
    [TYPE_DOUBLE] = new BinaryRelationOperator<TYPE_DOUBLE, calc::Le>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new BinaryRelationOperator<TYPE_STRING, calc::Le>,
};

const Operator *const OP_IS_NULL[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new UnarySpecialOperator<TYPE_INT32, calc::IsNull>,
    [TYPE_INT64] = new UnarySpecialOperator<TYPE_INT64, calc::IsNull>,
    [TYPE_BOOL] = new UnarySpecialOperator<TYPE_BOOL, calc::IsNull>,
    [TYPE_FLOAT] = new UnarySpecialOperator<TYPE_FLOAT, calc::IsNull>,
    [TYPE_DOUBLE] = new UnarySpecialOperator<TYPE_DOUBLE, calc::IsNull>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new UnarySpecialOperator<TYPE_STRING, calc::IsNull>,
};

const Operator *const OP_IS_TRUE[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new UnarySpecialOperator<TYPE_INT32, calc::IsTrue>,
    [TYPE_INT64] = new UnarySpecialOperator<TYPE_INT64, calc::IsTrue>,
    [TYPE_BOOL] = new UnarySpecialOperator<TYPE_BOOL, calc::IsTrue>,
    [TYPE_FLOAT] = new UnarySpecialOperator<TYPE_FLOAT, calc::IsTrue>,
    [TYPE_DOUBLE] = new UnarySpecialOperator<TYPE_DOUBLE, calc::IsTrue>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new UnarySpecialOperator<TYPE_STRING, calc::IsTrue>,
};

const Operator *const OP_IS_FALSE[] = {
    [TYPE_NULL] = nullptr,
    [TYPE_INT32] = new UnarySpecialOperator<TYPE_INT32, calc::IsFalse>,
    [TYPE_INT64] = new UnarySpecialOperator<TYPE_INT64, calc::IsFalse>,
    [TYPE_BOOL] = new UnarySpecialOperator<TYPE_BOOL, calc::IsFalse>,
    [TYPE_FLOAT] = new UnarySpecialOperator<TYPE_FLOAT, calc::IsFalse>,
    [TYPE_DOUBLE] = new UnarySpecialOperator<TYPE_DOUBLE, calc::IsFalse>,
    [TYPE_DECIMAL] = nullptr,
    [TYPE_STRING] = new UnarySpecialOperator<TYPE_STRING, calc::IsFalse>,
};

const Operator *const OP_NOT = new NotOperator();
const Operator *const OP_AND = new AndOperator();
const Operator *const OP_OR = new OrOperator();

const Operator *const OP_FUN[] = {
    [0x00] = nullptr,
    [0x01] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Ceil>,
    [0x02] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Floor>,
    [0x03] = nullptr,
    [0x04] = nullptr,
    [0x05] = nullptr,
    [0x06] = nullptr,
    [0x07] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Sin>,
    [0x08] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Cos>,
    [0x09] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Tan>,
    [0x0A] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Asin>,
    [0x0B] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Acos>,
    [0x0C] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Atan>,
    [0x0D] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Sinh>,
    [0x0E] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Cosh>,
    [0x0F] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Tanh>,
    [0x10] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Exp>,
    [0x11] = new UnaryArithmeticOperator<TYPE_DOUBLE, calc::Log>,
    [0x12] = nullptr,
    [0x13] = nullptr,
    [0x14] = nullptr,
    [0x15] = nullptr,
    [0x16] = nullptr,
    [0x17] = nullptr,
    [0x18] = nullptr,
    [0x19] = nullptr,
    [0x1A] = nullptr,
    [0x1B] = nullptr,
    [0x1C] = nullptr,
    [0x1D] = nullptr,
    [0x1E] = nullptr,
    [0x1F] = nullptr,
    [0x20] = nullptr,
    [0x21] = new BinaryOperatorV1<TYPE_STRING, TYPE_STRING, TYPE_STRING, calc::Concat>,
    [0x22] = new UnaryOperator<TYPE_STRING, TYPE_STRING, calc::Lower>,
    [0x23] = new UnaryOperator<TYPE_STRING, TYPE_STRING, calc::Upper>,
    [0x24] = new BinaryOperatorV1<TYPE_STRING, TYPE_STRING, TYPE_INT32, calc::Left>,
    [0x25] = new BinaryOperatorV1<TYPE_STRING, TYPE_STRING, TYPE_INT32, calc::Right>,
    [0x26] = new UnaryOperator<TYPE_STRING, TYPE_STRING, calc::Trim>,
    [0x27] = nullptr,
    [0x28] = new UnaryOperator<TYPE_STRING, TYPE_STRING, calc::LTrim>,
    [0x29] = nullptr,
    [0x2A] = new UnaryOperator<TYPE_STRING, TYPE_STRING, calc::RTrim>,
    [0x2B] = nullptr,
    [0x2C] = nullptr,
    [0x2D] = nullptr,
    [0x2E] = nullptr,
    [0x2F] = nullptr,
    [0x30] = nullptr,
    [0x31] = nullptr,
    [0x32] = nullptr,
    [0x33] = nullptr,
    [0x34] = nullptr,
    [0x35] = nullptr,
};

} // namespace dingodb::expr
