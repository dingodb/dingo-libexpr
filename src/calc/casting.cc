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

#include <cmath>

#include "casting.h"

namespace dingodb::expr::calc
{

template <> int32_t Cast(float v)
{
    return lround(v);
}

template <> int32_t Cast(double v)
{
    return lround(v);
}

template <> int32_t Cast(String v)
{
    return std::stoi(*v);
}

template <> int64_t Cast(float v)
{
    return llround(v);
}

template <> int64_t Cast(double v)
{
    return llround(v);
}

template <> int64_t Cast(String v)
{
    return std::stoll(*v);
}

template <> float Cast(String v)
{
    return std::stof(*v);
}

template <> double Cast(String v)
{
    return std::stod(*v);
}

template <> String Cast(int32_t v)
{
    return std::make_shared<std::string>(std::to_string(v));
}

template <> String Cast(int64_t v)
{
    return std::make_shared<std::string>(std::to_string(v));
}

template <> String Cast(bool v)
{
    return std::make_shared<std::string>(v ? "true" : "false");
}

template <> String Cast(float v)
{
    auto s = std::to_string(v);
    s.erase(s.find_last_not_of('0') + 1);
    return std::make_shared<std::string>(s);
}

template <> String Cast(double v)
{
    auto s = std::to_string(v);
    s.erase(s.find_last_not_of('0') + 1);
    return std::make_shared<std::string>(s);
}

} // namespace dingodb::expr::calc
