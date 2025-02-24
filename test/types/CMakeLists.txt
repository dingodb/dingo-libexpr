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

include_directories(${CMAKE_SOURCE_DIR}/src/types/decimal)
include_directories(${GMP_BINARY_PATH}/install/include)

add_executable(test_type_decimal test_type_decimal.cc)
target_link_libraries(test_type_decimal GTest::gtest_main ${TYPES_LIB_NAME} ${GMPXX_LIB_NAME} ${GMP_LIB_NAME})
add_dependencies(test_type_decimal gmp)

gtest_discover_tests(test_type_decimal)
