# Copyright (c) 2023 dingodb.com, Inc. All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(LIB_NAME expr)

set(SRCS
    calc/arithmetic.cc
    calc/casting.cc
    calc/mathematic.cc
    calc/relational.cc
    calc/special.cc
    calc/string_fun.cc
    codec.cc
    operator.cc
    operator_vector.cc
    operators.cc
    types.cc
)

add_library(${LIB_NAME} STATIC ${SRCS})
