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

#ifndef _CALC_STRING_FUN_H_
#define _CALC_STRING_FUN_H_

#include "../operand.h"
#include "../types.h"

namespace dingodb::expr::calc
{

wrap<String> Concat(const wrap<String> &v0, const wrap<String> &v1);

String Lower(String v);

String Upper(String v);

wrap<String> Left(const wrap<String> &v0, const wrap<int32_t> &v1);

wrap<String> Right(const wrap<String> &v0, const wrap<int32_t> &v1);

String Trim(String v);

String LTrim(String v);

String RTrim(String v);

} // namespace dingodb::expr::calc

#endif /* _CALC_STRING_FUN_H_ */
