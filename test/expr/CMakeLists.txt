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

include_directories(${CMAKE_SOURCE_DIR}/src/expr)
include_directories(${DECIMAL_TYPE_SOURCE_PATH})
include_directories(${GMP_BINARY_PATH}/install/include)

add_subdirectory(calc)

add_executable(test_expr test_expr.cc)
target_link_libraries(test_expr GTest::gtest_main ${EXPR_LIB_NAME})
gtest_discover_tests(test_expr)

add_executable(test_expr_decimal test_expr_decimal.cc)
target_link_libraries(test_expr_decimal GTest::gtest_main ${EXPR_LIB_NAME})
gtest_discover_tests(test_expr_decimal)

add_executable(test_types test_types.cc)
target_link_libraries(test_types GTest::gtest_main ${EXPR_LIB_NAME})
gtest_discover_tests(test_types)
