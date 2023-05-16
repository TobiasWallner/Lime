
For developers: try to follow the Style Guides and Nameing Conventions:

### Nameing Convention

| **Item**					| **Convention**			|
|---------------------------|---------------------------|
| Types and Namespaces		| `UpperCamelCase`			|
| Variables					| `lowerCamelCase`			|
| Functions and Methods		| `snake_case` 				|
| `#define` constants		| `SCREAMING_SNAKE_CASE`	|
| `#define` functions		| `snake_case` 				|

Exception: If an *Item* name is part of an *Item* with a different *Convention* it may be written in its original *Convention*

Example: If a type name is part of a function name you may write the type in `UpperCamelCase` and the rest in `snake_case`.

```C++
FunctionCall parse_FunctionCall(TokenStream tokenStrem);
```

## Style Guides 

Try to follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html):

## Additions to the Google Style Sheet:

### Additonally to the [Order of Includes](https://google.github.io/styleguide/cppguide.html#Include_What_You_Use) 

sort your includes by their library and make a comment above them stateing where they are comeing from. Excample:

```C++
// Project
#include "foo/server/fooserver.h"

// C and System
#include <sys/types.h>
#include <unistd.h>

// std
#include <string>
#include <vector>

// foo
#include "foo/basictypes.h"
#include "foo/server/bar.h"

// third party - for every third party library
#include "third_party/absl/flags/flag.h"
```


