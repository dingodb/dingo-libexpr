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

#include "operator_vector.h"

#include "codec.h"
#include "operators.h"
#include "types.h"

#define NULL_PREFIX  0x00
#define NULL_INT32   (NULL_PREFIX | TYPE_INT32)
#define NULL_INT64   (NULL_PREFIX | TYPE_INT64)
#define NULL_BOOL    (NULL_PREFIX | TYPE_BOOL)
#define NULL_FLOAT   (NULL_PREFIX | TYPE_FLOAT)
#define NULL_DOUBLE  (NULL_PREFIX | TYPE_DOUBLE)
#define NULL_DECIMAL (NULL_PREFIX | TYPE_DECIMAL)
#define NULL_STRING  (NULL_PREFIX | TYPE_STRING)

#define CONST_PREFIX  0x10
#define CONST_INT32   (CONST_PREFIX | TYPE_INT32)
#define CONST_INT64   (CONST_PREFIX | TYPE_INT64)
#define CONST_BOOL    (CONST_PREFIX | TYPE_BOOL)
#define CONST_FLOAT   (CONST_PREFIX | TYPE_FLOAT)
#define CONST_DOUBLE  (CONST_PREFIX | TYPE_DOUBLE)
#define CONST_DECIMAL (CONST_PREFIX | TYPE_DECIMAL)
#define CONST_STRING  (CONST_PREFIX | TYPE_STRING)

#define CONST_N_PREFIX 0x20
#define CONST_N_INT32  (CONST_N_PREFIX | TYPE_INT32)
#define CONST_N_INT64  (CONST_N_PREFIX | TYPE_INT64)
#define CONST_N_BOOL   (CONST_N_PREFIX | TYPE_BOOL)

#define VAR_I_PREFIX  0x30
#define VAR_I_INT32   (VAR_I_PREFIX | TYPE_INT32)
#define VAR_I_INT64   (VAR_I_PREFIX | TYPE_INT64)
#define VAR_I_BOOL    (VAR_I_PREFIX | TYPE_BOOL)
#define VAR_I_FLOAT   (VAR_I_PREFIX | TYPE_FLOAT)
#define VAR_I_DOUBLE  (VAR_I_PREFIX | TYPE_DOUBLE)
#define VAR_I_DECIMAL (VAR_I_PREFIX | TYPE_DECIMAL)
#define VAR_I_STRING  (VAR_I_PREFIX | TYPE_STRING)

#define POS 0x81
#define NEG 0x82
#define ADD 0x83
#define SUB 0x84
#define MUL 0x85
#define DIV 0x86
#define MOD 0x87

#define EQ 0x91
#define GE 0x92
#define GT 0x93
#define LE 0x94
#define LT 0x95
#define NE 0x96

#define IS_NULL  0xA1
#define IS_TRUE  0xA2
#define IS_FALSE 0xA3

#define NOT 0x51
#define AND 0x52
#define OR  0x53

#define CAST 0xF0
#define FUN  0xF1

using namespace dingodb::expr;

void OperatorVector::Decode(const Byte code[], size_t len)
{
    Release();
    bool successful = true;
    const Byte *b;
    for (const Byte *p = code; successful && p < code + len;) {
        b = p;
        switch (*p) {
        case NULL_INT32:
            ++p;
            Add(OP_NULL[TYPE_INT32]);
            break;
        case NULL_INT64:
            ++p;
            Add(OP_NULL[TYPE_INT64]);
            break;
        case NULL_BOOL:
            ++p;
            Add(OP_NULL[TYPE_BOOL]);
            break;
        case NULL_FLOAT:
            ++p;
            Add(OP_NULL[TYPE_FLOAT]);
            break;
        case NULL_DOUBLE:
            ++p;
            Add(OP_NULL[TYPE_DOUBLE]);
            break;
        case NULL_STRING:
            ++p;
            Add(OP_NULL[TYPE_STRING]);
            break;
        case CONST_INT32: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new ConstOperator<TYPE_INT32>(v));
            break;
        }
        case CONST_INT64: {
            ++p;
            int64_t v;
            p = DecodeVarint(v, p);
            AddRelease(new ConstOperator<TYPE_INT64>(v));
            break;
        }
        case CONST_BOOL:
            ++p;
            Add(OP_CONST_TRUE);
            break;
        case CONST_FLOAT: {
            ++p;
            float v;
            p = DecodeFloat(v, p);
            AddRelease(new ConstOperator<TYPE_FLOAT>(v));
            break;
        }
        case CONST_DOUBLE: {
            ++p;
            double v;
            p = DecodeDouble(v, p);
            AddRelease(new ConstOperator<TYPE_DOUBLE>(v));
            break;
        }
        case CONST_DECIMAL: {
            ++p;
            // TODO
            break;
        }
        case CONST_STRING: {
            ++p;
            String v;
            p = DecodeString(v, p);
            AddRelease(new ConstOperator<TYPE_STRING>(v));
            break;
        }
        case CONST_N_INT32: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new ConstOperator<TYPE_INT32>(-v));
            break;
        }
        case CONST_N_INT64: {
            ++p;
            int64_t v;
            p = DecodeVarint(v, p);
            AddRelease(new ConstOperator<TYPE_INT64>(-v));
            break;
        }
        case CONST_N_BOOL:
            ++p;
            Add(OP_CONST_FALSE);
            break;
        case VAR_I_INT32: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new IndexedVarOperator<TYPE_INT32>(v));
            break;
        }
        case VAR_I_INT64: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new IndexedVarOperator<TYPE_INT64>(v));
            break;
        }
        case VAR_I_BOOL: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new IndexedVarOperator<TYPE_BOOL>(v));
            break;
        }
        case VAR_I_FLOAT: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new IndexedVarOperator<TYPE_FLOAT>(v));
            break;
        }
        case VAR_I_DOUBLE: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new IndexedVarOperator<TYPE_DOUBLE>(v));
            break;
        }
        case VAR_I_DECIMAL: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new IndexedVarOperator<TYPE_DECIMAL>(v));
            break;
        }
        case VAR_I_STRING: {
            ++p;
            int32_t v;
            p = DecodeVarint(v, p);
            AddRelease(new IndexedVarOperator<TYPE_STRING>(v));
            break;
        }
        case POS:
            ++p;
            successful = AddOperatorByType(OP_POS, *p);
            ++p;
            break;
        case NEG:
            ++p;
            successful = AddOperatorByType(OP_NEG, *p);
            ++p;
            break;
        case ADD:
            ++p;
            successful = AddOperatorByType(OP_ADD, *p);
            ++p;
            break;
        case SUB:
            ++p;
            successful = AddOperatorByType(OP_SUB, *p);
            ++p;
            break;
        case MUL:
            ++p;
            successful = AddOperatorByType(OP_MUL, *p);
            ++p;
            break;
        case DIV:
            ++p;
            successful = AddOperatorByType(OP_DIV, *p);
            ++p;
            break;
        case EQ:
            ++p;
            successful = AddOperatorByType(OP_EQ, *p);
            ++p;
            break;
        case GE:
            ++p;
            successful = AddOperatorByType(OP_GE, *p);
            ++p;
            break;
        case GT:
            ++p;
            successful = AddOperatorByType(OP_GT, *p);
            ++p;
            break;
        case LE:
            ++p;
            successful = AddOperatorByType(OP_LE, *p);
            ++p;
            break;
        case LT:
            ++p;
            successful = AddOperatorByType(OP_LT, *p);
            ++p;
            break;
        case NE:
            ++p;
            successful = AddOperatorByType(OP_NE, *p);
            ++p;
            break;
        case IS_NULL:
            ++p;
            successful = AddOperatorByType(OP_IS_NULL, *p);
            ++p;
            break;
        case IS_TRUE:
            ++p;
            successful = AddOperatorByType(OP_IS_TRUE, *p);
            ++p;
            break;
        case IS_FALSE:
            ++p;
            successful = AddOperatorByType(OP_IS_FALSE, *p);
            ++p;
            break;
        case NOT:
            Add(OP_NOT);
            ++p;
            break;
        case AND:
            Add(OP_AND);
            ++p;
            break;
        case OR:
            Add(OP_OR);
            ++p;
            break;
        case CAST:
            ++p;
            successful = AddCastOperator(*p);
            ++p;
            break;
        case FUN:
            ++p;
            successful = AddFunOperator(*p);
            ++p;
            break;
        default:
            successful = false;
            break;
        }
    }
    if (!successful) {
        throw std::runtime_error("Unknown instruction, bytes = " + ConvertBytesToHex(b, len - (b - code)));
    }
}

std::string OperatorVector::ConvertBytesToHex(const Byte *data, size_t len)
{
    char *buf = new char[len * 2 + 1];
    BytesToHex(buf, data, len);
    buf[len * 2] = '\0';
    std::string result(buf);
    delete[] buf;
    return result;
}

bool OperatorVector::AddOperatorByType(const Operator *const ops[], Byte type)
{
    auto op = ops[type];
    if (op != nullptr) {
        Add(op);
        return true;
    }
    return false;
}

bool OperatorVector::AddCastOperator(Byte b)
{
    Byte dst = (Byte)(b >> 4);
    Byte src = (Byte)(b & 0x0F);
    if (dst == src) {
        return true;
    }
    auto op = OP_CAST[dst][src];
    if (op != nullptr) {
        Add(op);
        return true;
    }
    return false;
}

bool OperatorVector::AddFunOperator(Byte b)
{
    auto op = OP_FUN[b];
    if (op != nullptr) {
        Add(op);
        return true;
    }
    return false;
}
