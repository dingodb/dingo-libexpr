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

#include <gtest/gtest.h>
#include "decimal.h"
#include "decimal_p.h"

using namespace dingodb::types;

TEST(TestTypeDecimal, DecimalTest) {
  Decimal v = Decimal("123.123");

  //Test function printDecimal.
  v.printDecimal();

  //Test decimal value 0.
  Decimal izqval0 = Decimal("0");
  std::string izqresult0 = izqval0.toString();
  ASSERT_EQ(izqresult0, "0");
  std::string izqresulta = izqval0.toString(10,2);
  ASSERT_EQ(izqresulta, "0.00");
  std::string izqresultb = izqval0.toString(10,0);
  ASSERT_EQ(izqresultb, "0");
  std::string izqresultc = izqval0.toString(10,4);
  ASSERT_EQ(izqresultc, "0.0000");

  Decimal izqval01 = Decimal("0.000");
  std::string izqresult01 = izqval01.toString();
  ASSERT_EQ(izqresult01, "0");
  std::string izqresulta1 = izqval01.toString(10,2);
  ASSERT_EQ(izqresulta1, "0.00");
  std::string izqresultb1 = izqval01.toString(10,0);
  ASSERT_EQ(izqresultb1, "0");
  std::string izqresultc1 = izqval01.toString(10,4);
  ASSERT_EQ(izqresultc1, "0.0000");

  Decimal izqval1 = Decimal("100.00");
  std::string izqresult1 = izqval1.toString();
  ASSERT_EQ(izqresult1, "100");
  std::string izqresult2 = izqval1.toString(10,2);
  ASSERT_EQ(izqresult2, "100.00");
  std::string izqresult21 = izqval1.toString(10,0);
  ASSERT_EQ(izqresult21, "100");

  // ---- Test multi 0. ----
  Decimal zqval1 = Decimal("0.00123");
  std::string zqresult1 = zqval1.toString();
  ASSERT_EQ(zqresult1, "0.00123");
  std::string zqresult2 = zqval1.toString(10,4);
  ASSERT_EQ(zqresult2, "0.0012");
  std::string zqresult21 = zqval1.toString(10,0);
  ASSERT_EQ(zqresult21, "0");

  Decimal zaqval1 = Decimal("0.0000000123");
  std::string zaqresult1 = zaqval1.toString();
  ASSERT_EQ(zaqresult1, "0.0000000123");
  std::string zaqresult2 = zaqval1.toString(10,4);
  ASSERT_EQ(zaqresult2, "0");
  std::string zaqresult21 = zqval1.toString(10,0);
  ASSERT_EQ(zaqresult21, "0");

  Decimal zbqval1 = Decimal("-0.0000000123");
  std::string zbqresult1 = zbqval1.toString();
  ASSERT_EQ(zbqresult1, "-0.0000000123");
  std::string zbqresult2 = zbqval1.toString(10,4);
  ASSERT_EQ(zbqresult2, "0");
  std::string zbqresult21 = zbqval1.toString(10,0);
  ASSERT_EQ(zbqresult21, "0");

  Decimal znqval1 = Decimal("0.0012300000000000");
  std::string znqresult1 = znqval1.toString();
  ASSERT_EQ(znqresult1, "0.00123");
  std::string znqresult2 = znqval1.toString(10,4);
  ASSERT_EQ(znqresult2, "0.0012");
  std::string znqresult21 = znqval1.toString(10,0);
  ASSERT_EQ(znqresult21, "0");

  Decimal zanqval1 = Decimal("-0.0012300000000000");
  std::string zanqresult1 = zanqval1.toString();
  ASSERT_EQ(zanqresult1, "-0.00123");
  std::string zanqresult2 = zanqval1.toString(10,4);
  ASSERT_EQ(zanqresult2, "-0.0012");
  std::string zanqresult21 = zanqval1.toString(10,0);
  ASSERT_EQ(zanqresult21, "0");

  Decimal zaanqval1 = Decimal("-0.0000000012300000000000");
  std::string zaanqresult1 = zaanqval1.toString();
  ASSERT_EQ(zaanqresult1, "-0.00000000123");
  std::string zaanqresult2 = zaanqval1.toString(10,4);
  ASSERT_EQ(zaanqresult2, "0");
  std::string zaanqresult21 = zaanqval1.toString(10,0);
  ASSERT_EQ(zaanqresult21, "0");

  // ---- Test 0. ----
  //scale is not 0.
  Decimal qval1 = Decimal("0");
  std::string qresult1 = qval1.toString();
  ASSERT_EQ(qresult1, "0");
  std::string qresult2 = qval1.toString(10,4);
  ASSERT_EQ(qresult2, "0.0000");

  //scale is 0.
  Decimal qqval1 = Decimal("0");
  std::string qqresult1 = qqval1.toString();
  ASSERT_EQ(qqresult1, "0");
  std::string qqresult2 = qqval1.toString(10,0);
  ASSERT_EQ(qqresult2, "0");

  //Test 0.xxx
  //scale in data < scale parameter.
  Decimal zval1 = Decimal("0.123");
  std::string zresult1 = zval1.toString();
  ASSERT_EQ(zresult1, "0.123");
  std::string zresult2 = zval1.toString(10,4);
  ASSERT_EQ(zresult2, "0.1230");

  //scale in data = scale parameter.
  Decimal zval11 = Decimal("0.1234");
  std::string zresult11 = zval11.toString();
  ASSERT_EQ(zresult11, "0.1234");
  std::string zresult22 = zval11.toString(10,4);
  ASSERT_EQ(zresult22, "0.1234");
  std::string zaresult2 = zval11.toString(10,0);
  ASSERT_EQ(zaresult2, "0");

  //scale in data > scale parameter. tail digit smaller than 5.
  Decimal zval12 = Decimal("0.123449");
  std::string zresult12 = zval12.toString();
  ASSERT_EQ(zresult12, "0.123449");
  std::string zresult23 = zval12.toString(10,4);
  ASSERT_EQ(zresult23, "0.1234");
  std::string zaresult23 = zval12.toString(10,0);
  ASSERT_EQ(zaresult23, "0");

  //scale in data > scale parameter. tail digit bigger than 5.
  Decimal zval13 = Decimal("0.12345");
  std::string zresult13 = zval13.toString();
  ASSERT_EQ(zresult13, "0.12345");
  std::string zresult24 = zval13.toString(10,4);
  ASSERT_EQ(zresult24, "0.1235");
  std::string zaresult24 = zval13.toString(10,0);
  ASSERT_EQ(zaresult24, "0");

  //Test neg.
  Decimal yval1 = Decimal("-0.123");
  std::string yresult1 = yval1.toString();
  ASSERT_EQ(yresult1, "-0.123");
  std::string yresult2 = yval1.toString(10,4);
  ASSERT_EQ(yresult2, "-0.1230");
  std::string yaresult2 = yval1.toString(10,0);
  ASSERT_EQ(yaresult2, "0");

  Decimal yval14 = Decimal("-0.123449");
  std::string yresult14 = yval14.toString();
  ASSERT_EQ(yresult14, "-0.123449");
  std::string yresult24 = yval14.toString(10,4);
  ASSERT_EQ(yresult24, "-0.1234");
  std::string yaresult24 = yval14.toString(10,0);
  ASSERT_EQ(yaresult24, "0");

  Decimal yval15 = Decimal("-0.12345");
  std::string yresult15 = yval15.toString();
  ASSERT_EQ(yresult15, "-0.12345");
  std::string yresult25 = yval15.toString(10,4);
  ASSERT_EQ(yresult25, "-0.1235");
  std::string yaresult25 = yval15.toString(10,0);
  ASSERT_EQ(yaresult25, "0");

  //Test functon toString.
  Decimal bval1 = Decimal("10");
  std::string bresult1 = bval1.toString();
  ASSERT_EQ(bresult1, "10");
  std::string bresult2 = bval1.toString(10,4);
  ASSERT_EQ(bresult2, "10.0000");
  std::string baresult2 = bval1.toString(10,0);
  ASSERT_EQ(baresult2, "10");

  Decimal cval1 = Decimal("-10");
  std::string cresult1 = cval1.toString();
  ASSERT_EQ(cresult1, "-10");
  std::string cresult2 = cval1.toString(10,4);
  ASSERT_EQ(cresult2, "-10.0000");
  std::string caresult2 = cval1.toString(10,0);
  ASSERT_EQ(caresult2, "-10");

  //Test functon toString.
  Decimal gval1 = Decimal("12.34");
  std::string gresult1 = gval1.toString();
  ASSERT_EQ(gresult1, "12.34");
  std::string gresult2 = gval1.toString(10,4);
  ASSERT_EQ(gresult2, "12.3400");
  std::string garesult2 = gval1.toString(10,0);
  ASSERT_EQ(garesult2, "12");

  Decimal gval3 = Decimal("-12.34");
  std::string gresult3 = gval3.toString();
  ASSERT_EQ(gresult3, "-12.34");
  std::string gresult4 = gval3.toString(10,4);
  ASSERT_EQ(gresult4, "-12.3400");
  std::string garesult4 = gval3.toString(10,0);
  ASSERT_EQ(garesult4, "-12");

  //Test functon toString.
  Decimal val1 = Decimal("123.123");
  std::string result1 = val1.toString();
  ASSERT_EQ(result1, "123.123");

  Decimal val2 = Decimal("-123.123");
  std::string result2 = val2.toString();
  ASSERT_EQ(result2, "-123.123");

  Decimal val3 = Decimal("0.123");
  std::string result3 = val3.toString();
  ASSERT_EQ(result3, "0.123");

  Decimal val4 = Decimal("-0.123");
  std::string result4 = val4.toString();
  ASSERT_EQ(result4, "-0.123");

  Decimal v1(v);
  double d = v1.toDouble();
  ASSERT_LE(d, 123.123);
  ASSERT_GE(d, 123.122);

  long i = v1.toLong();
  ASSERT_EQ(i, 123);

  std::string str = v1.toString();
  ASSERT_EQ(str, "123.123");

  Decimal v2 = Decimal(std::string("-123456789.12345678987654321"));
  std::string str2 = v2.toString();
  ASSERT_EQ(str2, "-123456789.12345678987654321");

  Decimal v3 = Decimal(std::string("-0.12345678987654321"));
  std::string str3 = v3.toString();
  ASSERT_EQ(str3, "-0.12345678987654321");

  Decimal v4 = Decimal(std::string("0"));
  std::string str4 = v4.toString();
  ASSERT_EQ(str4, "0");

  EXPECT_ANY_THROW(Decimal(std::string("")));

  Decimal a1 = Decimal(std::string("123.456"));
  std::string aStr1 = a1.toString();
  ASSERT_EQ(aStr1, "123.456");
  ASSERT_EQ(a1.toString(5, 2), "123.46");

  Decimal a11 = Decimal(std::string("-123.456"));
  std::string aStr11 = a11.toString();
  ASSERT_EQ(aStr11, "-123.456");
  ASSERT_EQ(a11.toString(5, 2), "-123.46");

  Decimal a2 = Decimal(std::string("123.444"));
  std::string aStr2 = a2.toString();
  ASSERT_EQ(aStr2, "123.444");
  ASSERT_EQ(a2.toString(5, 2), "123.44");

  Decimal a3 = Decimal(std::string("123"));
  std::string aStr3 = a3.toString();
  ASSERT_EQ(aStr3, "123");
  ASSERT_EQ(a3.toString(5, 2), "123.00");

  Decimal a13 = Decimal(std::string("-123"));
  std::string aStr13 = a13.toString();
  ASSERT_EQ(aStr13, "-123");
  ASSERT_EQ(a13.toString(5, 2), "-123.00");

  Decimal a4 = Decimal(std::string("-123.456"));
  std::string aStr4 = a4.toString();
  ASSERT_EQ(aStr4, "-123.456");
  ASSERT_EQ(a4.toString(5, 2), "-123.46");

  Decimal a5 = Decimal(std::string("-123.444"));
  std::string aStr5 = a5.toString();
  ASSERT_EQ(aStr5, "-123.444");
  ASSERT_EQ(a5.toString(5, 2), "-123.44");

  Decimal a6 = Decimal(std::string("-123"));
  std::string aStr6 = a6.toString();
  ASSERT_EQ(aStr6, "-123");
  ASSERT_EQ(a6.toString(5, 2), "-123.00");

  Decimal a7 = Decimal(std::string("123.00000"));
  std::string aStr7 = a7.toString();
  ASSERT_EQ(aStr7, "123");
  ASSERT_EQ(a7.toString(5, 2), "123.00");

  Decimal a8 = Decimal(std::string("-123.00000"));
  std::string aStr8 = a8.toString();
  ASSERT_EQ(aStr8, "-123");
  ASSERT_EQ(a8.toString(5, 2), "-123.00");

  Decimal a18 = Decimal(std::string("-123.00000"));
  std::string aStr18 = a18.toString();
  ASSERT_EQ(aStr18, "-123");
  ASSERT_EQ(a18.toString(5, 2), "-123.00");

  Decimal a9 = Decimal(std::string("123.456"));
  std::string aStr9 = a9.toString();
  ASSERT_EQ(aStr9, "123.456");
  ASSERT_EQ(a9.toString(5, 0), "123");

  Decimal a10 = Decimal(std::string("-123.456"));
  std::string aStr10 = a10.toString();
  ASSERT_EQ(aStr10, "-123.456");
  ASSERT_EQ(a10.toString(5, 0), "-123");

  Decimal a19 = Decimal(std::string("123.556"));
  std::string aStr19 = a19.toString();
  ASSERT_EQ(aStr19, "123.556");
  ASSERT_EQ(a19.toString(5, 0), "124");

  Decimal b11 = Decimal(std::string("12.47799999"));
  std::string bStr11 = b11.toString();
  ASSERT_EQ(bStr11, "12.47799999");
  ASSERT_EQ(b11.toString(6, 4), "12.4780");

  Decimal a12 = Decimal(std::string("-12.47799999"));
  std::string aStr12 = a12.toString();
  ASSERT_EQ(aStr12, "-12.47799999");
  ASSERT_EQ(a12.toString(6, 4), "-12.4780");

  //Test dec + dec.
  Decimal result = Decimal(std::string("123.123")) + Decimal(std::string("123.123"));
  ASSERT_EQ(result.toString(), "246.246");

  //Test dec - dec.
  Decimal minusResult = Decimal(std::string("123.123")) - Decimal(std::string("123.123"));
  ASSERT_EQ(minusResult.toString(), "0");
  ASSERT_EQ(minusResult.toString(5, 2), "0.00");

  //Test dec == dec.
  ASSERT_TRUE(Decimal(std::string("123.123")) == Decimal(std::string("123.123")));

  //Test dec != dec.
  ASSERT_TRUE(Decimal(std::string("123.123")) != Decimal(std::string("123.124")));

  //Test dec <= dec.
  ASSERT_TRUE(Decimal(std::string("123.123")) <= Decimal(std::string("123.124")));

  //Test dec < dec.
  ASSERT_TRUE(Decimal(std::string("123.123")) < Decimal(std::string("123.124")));

  //Test dec >= dec.
  ASSERT_FALSE(Decimal(std::string("123.123")) >= Decimal(std::string("123.124")));

  //Test dec > dec.
  ASSERT_FALSE(Decimal(std::string("123.123")) > Decimal(std::string("123.124")));

  //Test -dec.
  //This case will append '.00' in serial stage.
  ASSERT_EQ((-Decimal(std::string("10.00"))).toString(), std::string("-10"));

  //Test -dec.
  ASSERT_EQ((-Decimal(std::string("123.123"))).toString(), std::string("-123.123"));

  //Test dec.
  ASSERT_EQ((Decimal(std::string("123.123"))).toString(), std::string("123.123"));

  //Test +dec.
  ASSERT_EQ((+Decimal(std::string("123.123"))).toString(), std::string("123.123"));

  //Test dec.
  ASSERT_EQ((Decimal(std::string("123"))).toLong(), 123);

  //Test dec 0.
  ASSERT_EQ((Decimal(std::string("0"))).toLong(), 0);

  //Test +dec 0.
  ASSERT_EQ((+Decimal(std::string("-0"))).toLong(), 0);

  //Test -dec 0.
  ASSERT_EQ((-Decimal(std::string("0"))).toLong(), 0);

  //DecimalP p = DecimalP((float)12.34);
  ASSERT_EQ(DecimalP((long)12).GetPtr()->toString(10,4), std::string("12.0000"));
  ASSERT_EQ(DecimalP((float)12.34).GetPtr()->toString(10,4), std::string("12.3400"));
  ASSERT_EQ(DecimalP((double)12.34).GetPtr()->toString(10,4), std::string("12.3400"));

  //+, -, *, /
  Decimal result0 = Decimal(std::string("100.50")) + Decimal(std::string("12345.6789"));
  std::cout << result0.toString() << std::endl;
  ASSERT_EQ(result0.toString(), std::string("12446.1789"));

  Decimal aresult2 = Decimal(std::string("100.50")) - Decimal(std::string("12345.6789"));
  std::cout << aresult2.toString() << std::endl;
  ASSERT_EQ(aresult2.toString(), std::string("-12245.1789"));

  Decimal bresult0 = Decimal(std::string("25.75")) + Decimal(std::string("987.1234"));
  std::cout << bresult0.toString() << std::endl;
  ASSERT_EQ(bresult0.toString(), std::string("1012.8734"));

  Decimal bbaresult2 = Decimal(std::string("25.75")) - Decimal(std::string("987.1234"));
  std::cout << bbaresult2.toString() << std::endl;
  ASSERT_EQ(bbaresult2.toString(), std::string("-961.3734"));

  Decimal ddbresult0 = Decimal(std::string("-50.25")) + Decimal(std::string("-1000.5000"));
  std::cout << ddbresult0.toString() << std::endl;
  ASSERT_EQ(ddbresult0.toString(), std::string("-1050.75"));
  ASSERT_EQ(ddbresult0.toString(10,4), std::string("-1050.7500"));

  Decimal ddbbaresult2 = Decimal(std::string("25.75")) - Decimal(std::string("-961.3734"));
  std::cout << ddbbaresult2.toString() << std::endl;
  ASSERT_EQ(ddbbaresult2.toString(), std::string("987.1234"));
  ASSERT_EQ(ddbbaresult2.toString(10,4), std::string("987.1234"));
}


