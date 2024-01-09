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

#include "rel_runner.h"

#include <cassert>

#include "../expr/exception.h"
#include "../expr/runner.h"
#include "op/filter_op.h"
#include "op/grouped_agg_op.h"
#include "op/project_op.h"
#include "op/tandem_op.h"
#include "op/ungrouped_agg_op.h"

namespace dingodb::rel {

static const expr::Byte FILTER_OP = 0x71;
static const expr::Byte PROJECT_OP = 0x72;
static const expr::Byte GROUPED_AGGREGATE = 0x73;
static const expr::Byte UNGROUPED_AGGREGATE = 0x74;

static const expr::Byte ARRAY_PREFIX = 0x60;
static const expr::Byte ARRAY_INT32 = ARRAY_PREFIX | expr::TYPE_INT32;

static const expr::Byte AGG_COUNT_ALL = 0x10;
static const expr::Byte AGG_COUNT = 0x10;
static const expr::Byte AGG_SUM = 0x20;
static const expr::Byte AGG_MAX = 0x30;
static const expr::Byte AGG_MIN = 0x40;

RelRunner::RelRunner() : m_op(nullptr) {
}

RelRunner::~RelRunner() {
  Release();
}

const expr::Byte *RelRunner::Decode(const expr::Byte *code, size_t len) {
  Release();
  bool successful = true;
  const expr::Byte *p = code;
  const expr::Byte *b;
  while (successful && p < code + len) {
    b = p;
    switch (*p) {
    case FILTER_OP: {
      ++p;
      auto *filter = new expr::Runner();
      p = filter->Decode(p, code + len - p);
      AppendOp(new op::FilterOp(filter));
      break;
    }
    case PROJECT_OP: {
      ++p;
      auto *projects = new expr::Runner();
      p = projects->Decode(p, code + len - p);
      AppendOp(new op::ProjectOp(projects));
      break;
    }
    case GROUPED_AGGREGATE: {
      ++p;
      assert(*p == ARRAY_INT32);
      ++p;
      int32_t *groupe_indices;
      size_t count;
      p = expr::DecodeArray(groupe_indices, count, p, code + len - p);
      std::vector<const op::Agg *> *aggs;
      p = expr::DecodeVector(aggs, p, code + len - p);
      AppendOp(new op::GroupedAggOp(groupe_indices, count, aggs));
      break;
    }
    case UNGROUPED_AGGREGATE: {
      ++p;
      std::vector<const op::Agg *> *aggs;
      p = expr::DecodeVector(aggs, p, code + len - p);
      AppendOp(new op::UngroupedAggOp(aggs));
      break;
    }
    default:
      successful = false;
      break;
    }
  }
  if (successful) {
    return p;
  }
  throw std::runtime_error("Unknown instruction, bytes = " + expr::HexOfBytes(b, len - (b - code)));
}

const expr::Tuple *RelRunner::Put(const expr::Tuple *tuple) const {
  return m_op->Put(tuple);
}

const expr::Tuple *RelRunner::Get() const {
  return m_op->Get();
}

void RelRunner::AppendOp(RelOp *op) {
  if (m_op != nullptr) {
    m_op = new op::TandemOp(m_op, op);
  } else {
    m_op = op;
  }
}

}  // namespace dingodb::rel

namespace dingodb::expr {

template <>
const Byte *DecodeAgg<rel::op::CountAllAgg>(const rel::op::Agg *&agg, const Byte *data) {
  const expr::Byte *p = data;
  ++p;
  agg = new rel::op::CountAllAgg();
  return p;
}

template <>
const Byte *DecodeValue<const rel::op::Agg *>(const rel::op::Agg *&value, const Byte *data) {
  const Byte *p = data;
  switch (*p) {
  case rel::AGG_COUNT_ALL:
    p = DecodeAgg<rel::op::CountAllAgg>(value, p);
    break;
  case rel::AGG_COUNT | TYPE_INT32:
    p = DecodeAgg<rel::op::CountAgg<int32_t>>(value, p);
    break;
  case rel::AGG_COUNT | TYPE_INT64:
    p = DecodeAgg<rel::op::CountAgg<int64_t>>(value, p);
    break;
  case rel::AGG_COUNT | TYPE_BOOL:
    p = DecodeAgg<rel::op::CountAgg<bool>>(value, p);
    break;
  case rel::AGG_COUNT | TYPE_FLOAT:
    p = DecodeAgg<rel::op::CountAgg<float>>(value, p);
    break;
  case rel::AGG_COUNT | TYPE_DOUBLE:
    p = DecodeAgg<rel::op::CountAgg<double>>(value, p);
    break;
  case rel::AGG_COUNT | TYPE_STRING:
    p = DecodeAgg<rel::op::CountAgg<expr::String>>(value, p);
    break;
  case rel::AGG_SUM | TYPE_INT32:
    p = DecodeAgg<rel::op::SumAgg<int32_t>>(value, p);
    break;
  case rel::AGG_SUM | TYPE_INT64:
    p = DecodeAgg<rel::op::SumAgg<int64_t>>(value, p);
    break;
  case rel::AGG_SUM | TYPE_FLOAT:
    p = DecodeAgg<rel::op::SumAgg<float>>(value, p);
    break;
  case rel::AGG_SUM | TYPE_DOUBLE:
    p = DecodeAgg<rel::op::SumAgg<double>>(value, p);
    break;
  case rel::AGG_MAX | TYPE_INT32:
    p = DecodeAgg<rel::op::MaxAgg<int32_t>>(value, p);
    break;
  case rel::AGG_MAX | TYPE_INT64:
    p = DecodeAgg<rel::op::MaxAgg<int64_t>>(value, p);
    break;
  case rel::AGG_MAX | TYPE_FLOAT:
    p = DecodeAgg<rel::op::MaxAgg<float>>(value, p);
    break;
  case rel::AGG_MAX | TYPE_DOUBLE:
    p = DecodeAgg<rel::op::MaxAgg<double>>(value, p);
    break;
  case rel::AGG_MAX | TYPE_STRING:
    p = DecodeAgg<rel::op::MaxAgg<expr::String>>(value, p);
    break;
  case rel::AGG_MIN | TYPE_INT32:
    p = DecodeAgg<rel::op::MinAgg<int32_t>>(value, p);
    break;
  case rel::AGG_MIN | TYPE_INT64:
    p = DecodeAgg<rel::op::MinAgg<int64_t>>(value, p);
    break;
  case rel::AGG_MIN | TYPE_FLOAT:
    p = DecodeAgg<rel::op::MinAgg<float>>(value, p);
    break;
  case rel::AGG_MIN | TYPE_DOUBLE:
    p = DecodeAgg<rel::op::MinAgg<double>>(value, p);
    break;
  case rel::AGG_MIN | TYPE_STRING:
    p = DecodeAgg<rel::op::MinAgg<expr::String>>(value, p);
    break;
  default:
    throw ExprError("Unknown aggregation type: " + HexOfBytes(data, 1));
    break;
  }
  return p;
}

}  // namespace dingodb::expr
