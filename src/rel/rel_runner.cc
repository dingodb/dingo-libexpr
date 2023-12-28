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

#include "expr/exception.h"
#include "expr/runner.h"
#include "expr/utils.h"
#include "op/filter_op.h"
#include "op/project_op.h"
#include "op/tandem_op.h"
#include "op/ungrouped_agg_op.h"

#define FILTER_OP           0x71
#define PROJECT_OP          0x72
#define GROUPED_AGGREGATE   0x73
#define UNGROUPED_AGGREGATE 0x74

#define AGG_COUNT_ALL 0x10
#define AGG_COUNT     0x10
#define AGG_SUM       0x20
#define AGG_SUM0      0x30
#define AGG_MAX       0x40
#define AGG_MIN       0x50

namespace dingodb::rel {

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
      break;
    }
    case UNGROUPED_AGGREGATE: {
      ++p;
      auto *aggs = new std::vector<const op::Agg *>();
      p = DecodeAggList(*aggs, p, code + len - p);
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

expr::Tuple *RelRunner::Put(expr::Tuple *tuple) const {
  return m_op->Put(tuple);
}

template <>
const expr::Byte *RelRunner::DecodeAgg<op::CountAllAgg>(std::vector<const op::Agg *> &aggs, const expr::Byte *code) {
  const expr::Byte *p = code;
  ++p;
  auto *agg = new op::CountAllAgg();
  aggs.push_back(agg);
  return p;
}

const expr::Byte *RelRunner::DecodeAggList(std::vector<const op::Agg *> &aggs, const expr::Byte *code, size_t len) {
  bool successful = true;
  const expr::Byte *p = code;
  const expr::Byte *b;
  int32_t count;
  p = expr::DecodeVarint(count, p);
  for (int32_t i = 0; i < count; ++i) {
    b = p;
    switch (*p) {
    case AGG_COUNT_ALL:
      p = DecodeAgg<op::CountAllAgg>(aggs, p);
      break;
    case AGG_COUNT | TYPE_INT32:
      p = DecodeAgg<op::CountAgg<int32_t>>(aggs, p);
      break;
    case AGG_COUNT | TYPE_INT64:
      p = DecodeAgg<op::CountAgg<int64_t>>(aggs, p);
      break;
    case AGG_COUNT | TYPE_BOOL:
      p = DecodeAgg<op::CountAgg<bool>>(aggs, p);
      break;
    case AGG_COUNT | TYPE_FLOAT:
      p = DecodeAgg<op::CountAgg<float>>(aggs, p);
      break;
    case AGG_COUNT | TYPE_DOUBLE:
      p = DecodeAgg<op::CountAgg<double>>(aggs, p);
      break;
    case AGG_COUNT | TYPE_STRING:
      p = DecodeAgg<op::CountAgg<expr::String>>(aggs, p);
      break;
    case AGG_SUM | TYPE_INT32:
      p = DecodeAgg<op::SumAgg<int32_t>>(aggs, p);
      break;
    case AGG_SUM | TYPE_INT64:
      p = DecodeAgg<op::SumAgg<int64_t>>(aggs, p);
      break;
    case AGG_SUM | TYPE_FLOAT:
      p = DecodeAgg<op::SumAgg<float>>(aggs, p);
      break;
    case AGG_SUM | TYPE_DOUBLE:
      p = DecodeAgg<op::SumAgg<double>>(aggs, p);
      break;
    case AGG_SUM0 | TYPE_INT32:
      p = DecodeAgg<op::Sum0Agg<int32_t>>(aggs, p);
      break;
    case AGG_SUM0 | TYPE_INT64:
      p = DecodeAgg<op::Sum0Agg<int64_t>>(aggs, p);
      break;
    case AGG_SUM0 | TYPE_FLOAT:
      p = DecodeAgg<op::Sum0Agg<float>>(aggs, p);
      break;
    case AGG_SUM0 | TYPE_DOUBLE:
      p = DecodeAgg<op::Sum0Agg<double>>(aggs, p);
      break;
    case AGG_MAX | TYPE_INT32:
      p = DecodeAgg<op::MaxAgg<int32_t>>(aggs, p);
      break;
    case AGG_MAX | TYPE_INT64:
      p = DecodeAgg<op::MaxAgg<int64_t>>(aggs, p);
      break;
    case AGG_MAX | TYPE_FLOAT:
      p = DecodeAgg<op::MaxAgg<float>>(aggs, p);
      break;
    case AGG_MAX | TYPE_DOUBLE:
      p = DecodeAgg<op::MaxAgg<double>>(aggs, p);
      break;
    case AGG_MAX | TYPE_STRING:
      p = DecodeAgg<op::MaxAgg<expr::String>>(aggs, p);
      break;
    case AGG_MIN | TYPE_INT32:
      p = DecodeAgg<op::MinAgg<int32_t>>(aggs, p);
      break;
    case AGG_MIN | TYPE_INT64:
      p = DecodeAgg<op::MinAgg<int64_t>>(aggs, p);
      break;
    case AGG_MIN | TYPE_FLOAT:
      p = DecodeAgg<op::MinAgg<float>>(aggs, p);
      break;
    case AGG_MIN | TYPE_DOUBLE:
      p = DecodeAgg<op::MinAgg<double>>(aggs, p);
      break;
    case AGG_MIN | TYPE_STRING:
      p = DecodeAgg<op::MinAgg<expr::String>>(aggs, p);
      break;
    default:
      successful = false;
      break;
    }
  }
  if (successful) {
    return p;
  }
  throw expr::UnknownCode(b, len - (b - code));
}

void RelRunner::AppendOp(RelOp *op) {
  if (m_op != nullptr) {
    m_op = new op::TandemOp(m_op, op);
  } else {
    m_op = op;
  }
}

}  // namespace dingodb::rel
