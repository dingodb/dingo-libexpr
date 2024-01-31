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

#include "string_fun.h"

#include <algorithm>

namespace dingodb::expr::calc {

String Concat(String v0, String v1) {
  if (!v0->empty()) {
    if (!v1->empty()) {
      return *v0 + *v1;
    }
    return v0;
  }
  return v1;
}

String Lower(String v) {
  std::string str(v->length(), '\0');
  std::transform(v->cbegin(), v->cend(), str.begin(), [](unsigned char ch) { return std::tolower(ch); });
  return str;
}

String Upper(String v) {
  std::string str(v->length(), '\0');
  std::transform(v->cbegin(), v->cend(), str.begin(), [](unsigned char ch) { return std::toupper(ch); });
  return str;
}

String Left(String v0, int32_t v1) {
  if (!v0->empty()) {
    if (v1 > 0) {
      auto len = v0->length();
      if (v1 < len) {
        return v0->substr(0, v1);
      }
      return v0;
    }
    return String();
  }
  return v0;
}

String Right(String v0, int32_t v1) {
  if (!v0->empty()) {
    if (v1 > 0) {
      auto len = v0->length();
      if (v1 < len) {
        return v0->substr(len - v1);
      }
      return v0;
    }
    return String();
  }
  return v0;
}

static bool IsSpace(unsigned char ch) {
  return std::isspace(ch);
}

String Trim(String v) {
  auto s = std::find_if_not(v->cbegin(), v->cend(), IsSpace);
  auto e = std::find_if_not(v->crbegin(), v->crend(), IsSpace);
  return v->substr(s - v->cbegin(), (v->crend() - e) - (s - v->cbegin()));
}

String LTrim(String v) {
  auto s = std::find_if_not(v->cbegin(), v->cend(), IsSpace);
  return v->substr(s - v->cbegin());
}

String RTrim(String v) {
  auto e = std::find_if_not(v->crbegin(), v->crend(), IsSpace);
  return v->substr(0, v->crend() - e);
}

String Substr(String v0, int32_t v1, int32_t v2) {
  int len = v0->length();
  if (v1 < 0) {
    v1 = 0;
  }
  if (v1 == 0) {
    if (v2 >= len) {
      return v0;
    } else {
      return v0->substr(v1, v2 - v1);
    }
  } else {
    if (v2 >= len) {
      return v0->substr(v1);
    } else {
      return v0->substr(v1, v2 - v1);
    }
  }
}

String Substr(String v0, int32_t v1) {
  int len = v0->length();
  if (v1 < 0) {
    v1 = 0;
  }
  if (v1 == 0) {
    return v0;
  } else {
    return v0->substr(v1);
  }
}

String Mid(String v0, int32_t v1, int32_t v2) {
  if (v2 > 0) {
    int len = v0->length();
    if (0 < v1 && v1 <= len) {
      v1 -= 1;
    } else if (-len <= v1 && v1 < 0) {
      v1 += len;
    } else {
      return String();
    }
    if (v1 + v2 >= len) {
      return v0->substr(v1);
    } else {
      return v0->substr(v1, v2);
    }
  }
  return String();
}

String Mid(String v0, int32_t v1) {
  int len = v0->length();
  if (0 < v1 && v1 <= len) {
    v1 -= 1;
  } else if (-len <= v1 && v1 < 0) {
    v1 += len;
  } else {
    return String();
  }
  return v0->substr(v1);
}

}  // namespace dingodb::expr::calc
