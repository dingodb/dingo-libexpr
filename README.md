# Dingo Expression Coprocessor

[![Build](https://github.com/dingodb/dingo-libexpr/actions/workflows/build.yml/badge.svg)](https://github.com/dingodb/dingo-libexpr/actions/workflows/build.yml)

This project is the coprocessor part of [Dingo Expression](https://github.com/dingodb/dingo-expr), written in C++. The project is made standalone to be easily integrated into other C++ projects.

## Getting Started

```cpp
#include "runner.h"

using namespace dingodb::expr;
```

Assume `bytes` is an array of type `Byte`, which contains the encoded expression

```cpp
Runner runner;
runner.Decode(bytes, sizeof(bytes)/sizeof(Byte));
runner.Run();
if (ruuner.GetType() == TYPE_INT32) {
  Wrap<int32_t> result = runner.GetResult<int32_t>();
}
```

`Wrap` is a type alias of `std::optional`, so the result may have or have not a value.

An raw result which contains any type can be retrieved by

```cpp
Operand result = runner.GetRawResult();
```

`Operand` is a type alias of `std::any`, which means it can contain any type of data. Generally, it contains a value of `Warp<T>` type.

If the expression contains variables, a `Tuple` should be bind to the `Runner` before run

```cpp
Tuple tuple{Wrap<int32_t>(1), Wrap<int32_t>(2)};
runner.BindTuple(&tuple, false);
```

where `Tuple` is a type alias of `std::vector<Operand>`.

The 2nd parameter of `BindTuple` indicate whether the `Runner` should take ownership of the pointer. If it set to `true`, the pointer would be released when the `Runner` is destructed.

## Used by

- [Dingo-Store](https://github.com/dingodb/dingo-store)
