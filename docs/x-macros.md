# X-Macros

X-macros are a powerful C/C++ preprocessor technique for defining lists of
related items in a single place and then reusing that list to generate code or
data declarations multiple times in consistent ways.

## Example

In a file `data.inc`:

```C
#ifndef X
#error "This file is intended for textual inclusion with the X macro defined"
#endif

X(marshmallow, ginger, 3)
X(milkshake, tortoiseshell, 3)
X(lexa, black, 6)
```

In a different `main.cpp` file where it is going to be used:
```C
#define X(name, color, age) name,
  enum class cat {
  #include "data.inc"
  };
#undef X

void print_known_cats() {
  #define X(name, color, age)\
    fmt::print("{} the {} cat, age {}\n", #name, #color, age);
  #include "data.inc"
  #undef X
}
```

The `#include` statements in this case just copy-pastes the content of their
target. So, the following code is equivalent to the previous CPP file:

```C
#define X(name, color, age) name,
  enum class cat {
  // following 3 statements comes from the `data.inc` file
  X(marshmallow, ginger, 3)
  X(milkshake, tortoiseshell, 3)
  X(lexa, black, 6)
  };
#undef X

void print_known_cats() {
  #define X(name, color, age)\
    fmt::print("{} the {} cat, age {}\n", #name, #color, age);
  // following 3 statements comes from the `data.inc` file
  X(marshmallow, ginger, 3)
  X(milkshake, tortoiseshell, 3)
  X(lexa, black, 6)
  #undef X
}
```
In the above C++ examples, we define macros, use them immediately and then
remove them. After the preprocessor step, this will become:

```C
enum class cat {
  marshmallow,
  milkshake,
  lexa,
};

void print_known_cats() {
  fmt::print("{} the {} cat, age {}\n", "marshmallow", "ginger", 3);
  fmt::print("{} the {} cat, age {}\n", "milkshake", "tortoiseshell", 3);
  fmt::print("{} the {} cat, age {}\n", "lexa", "black", 6);
}
```
