# Dingo Expression Coprocessor

[![Build](https://github.com/dingodb/dingo-libexpr/actions/workflows/build.yml/badge.svg)](https://github.com/dingodb/dingo-libexpr/actions/workflows/build.yml)

This project is the coprocessor part of [Dingo Expression](https://github.com/dingodb/dingo-expr), written in C++. The project is made standalone to be easily integrated into other C++ projects.

## Getting Started

```cpp
#include "expr/runner.h"

using namespace dingodb::expr;
```

Assume `bytes` is an array of type `expr::Byte[size]`, which contains the encoded expression

```cpp
Runner runner;
runner.Decode(bytes, size);
runner.Run();
Operand result = runner.Get();
```

`Operand` is a class to hold any supported data types. An `Operand` object can be created from any supported type `T` by the constructor `Operand<T>(T value)`, and the value contained in an `Operand` object can be retrieved by

```cpp
T Operand::GetValue() const;
```

`Operand` can also hold a `NULL` value, and can be tested by the overloaded "equals/non-equals" operator, like `operand == nullptr` or `operand != nullptr`.

## Variables

Dingo Expression Coprocessor supports variables. Internally, variables are indexed by integers, start from 0. If the expression contains variables, a `Tuple` must be bind to the `Runner` before run

```cpp
Tuple tuple{1, "Alice", 100.0};
runner.BindTuple(&tuple);
```

where `Tuple` is a type alias of `std::vector<Operand>`. The variables indexed by 0, 1, 2 would be assigned the 1st, 2nd, 3rd values of the `tuple`, respectively.

Note:

- The `runner` does not take ownership of the `tuple`, which should be released by the caller if necessary
- The number of elemnets in the `tuple` is not checked when retrieving the value and it is the caller's duty to ensure the index of variables are valid

## Relational Algebra

Dingo Expression Coprocessor has also limited implementation for relational algebra. To use it, add the following to the source code

```cpp
#include "rel/rel_runner.h"

using namespace dingodb::expr;
using namespace dingodb::rel;
```

Assume `bytes` is an array of type `expr::Byte[size]`, which contains the encoded relational algebra

```cpp
auto *rel = new RelRunner();
rel->Decode(buf, size);
```

Then `Tuple`s can be put into the `RelRunner`

```cpp
for (int i = 0; i < tuples.size(); ++i) {
    const auto *output = rel->Put(tuples[i]);
}
```

The `output` returned is another `Tuple` or `nullptr`. The former is a valid result and the latter means no result can be retrieved immediately for the input may be fitered out or cached.

If there are datum cached in the `RelRunner` (which is typically when the relational algebra contains any aggregation), the cached results must be taken out after all tuples are put in

```cpp
while ((const auto *output = rel->Get()) != nullptr) {
    do_some_thing(output);
};
```

Note:

- The `RelRunner` takes over the ownership of the `Tuple` put in. The caller must not try to release it
- If the `output` returned either by `Put` or `Get` is not `nullptr`, it must be released by the caller
- The implementation of `RelRunner` is not thread-safe

## Implementations

### Expression Evaluating

The `Runner` class is for expression evaluating. The following figure illustrate the evaluating process, in which the expression is like `2 + T[0] * 3`

![Implementation of runner](docs/images/runner.drawio.svg)

The `Runner` contains an operand stack and an operator verctor. The operator vector is constructed by `Decode` method from the encoded bytes of an expression. Each operator can manipulate (push/pop) operands in the operand stack following pre-defined process. When `Run` method is called, operators in the vector are carried out one by one. By calling `Get` method, the top elemement in the stack is poped out as the returned result. Mostly, there is only one oprand left in the stack after `Run` for a valid expression.

## Encodings

### Data Types

Data types are represented by 4 bit, as described in the following table

| Type | Description | Decimal Code | Hex Code | Corresponding C++ Type |
|---|---|---|---|---|
| `INT32` | 32 bit integer | `1` | `0x1` | `int32_t` |
| `INT64` | 64 bit integer | `2` | `0x2` | `int64_t` |
| `BOOL` | boolean | `3` | `0x3` | `bool` |
| `FLOAT` | single precision floating point | `4` | `0x4` | `float` |
| `DOUBLE` | double precision floating point | `5` | `0x5` | `double` |
| `DECIMAL` | arbitrary-precision decimal number | `6` | `0x6` | **Not supported yet** |
| `STRING` | string | `7` | `0x7` | `std::shared_ptr<std::string>` |

Note the C++ types are wrapped in `Operand` class.

### Immediate Numbers

Immediate numbers are const values embedded in the encoded expressions, to implement const, index of variables, length of arrays, etc. The encoding of immediate numbers is described in the following table

| Type of Immediate Number | Description of Encoding |
|---|---|
| `INT32` | See encoding of VarInt in [Protocol Buffers](https://protobuf.dev/programming-guides/encoding/) |
| `INT64`| The same as above |
| `FLOAT` | 4 bytes big-endian representation |
| `DOUBLE` | 8 bytes big-endian representation |
| `DECIMAL` | To be defined |
| `STRING` | Encoding the byte length of it first, followed by all the types of it. The length is encoded as `INT32` type |

### End of Expression

If the expressions are used in relational algebra, a special byte may be needed to indicate the end of expression. Byte `0x00` is used for that purpose. But byte `0x00` is not always means the termination of expression, because there may be any bytes in the presentation of an immediate number.

### Operators

The expressions Dingo Expression Coprocessor processed are represented in suffix style, i.e. the operator is put after its operands. Consts and variables are also treated as operator in this representation. All the operators are encoded into unique bytes representation. Operators may have one or more immediate numbers followed. Generally

- For 1-byte representation, the HSB is set to `0`, and the lower 4 bits can be used to indicate a data type if needed
- For 2-bytes representation, the HSB is set to `1`, and the higher 4 bits and the lower 4 bits of the 2nd byte can be used to indicate two data types individually if needed
- No byte `0x00` or `0xFF` is allowed in the representation except for immediate numbers

The encoding of operators is illustrated in the following figure

![Encoding of operators](docs/images/encoding.drawio.svg)

#### One-byte Operators

The encoding of one-byte operators is as follows (The template-like `T` is any of the [Data Types](#data-types))

| Operator | Higher 4 Bits | Lower 4 Bits | Immediate Number | Description |
|---|---|---|---|---|
| `NULL<T>` | `0x0` | Encode type `T` | None | `NULL` value |
| `CONST<T>` | `0x1` | Encode type `T` | `T` type value | `T` type const, `T` != `BOOL` |
| `CONST<BOOL>` | `0x1` | Encode type `BOOL` | None | `BOOL` value `true` |
| `CONST_N<T>` | `0x2` | Encode type `T` | `T` type value | `T` type const, `T` == `INT32` or `T` == `INT64`, the real value is the inverse of the encoded immediate number |
| `CONST_N<BOOL>` | `0x2` | Encode type `BOOL` | None | `BOOL` value `false` |
| `VAR<T>` | `0x3` | Encode type `T` | `INT32` type value | `T` type variable indexed by an integer |
| `VAR_S<T>` | `0x4` | Encode type `T` | `STRING` type value | `T` type variable indexed by a string. **Not implemented yet** |
| `NOT` | `0x5` | `0x1` | None | Unary `NOT` |
| `AND` | `0x5` | `0x2` | None | Binary `AND` |
| `OR` | `0x5` | `0x3` | None | Binary `OR` |
| `AND_FUN` | `0x5` | `0x4` | `INT32` type value | Variadic `AND`, the immediate number is the number of parameters. **Not implemented yet** |
| `OR_FUN` | `0x5` | `0x5` | `INT32` type value | Variadic `OR`, the immediate number is the number of parameters. **Not implemented yet** |

#### Arrays

| Operator | Higher 4 Bits | Lower 4 Bits | Immediate number 0 | Imediate number 1..N | Description |
|---|---|---|---|---|---|
| `ARRAY<T>` | `0x6` | Encode type `T` | `INT32` type number `N`, the number of elements | encode `N` values of `T` type continously | Array of `T` type consts. Not implement yet |
| `ARRAY<AGG>` | None | None | `INT32` type number `N`, the number of elements | encode `N` aggregation functions continously | Array of aggregations, used in relational algebra |
| `ARRAY<CONST>` | None | None | `INT32` type number `N`, the number of elements | encode `N` `CONST`/`CONST_N` expressions continously | Tuple of consts. **Not implemented yet** |

#### Map (Key-Value Data)

| Operator | 1st Byte | Higher 4 Bits of 2nd Byte | Lower 4 Bits of 2nd Byte | Immediate Number 0 | Immediate Number 1..2N | Description |
|---|---|---|---|---|---|---|
| `MAP<K, V>` | `0x80` | Encode type `K` | Encode type `V` | `INT32` type number `N`, tye number of key-value pairs | encode `K` and `V` type consts alternately | Map of consts, `K` and `V` is the type of key and value. **Not implemented yet** |

#### Two-bytes operators

The encoding of two-bytes operators is as follows (The template-like `T` is any of the [Data Types](#data-types))

| Operator | 1st Byte | Higher 4 Bits of 2nd Byte | Lower 4 Bits of 2nd Byte | Immediate Number | Description |
|---|---|---|---|---|---|
| `POS<T>` | `0x81` | `0x0` | Encode type `T` | None | Unary `+` |
| `SUM<T>` | `0x81` | `0x1` | Encode type `T` | `INT32` type value, the number of parameters | Varidiac `SUM`. **Not implemented yet** |
| `NEG<T>` | `0x82` | `0x0` | Encode type `T` | None | Unary `-` |
| `ADD<T>` | `0x83` | `0x0` | Encode type `T` | None | Binary `+` |
| `SUB<T>` | `0x84` | `0x0` | Encode type `T` | None | Binary `-` |
| `MUL<T>` | `0x85` | `0x0` | Encode type `T` | None | Binary `*` |
| `DIV<T>` | `0x86` | `0x0` | Encode type `T` | None | Binary `/` |
| `MOD<T>` | `0x87` | `0x0` | Encode type `T` | None | Binary `%` |
| `EQ<T>` | `0x91` | `0x0` | Encode type `T` | None | Binary `==` |
| `GE<T>` | `0x92` | `0x0` | Encode type `T` | None | Binary `>=` |
| `GT<T>` | `0x93` | `0x0` | Encode type `T` | None | Binary `>` |
| `LE<T>` | `0x94` | `0x0` | Encode type `T` | None | Binary `<=` |
| `LT<T>` | `0x95` | `0x0` | Encode type `T` | None | Binary `<` |
| `NE<T>` | `0x96` | `0x0` | Encode type `T` | None | Binary `<>` |
| `IS_NULL<T>` | `0xA1` | `0x0` | Encode type `T` | None | Unary `IS_NULL` function |
| `IS_TRUE<T>` | `0xA2` | `0x0` | Encode type `T` | None | Unary `IS_TRUE` function |
| `IS_FALSE<T>` | `0xA3` | `0x0` | Encode type `T` | None | Unary `IS_FALSE` function |
| `MIN<T>` | `0xB1` | `0x0` | Encode type `T` | None | Binary `MIN` function |
| `VARG_MIN<T>` | `0xB1` | `0x1` | Encode type `T` | `INT32` type value, the number of parameters | Variadic `MIN` function |
| `MAX<T>` | `0xB2` | `0x0` | Encode type `T` | None | Binary `MAX` function |
| `VARG_MAX<T>` | `0xB2` | `0x1` | Encode type `T` | `INT32` type value, the number of parameters | Variadic `MAX` function |
| `ABS<T>` | `0xB3` | `0x0` | Encode type `T` | None | Unary `ABS` function |
| `CAST<D, T>` | `0xF0` | Encode type `D` | Encode type `T` | None | Casting from `T` type to `D` type |

#### Functions

The coding of functions is as follows

| Operator | 1st Byte | 2nd Byte | Immediate Number | Description |
|---|---|---|---|---|
| `FUN` | `0xF1` | 1-byte positive integer, the sequence number of the function | None | Pre-defined functions |
| `VARG_FUN` | `0xF2` | 1-byte positive integer, the sequence number of the function | `INT32` type value, the number of parameters | Pre-defined varidiac functions. **Not implemented yet** |

### Sequence Number of Functions

#### Mathematical Functions

| Function | Type of Parameters | Type of Return Value | Sequence Number | Description |
|---|---|---|---|---|
| `CEIL` | `DOUBLE` | `DOUBLE` | `0x01` | Round up to the smallest integer |
| `FLOOR` | `DOUBLE` | `DOUBLE` | `0x02` | Round down to the largest integer |
| `ROUND` | `INT64`, `INT32` | `INT64` | `0x03` | Round. **Not implemented yet** |
| `ROUND` | `DOUBLE`, `INT32` | `DOUBLE` | `0x04` | Round. **Not implemented yet** |
| `POW` | `DOUBLE` | `DOUBLE` | `0x05` | Power. **Not implemented yet** |
| `POW` | `INT64` | `INT64` | `0x06` | Power. **Not implemented yet** |
| `SIN` | `DOUBLE` | `DOUBLE` | `0x07` | Sine |
| `COS` | `DOUBLE` | `DOUBLE` | `0x08` | Cosine |
| `TAN` | `DOUBLE` | `DOUBLE` | `0x09` | Tangent |
| `ASIN` | `DOUBLE` | `DOUBLE` | `0x0A` | Arc sine |
| `ACOS` | `DOUBLE` | `DOUBLE` | `0x0B` | Arc cosine |
| `ATAN` | `DOUBLE` | `DOUBLE` | `0x0C` | Arc tangent |
| `SINH` | `DOUBLE` | `DOUBLE` | `0x0D` | Hyperbolic sine |
| `COSH` | `DOUBLE` | `DOUBLE` | `0x0E` | Hyperbolic cosine |
| `TANH` | `DOUBLE` | `DOUBLE` | `0x0F` | Hyperbolic tangent |
| `EXP` | `DOUBLE` | `DOUBLE` | `0x10` | Exponential |
| `LOG` | `DOUBLE` | `DOUBLE` | `0x11` | Logarithm |

#### String Functions

| Function | Type of Parameters | Type of Return Value | Sequence Number | Description |
|---|---|---|---|---|
| `CHAR_LENGTH` | `STRING` | `INT32` | `0x20` | Return the length of a string. **Not implemented yet** |
| `CONCAT` | `STRING`, `STRING` | `STRING` | `0x21` | Concatenate two strings |
| `LOWER` | `STRING` | `STRING` | `0x22` | Converts to lowercase |
| `UPPER` | `STRING` | `STRING` | `0x23` | Converts to uppercase |
| `LEFT` | `STRING`, `INT32` | `STRING` | `0x24` | Sub string from left |
| `RIGHT` | `STRING`, `INT32` | `STRING` | `0x25` | Sub string from right |
| `TRIM` | `STRING` | `STRING` | `0x26` | Trim whitespaces |
| `TRIM` | `STRING`, `STRING` | `STRING` | `0x27` | Trim specified string. **Not implemented yet** |
| `LTRIM` | `STRING` | `STRING` | `0x28` | Trim whitespaces from left |
| `LTRIM` | `STRING`, `STRING` | `STRING` | `0x29` | Trim specified string from left. **Not implemented yet** |
| `RTRIM` | `STRING` | `STRING` | `0x2A` | Trim whitespaces from right |
| `RTRIM` | `STRING`, `STRING` | `STRING` | `0x2B` | Trime specified string from right. **Not implemented yet** |
| `SUBSTR` | `STRING`, `INT32`, `INT32` | `STRING` | `0x2C` | Sub string from a "position" to another "position". The 1st "position" is `0` |
| `SUBSTR` | `STRING`, `INT32` | `STRING` | `0x2D` | Sub string from a "position" to the end. The 1st "position" is `0` |
| `MID` | `STRING`, `INT32`, `INT32` | `STRING` | `0x2E` | Sub string from a "position" to the specified length. The 1st "position" is `1` |
| `MID` | `STRING`, `INT32` | `STRING` | `0x2F` | Sub string from a "position" to the end. The 1st "position" is `1` |
| `REPEAT` | `STRING`, `INT32` | `STRING` | `0x30` | Repeat the string. **Not implemented yet** |
| `REVERSE` | `STRING` | `STRING` | `0x31` | Reverse the string. **Not implemented yet** |
| `REPLACE` | `STRING`, `STRING`, `STRING` | `STRING` | `0x32` | Search and replace. **Not implemented yet** |
| `LOCATE` | `STRING`, `STRING`, `INT32` | `INT32` | `0x33` | Find the "position" of a string in another string. The 1st "position" is `1`. **Not implemented yet** |
| `LOCATE` | `STRING`, `STRING` | `INT32` | `0x34` | Find the "position" of a string in another string. The 1st "position" is `1`. **Not implemented yet** |
| `FORMAT` | DOUBLE, `INT32` | `STRING` | `0x35` | Formatted output of a number. **Not implemented yet** |

#### Aggregation Functions

Aggregation functions are used only in relational algebra. Complicated aggregations such as `AVG` is not supported here. Actually, `AVG` can be converted to `SUM` and `COUNT` with a projection before encoded.

Encoding of aggregations does not follow the way to encode normal function and has no leading `0xF1` byte, but is more like that of one-byte operators, as in the following table

| Aggregation | Type of Parameter | Type of Return Value | Higher 4 Bits | Low 4 Bits | Immediate Number | Description |
|---|---|---|---|---|---|---|
| `COUNT_ALL` | None | `INT64` | `0x1` | `0x0` | None | Count all rows |
| `COUNT<T>` | `T` | `INT64` | `0x1` | Encode type `T` | `INT32` type value, the column index | Count only non-null values |
| `SUM<T>` | `T` | `T` | `0x2` | Encode type `T` | `INT32` type value, the column index | Sum the values |
| `MAX<T>` | `T` | `T` | `0x3` | Encode type `T` | `INT32` type value, the column index | Maximum of the values |
| `MIN<T>` | `T` | `T` | `0x4` | Encode type `T` | `INT32` type values, the column index | Minimum of the values |

Note:

- The column indices are all started from `0`
- Some aggregation functions are requried to return `0` if there are no input rows, such as `COUNT`. For simplicity, `NULL` is returned for all aggregation functions here and it is in the final reducing stage to convert `NULL` to `0`

### Relational Algebra Operators

Only "simple" (the operators have only one single input and one output) relational algebra expressions are supported. These operators are chained with one's output connected to another's input to form a whole relational algebra expression. They are encoded one by one from the input end to the output end.

The encoding of each algebra operators contains an unique "leading byte", a "payload" for the parameters and expressions in the operator and a "trailing byte" which is always `EOE` (End Of Expression). The details are in the following table

| Operator | Leading Byte | Payload | Trailing Byte |
|---|---|---|---|
| Filter | `0x71` | Encode the filter expression | `EOE` |
| Project | `0x72` | Encode the project expression one by one | `EOE` |
| Grouped Aggregation | `0x73` | Encode group indices as `ARRAY<INT32>` type, then the list of aggregation functions as `ARRAY<AGG>` type | `EOE` |
| Ungrouped Aggregation | `0x74` | Encode the list of aggregation functions as `ARRAY<AGG>` type | `EOE` |

A "Project" operator may contains several expressions but they can be concatenated into one "huge" expression without any separator simplify the evaluating process. The "huge" expression is decoded by one `Runner`, and after evaluating there will be several results left in the operand stack just as needed. These results can be taken out by multiple calls to `Get` method.

## Used by

- [Dingo-Store](https://github.com/dingodb/dingo-store)
- [Dingo Expression](https://github.com/dingodb/dingo-expr)
