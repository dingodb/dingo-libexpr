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

#include <iostream>
#include <sstream>
#include "decimal.h"

namespace dingodb {
namespace types {
//#define DEBUG_DECIMAL

#ifdef DEBUG_DECIMAL
#define PRINT_DECIMAL printDecimal()
#define SHOW_EXCEPTION_INFO std::cout << "Decimal error: " << e.what() << std::endl;
#else
#define PRINT_DECIMAL
#define SHOW_EXCEPTION_INFO
#endif

Decimal::Decimal(const std::string & var) {
  try {
    int i = 0;
    for (; i < var.length(); i++) {
      if (var[i] != '.' && var[i] != 'e' && var[i] != 'E'
        && var[i] != '-' && var[i] != '+'
        && (var[i] < 0x30 || var[i] > 0x39)) {
        break;
        }
    }

    if (i == 0) {
      v = mpf_class("0", MAX_PRECISION, BASE);
    } else {
      v = mpf_class(var.substr(0, i), MAX_PRECISION, BASE);
    }
    PRINT_DECIMAL;
  } catch (const std::invalid_argument& e) {
    mp_exp_t exp;
    std::string error_info = "Exception in function: " + std::string(__func__) + ", " + std::string(e.what()) + ", " +
                            "Decimal value info - str: " + v.get_str(exp) + " expr: " + std::to_string(exp);
    throw std::runtime_error(error_info);
  }
}

Decimal::Decimal(const char * var) : v(mpf_class(var, MAX_PRECISION, BASE)) {
  PRINT_DECIMAL;
}

Decimal::Decimal(long var) : v(mpf_class(var)) {
  PRINT_DECIMAL;
}

Decimal::Decimal(double var) : v(mpf_class(var)) {
  PRINT_DECIMAL;
}

std::string Decimal::toString() const {
  mp_exp_t exp;
  std::string result;
  bool isNegative = false;

  std::string str = v.get_str(exp);

  if(str.empty() && exp == 0) {
    return std::string("0");
  }

  //has flag.
  int curPos = 0;

  if(str.at(0) == '-') {
    result.push_back(str.at(0));
    curPos++;
    isNegative = true;
  }

  //expr
  if(exp == 0){
    result.push_back('0');
    result.push_back('.');
    result.append(str, curPos);
  } else if (exp < 0) {
    result.push_back('0');
    result.push_back('.');
    for(int i = 0; i < 0 - exp; i++) {
      result.push_back('0');
    }

    result.append(str, curPos);
  } else {
    result.append(str, curPos, exp);
    int digitCount = 0;
    if (isNegative) {
      digitCount = str.length() - 1;
    } else {
      digitCount = str.length();
    }

    if (digitCount > exp) {
      result.push_back('.');
    } else {
      for (int i = 0; i < exp - digitCount; i++) {
        result.push_back('0');
      }
      return std::move(result);
    }

    result.append(str, curPos + exp);
  }

  return std::move(result);
}

std::string Decimal::toString(long precision, long scale) const {
  mp_exp_t exp;
  std::string str = v.get_str(exp);
  int srcDigitStartPos = 0;
  int srcDigitCount = str.length();
  bool isNegative = false;

  //fast path.
  if(str.empty() && exp == 0) {
    str = "0";
  }

  //deal with flag.
  if(str.at(0) == '-') {
    srcDigitStartPos++;
    srcDigitCount--;
    isNegative = true;
  }

  //cut digits.
  std::string str1;
  bool hasCutDigit = false;
  char cutDigit = 0;
  int targetMaxDigitCount = 0;
  if (exp == 0) {
    targetMaxDigitCount = scale;
  } else {
    targetMaxDigitCount = exp + scale;
  }

  //fast path.
  if (targetMaxDigitCount <= 0) {
    return std::move(std::string("0"));
  }

  str1.append(str.substr(srcDigitStartPos, targetMaxDigitCount));

  if (srcDigitCount > targetMaxDigitCount) {
    hasCutDigit = true;
    cutDigit = str.at(srcDigitStartPos + targetMaxDigitCount);
  }

  std::string interStr;
  mp_exp_t interExp;
  //calculate tail digit.
  if (hasCutDigit) {
    mpf_class tmp = (cutDigit >= 53) ? mpf_class(str1, MAX_PRECISION, BASE) + 1 : mpf_class(str1, MAX_PRECISION, BASE);
    interStr = tmp.get_str(interExp);
  } else {
    interStr = str1;
    interExp = str1.length();
  }

  //serial decimal for mysql format.
  //flag
  std::string result;
  int curResultPos = 0;
  if (isNegative) {
    result.push_back('-');
  }

  //int part.
  int intPost0 = 0;
  int consumed = 0;
  if (exp <= 0) {
    if (0 - exp >= scale) {
      //fast path.
      return std::move(std::string("0"));
    }

    result.push_back('0');
    result.push_back('.');
  } else {
    result.append(interStr.substr(0, exp));
    if (interStr.length() <= exp) {
      intPost0 = exp - interStr.length();
      consumed = interStr.length();
      for (int i = 0; i < intPost0; i++) {
        result.push_back('0');
      }
    } else if (interStr.length() >= exp) {
      intPost0 = exp;
      consumed = exp;
    }

    if (scale > 0) {
      result.push_back('.');
    }
  }

  //return immediatly as we not have scale part.
  if (scale ==0) {
    return std::move(result);
  }

  //scale part.
  if (exp == 0) {
    result.append(interStr);

    for (int i = 0; i < targetMaxDigitCount - interStr.length(); i++) {
      result.push_back('0');
    }
  } else if (exp < 0) {
    for (int i = 0; i < 0 - exp; i++) {
      result.push_back('0');
    }
    result.append(interStr.substr(0, targetMaxDigitCount));
  } else {
    int remainDigitCOunt = interStr.length() - consumed;
    if (remainDigitCOunt < scale) {
      result.append(interStr.substr(consumed));
      for (int i = 0; i < scale - remainDigitCOunt; i++) {
        result.push_back('0');
      }
    } else {
      result.append(interStr.substr(intPost0));
    }

  }

  return std::move(result);
}

std::string Decimal::toString1(long precision, long scale) const {
  mp_exp_t exp;

  std::string str = v.get_str(exp);

  //fast path.
  if(str.empty() && exp == 0) {
    return std::string("0");
  }

  std::string result;
  int curPos = 0;
  int targetPos = 0;
  bool isNegative = false;

  if(str.at(0) == '-') {
    isNegative = true;
    curPos++;
  }

  if(str.empty() && exp == 0) {
    return std::move(std::string("0"));
  }

  int resultLength = exp + scale;
  int sourceStringLength = str.length();
  if (isNegative) {
    resultLength += 1;
  }

  if ( sourceStringLength > resultLength ) {
    std::string str1 = str.substr(0, resultLength);
    mpf_class tmp;
    if (str.at(resultLength) >= 53) {
      if (isNegative) {
        tmp = mpf_class(str1, MAX_PRECISION, BASE) - 1;
      } else {
        tmp = mpf_class(str1, MAX_PRECISION, BASE) + 1;
      }
    } else {
      tmp = mpf_class(str1, MAX_PRECISION, BASE);
    }

    std::string toResult = tmp.get_str(exp);
    targetPos = 0;
    curPos = 0;

    if (isNegative) {
      result.push_back('-');
      targetPos++;
      curPos++;
    }

    result.append(toResult, curPos, exp - scale);
    curPos += exp - scale;
    targetPos += exp - scale;

    int lengthWithoutFlag = toResult.length();
    if (isNegative) {
      lengthWithoutFlag -= 1;
    }

    if (exp - (long)result.length() > 0) {
      result.push_back('.');
      result.append(toResult, curPos);

      for (int i = 0; i < exp - lengthWithoutFlag ; i++) {
        result.push_back('0');
      }
    }
    return std::move(result);
  } else {
    std::string str1;

    if (isNegative) {
      str1.push_back('-');
      targetPos++;
    }

    str1.append(str, curPos, exp);
    targetPos += exp;
    curPos += exp;

    int remainingLength  = 0;
    int digitCount = 0;
    if (isNegative) {
      digitCount = str.length() - 1;
    } else {
      digitCount = str.length();
    }

    remainingLength = digitCount > exp ? digitCount - exp : 0;

    if (remainingLength == 0) {
      int loop = 0;
      if (isNegative) {
        loop = exp - str.length() + 1;
      } else {
        loop = exp - str.length();
      }

      for (int i = 0; i < loop; i++) {
        str1.push_back('0');
      }
    } else {
      if (exp == 0 && digitCount > 0) {
        str1.push_back('0');
        targetPos++;
      }
    }

    str1.push_back('.');
    targetPos++;

    if (remainingLength != 0) {
      str1.append(str, curPos, remainingLength);
    }

    for (int i = 0; i < scale - remainingLength; i++) {
      str1.push_back('0');
    }

    return std::move(str1);
  }
}

Decimal Decimal::operator-() const {
  mp_exp_t exp;
  std::string result;
  std::string str = v.get_str(exp);

  if(str.empty()) {
    result.push_back('0');
    return result;
  }

  //has flag.
  int curPos = 0;

  if(str.at(0) != '-') {
    //The origin number must be pos, so we add '-' here for '-' operator.
    result.push_back('-');
  }

  //expr
  if(exp == 0){
    result.push_back('0');
    result.push_back('.');
    result.append(str, curPos);
  }
  else {
    std::ostringstream oss;
    oss << 0 - v;
    result = oss.str();
  }

  return std::move(Decimal(result));
}

Decimal Decimal::operator+() const {
  mp_exp_t exp;
  std::string result;
  std::string str = v.get_str(exp);

  if(str.empty()) {
    result.push_back('0');
    return result;
  }

  //has flag.
  int curPos = 0;

  if(str.at(0) == '-') {
    //The origin number must be pos, so we add '-' here for '-' operator.
    result.push_back('-');
  }

  //expr
  if(exp == 0){
    result.push_back('0');
    result.push_back('.');
    result.append(str, curPos);
  }
  else {
    result.append(str, curPos, exp);
    result.push_back('.');
    result.append(str, curPos + exp);
  }

  return std::move(Decimal(result));
}

void Decimal::printDecimal() const {
    mp_exp_t exp;
    std::cout << "Decimal value info - str: " << v.get_str(exp) << " expr: " << exp << std::endl;
}

}  // namespace types
}  // namespace dingodb