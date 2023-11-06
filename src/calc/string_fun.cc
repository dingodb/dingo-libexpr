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

#include <algorithm>

#include "string_fun.h"

namespace dingodb::expr::calc
{

Wrap<String> Concat(const Wrap<String> &v0, const Wrap<String> &v1)
{
    if (v0.has_value()) {
        if (v1.has_value()) {
            return Wrap<String>(std::make_shared<std::string>(**v0 + **v1));
        }
        return v0;
    }
    return v1;
}

String Lower(String v)
{
    std::string str(v->length(), '\0');
    std::transform(v->cbegin(), v->cend(), str.begin(), [](unsigned char ch) { return std::tolower(ch); });
    return std::make_shared<std::string>(str);
}

String Upper(String v)
{
    std::string str(v->length(), '\0');
    std::transform(v->cbegin(), v->cend(), str.begin(), [](unsigned char ch) { return std::toupper(ch); });
    return std::make_shared<std::string>(str);
}

Wrap<String> Left(const Wrap<String> &v0, const Wrap<int32_t> &v1)
{
    if (v1.has_value()) {
        if (v0.has_value() && *v1 > 0) {
            auto len = (*v0)->length();
            if (*v1 < len) {
                return Wrap<String>(std::make_shared<std::string>((*v0)->substr(0, *v1)));
            }
            return v0;
        }
        return Wrap<String>(std::make_shared<std::string>());
    }
    return Wrap<String>();
}

Wrap<String> Right(const Wrap<String> &v0, const Wrap<int32_t> &v1)
{
    if (v1.has_value()) {
        if (v0.has_value() && *v1 > 0) {
            auto len = (*v0)->length();
            if (*v1 < len) {
                return Wrap<String>(std::make_shared<std::string>((*v0)->substr(len - *v1)));
            }
            return v0;
        }
        return Wrap<String>(std::make_shared<std::string>());
    }
    return Wrap<String>();
}

static bool IsSpace(unsigned char ch)
{
    return std::isspace(ch);
}

String Trim(String v)
{
    auto s = std::find_if_not(v->cbegin(), v->cend(), IsSpace);
    auto e = std::find_if_not(v->crbegin(), v->crend(), IsSpace);
    return std::make_shared<std::string>(v->substr(s - v->cbegin(), (v->crend() - e) - (s - v->cbegin())));
}

String LTrim(String v)
{
    auto s = std::find_if_not(v->cbegin(), v->cend(), IsSpace);
    return std::make_shared<std::string>(v->substr(s - v->cbegin()));
}

String RTrim(String v)
{
    auto e = std::find_if_not(v->crbegin(), v->crend(), IsSpace);
    return std::make_shared<std::string>(v->substr(0, v->crend() - e));
}

} // namespace dingodb::expr::calc
