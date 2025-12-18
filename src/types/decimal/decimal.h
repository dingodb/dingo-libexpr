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

#ifndef DINGO_LIBEXPR_DECIMAL_H
#define DINGO_LIBEXPR_DECIMAL_H

#include <string>
#include "gmpxx.h"
#include "gmp.h"

namespace dingodb {
namespace types {

/**
 * Max precision for c++ decimal digits in expr.
 */
const int MAX_PRECISION = 128;

/**
 * Base for decimal digits.
 */
const int BASE = 10;

/**
 * Class for decimal type in expression.
 */
class Decimal {
 public:
  /**
   * default constructor.
   */
  Decimal(): v(MAX_PRECISION) {
  }

  /**
   * constructor.
   * @param var The value presented by string.
   */
  Decimal(const std::string & var);

  /**
   * constructor.
   * @param var The value presented by char array.
   */
  Decimal(const char * var);

  Decimal(const mpf_class & val) : v(val) {
  }

  /**
   * constructor.
   * @param var
   */
  Decimal(long var);

  /**
   * constructor.
   * @param var
   */
  Decimal(double var);
  /*
  Decimal(double var) {
    Decimal(std::to_string(var));
  }
  */

  /**
   * copy construcotr.
   * @param dec
   */
  Decimal(const Decimal& dec) {
    v = dec.getMpf();
  }

  /**
   * for copy construct operator.
   * @param dec
   * @return
   */
  Decimal& operator =(const Decimal& dec) {
    if(this == &dec) {
      return *this;
    }

    v = dec.getMpf();
  }

  /**
   * move constructor.
   * @param dec
   */
  Decimal(Decimal&& dec) {
    v = dec.moveMpf();
  }

  /**
   * constructor.
   * @param mp
   */
  Decimal(mpf_class& mp) : v(mp) {
  }

  /**
   * Get the precision.
   * @return
   */
  int getPrecision() {
    return v.get_prec();
  }

  /**
   * trans decimal to long.
   * @return
   */
  long toLong() {
    return v.get_si();
  }

  /**
   * trans decimal to unsigned long.
   * @return
   */
  unsigned long toULong() {
    return v.get_ui();
  }

  /**
   * trans decimal to int.
   * @return
   */
  int toInt() {
    return (int)v.get_si();
  }

  /**
   * trans decimal to unsigned int.
   * @return
   */
  unsigned int toUInt() {
    return (unsigned int)v.get_ui();
  }

  /**
   * trans decimal to double.
   * @return
   */
  double toDouble() {
    return v.get_d();
  }

  /**
   * trans decimal to string.
   * @return
   */
  std::string toString() const;

  /**
 * trans decimal to string with precision and scale.
 * @return
 */
  std::string toString(long precision, long scale) const;

  std::string toString1(long precision, long scale) const;

  /**
   * decimal + decimal.
   * @param dec
   * @return
   */
  Decimal operator+(const Decimal &dec) const {
      mpf_class ret = v + dec.getMpf();
      return std::move(Decimal(ret));
  }

  /**
   * decimal - decimal.
   * @param dec
   * @return
   */
  Decimal operator-(const Decimal &dec) const {
    mpf_class ret = v - dec.getMpf();
    return std::move(Decimal(ret));
  }

  /**
   * decimal * decimal.
   * @param dec
   * @return
   */
  Decimal operator*(const Decimal &dec) const {
    mpf_class ret = v * dec.getMpf();
    return std::move(Decimal(ret));
  }

  /**
   * decimal / decimal.
   * @param dec
   * @return
   */
  Decimal operator/(const Decimal &dec) const {
    mpf_class ret = v / dec.getMpf();
    return std::move(Decimal(ret));
  }

    /**
   * decimal == decimal.
   * @param dec
   * @return
   */
    bool operator==(const Decimal &dec) const {
      const mpf_class& mpf1 = mpf_class(toString());
      const mpf_class& mpf2 = mpf_class(dec.toString());

      int ret = cmp(mpf1, mpf2);
      if(ret == 0) {
        return true;
      }
      return false;
    }

    /**
     * decimal != decimal.
     * @param dec
     * @return
     */
    bool operator!=(const Decimal &dec) const {
        return !(*this == dec);
    }

    /**
     * decimal < decimal.
     * @param dec
     * @return
     */
    bool operator<(const Decimal &dec) const {
      const mpf_class& mpf1 = mpf_class(toString());
      const mpf_class& mpf2 = mpf_class(dec.toString());

      int ret = cmp(mpf1, mpf2);
      if(ret < 0) {
        return true;
      }
      return false;
    }

    /**
     * decimal <= decimal.
     * @param dec
     * @return
     */
    bool operator<=(const Decimal &dec) const {
      const mpf_class& mpf1 = mpf_class(toString());
      const mpf_class& mpf2 = mpf_class(dec.toString());

      int ret = cmp(mpf1, mpf2);
      if(ret <= 0) {
        return true;
      }
      return false;
    }

    /**
     * decimal > decimal.
     * @param dec
     * @return
     */
    bool operator>(const Decimal &dec) const {
      const mpf_class& mpf1 = mpf_class(toString());
      const mpf_class& mpf2 = mpf_class(dec.toString());

      int ret = cmp(mpf1, mpf2);
      if(ret > 0) {
        return true;
      }
      return false;
    }

    /**
     * decimal >= decimal.
     * @param dec
     * @return
     */
    bool operator>=(const Decimal &dec) const {
      const mpf_class& mpf1 = mpf_class(toString());
      const mpf_class& mpf2 = mpf_class(dec.toString());

      int ret = cmp(mpf1, mpf2);
      if(ret >= 0) {
        return true;
      }
      return false;
    }

    /**
     * -decimal.
     * @return
     */
    Decimal operator-() const;

    /**
     * +decimal.
     * @return
     */
  Decimal operator+() const;

    /**
     * abs.
     * @param dec
     */
    Decimal Abs() const {
      mpf_class ret = abs(v);
      return std::move(Decimal(ret));
    }

    /**
     * Print decimal as string.
     */
      void printDecimal() const;
 private:
  mpf_class v;

  /**
   * Get the internal mpf object.
   * @return
   */
  mpf_class getMpf() const {
    return v;
  }

  /**
   * move the internal mpf object.
   * @return
   */
  mpf_class moveMpf() {
    return std::move(v);
  }
};

}  // namespace types
}  // namespace dingodb

#endif  // DINGO_LIBEXPR_DECIMAL_H
