# Meta Enumerator
Meta Enumerator is a modern, single-header C++11 library to enhance enumerator capabilities and ease enum operations. The primary feature is enabling automatic generation of arbitrarily large (bitwise) masks based off of regular, sequential enum values - useful when you want to be able to enumerate values while also using them as flags.

The enum masks are statically checked, type safe and statically allocated for better performance. Also, when using `enum classes`, Mask type can be forward declared (like the `enum class` itself) and therefore used in function parameters before the enum is fully defined.

- [Features](#Features)
- [Missing Features](#Missing-Features)
- [Installation](#Installation)
- [Usage](#Usage)
	- [Forward Declaration](#Forward-Declaration)
	- [Arbitrarily Large Masks](#Arbitrarily-Large-Masks)


## Features

- Automatic generation of masks based off enums
	- Mask is held in a custom class, guaranteeing type safety
	- Mask can be arbitrarily large, to store any amount of flags
	- Mask storage is always allocated statically, using templates
	- Static checks ensure Mask will always be large enough to contain all enum values, else a compile error is raised
	- Mask can detect if all enum flags will fit in a built-in type and use that
	- If using built-in type, all bitwise operations on the mask will collapse to built-in bitwise operators
	- If using `enum class`, Mask type can be forward declared and used in function parameters before the enum is defined
- Opted-in conversion to and from string (you need to specify the strings though, see [Usage](#usage))
- Opted-in support for automatic mathematical and logical operations on the enum values
- Ability to customize implementation to attach arbitrary meta-data to each enum value
- **NO MACROS!** I really dislike how many C++ enum utilities make heavy usage of macros, which is another reason why I wrote this library

## Missing Features

Things I'd like to implement at some point but are not there yet.

- Endianness awareness; make the library work on Big Endian systems
- Tests! Right now there are none, although I've tested this quite a bit as I actively use it in my game
- Samples and Documentation on how to modify some of the behaviors


## Installation

Just copy `EnumeratorMeta.hpp` to your project and include it like the following:

```cpp
#define ENUMERATORMETA_NAMESPACE MyRootNamespace
#include "EnumeratorMeta.hpp"
```

`MyRootNamespace` is the root namespace in your project. Define this for quality of life, otherwise you'd be required to close/open namespaces every time you need to define the template specializations for your own enum types. If you don't use a namespace (i.e. your code lives in the global namespace) then you can remove the `#define ENUMERATORMETA_NAMESPACE MyRootNamespace` line entirely.


## Usage

This is how you define the meta information on your enum:

```cpp
#define ENUMERATORMETA_NAMESPACE MyRootNamespace
#include "EnumeratorMeta.hpp"

namespace MyRootNamespace
{
	
enum class TargetType
{
	NONE = 0,
	ENEMY_ALIVE,
	ENEMY_CORPSE,
	ENEMY_SPOT,
	ALLY_ALIVE,
	ALLY_CORPSE,
	ALLY_SPOT,
	MAX = ALLY_SPOT
};

template <>
class EnumeratorMeta<TargetType> : public EnumeratorMetaDefault<TargetType>
{
public:
	static constexpr const bool math_operators = true;
	static constexpr const bool logic_operators = true;
	static constexpr const bool string_operators = true;
	static constexpr const TargetType MAX_VALUE = TargetType::MAX;
	
	static constexpr const StringEntry string_values[]{
		{ TargetType::ENEMY_ALIVE, "ENEMY_ALIVE" },
		{ TargetType::ENEMY_CORPSE, "ENEMY_CORPSE" },
		{ TargetType::ENEMY_SPOT, "ENEMY_SPOT" },
		{ TargetType::ALLY_ALIVE, "ALLY_ALIVE" },
		{ TargetType::ALLY_CORPSE, "ALLY_CORPSE" },
		{ TargetType::ALLY_SPOT, "ALLY_SPOT" }
	};
};

}
```

This is how you'd create a mask:
```cpp
// creates a mask named accepted_targets
auto accepted_targets = TargetType::ENEMY_ALIVE | TargetType::ALLY_ALIVE;
std::cout << "Accepted Targets: " << accepted_targets << "\n";
// prints: Accepted Targets: ENEMY_ALIVE, ALLY_ALIVE
```

**Note**: `accepted_targets` above is an instance of a custom (templated) class and therefore type-safe. In fact, if you don't want to use `auto` you can actually specify the type, e.g.
```cpp
// creates a mask named accepted_targets
EnumeratorMask<TargetType> accepted_targets = TargetType::ENEMY_ALIVE | TargetType::ALLY_ALIVE;
std::cout << "Accepted Targets: " << accepted_targets << "\n";
// prints: Accepted Targets: ENEMY_ALIVE, ALLY_ALIVE
```

This is how you'd check a mask against a flag:
```cpp
// creates a mask named accepted_targets
auto accepted_targets = TargetType::ENEMY_ALIVE | TargetType::ALLY_ALIVE;
if (accepted_targets.has(TargetType::ENEMY_ALIVE))
	std::cout << "Looking for a live enemy...\n";
// prints: Looking for a live enemy...
```

This is how you obtain the string representation for an enum value:

```cpp
const char* targetStr = EnumeratorSerializer<TargetType>::get_name(TargetType::ALLY_CORPSE);
std::cout << "We got this target: " << targetStr;
// prints: We got this target: ALLY_CORPSE
```

*Note*: the library supports `ostream operator<<` directly so if you want to print out the value you can simply do:
```cpp
auto target = TargetType::ALLY_CORPSE;
std::cout << "We got this target: " << target;
// prints: We got this target: ALLY_CORPSE
```

This is how you obtain an enum value from its string representation:

```cpp
TargetType target = EnumeratorSerializer<TargetType>::get_value("ALLY_CORPSE");
if (target == TargetType::ALLY_CORPSE)
	std::cout << "We got an ally's corpse!";
// prints: We got an ally's corpse!
```

### Forward Declaration

Another useful feature of the library is allowing forward declaring the Mask type for `enum classes`, for instance:

```cpp
enum class TargetType;
using TargetTypeMask = EnumeratorMask<TargetType>;
```

This makes it possible to specify `TargetTypeMask` as an argument type in function declarations, and then only `#include` the `enum class` definition in the `.cpp` file that contains the function body. This means adding entries to the enum class will require less re-compilation.

### Arbitrarily Large Masks

The usual way to use enums for flags in a mask in C++ is to define the enum so that each value in it is a power of 2. In addition to the issue of making the masks not type safe, this also limits how large the mask can be to whatever built-in integer type is largest. In other words, assuming your code has to be cross-platform, you will be limited to a maximum of 64 flags.

The Mask type instead has no such limitation as it can store any amount of flags. The Mask will try to find the largest built-in type available to store the flags and if none is available it will use its own large integer representation.

Keep in mind that of course, when using this large integer storage, performance will be lower than using a built-in type, because the operations have to be abstracted. This is however unavoidable because representing such values would always require a custom type.

Also, because it's a goal of the implementation for all storage to be statically allocated, you have to specify how many flags, or bits of storage, you need for the mask by passing an extra parameter to the template, like below:

```cpp
enum class TargetType;
using TargetTypeMask = EnumeratorMask<TargetType, 100>; // store up to 100 different flags in this mask
```

You also need to specify this when adding the meta information in the specialization:

```cpp
#define ENUMERATORMETA_NAMESPACE MyRootNamespace
#include "EnumeratorMeta.hpp"

namespace MyRootNamespace
{
	
enum class TargetType
{
	NONE = 0,
	ENEMY_ALIVE,
	ENEMY_CORPSE,
	ENEMY_SPOT,
	ALLY_ALIVE,
	ALLY_CORPSE,
	ALLY_SPOT,
	MAX = ALLY_SPOT
};

template <>
class EnumeratorMeta<TargetType> : public EnumeratorMetaDefault<TargetType, false, 100> // up to 100 different values
{
public:
	static constexpr const bool math_operators = true;
	static constexpr const bool logic_operators = true;
	static constexpr const bool string_operators = true;
	static constexpr const TargetType MAX_VALUE = TargetType::MAX;
	
	static constexpr const StringEntry string_values[]{
		{ TargetType::ENEMY, "ENEMY" },
		{ TargetType::ENEMY_ALIVE, "ENEMY_ALIVE" },
		{ TargetType::ENEMY_CORPSE, "ENEMY_CORPSE" },
		{ TargetType::ENEMY_SPOT, "ENEMY_SPOT" },
		{ TargetType::ALLY, "ALLY" },
		{ TargetType::ALLY_ALIVE, "ALLY_ALIVE" },
		{ TargetType::ALLY_CORPSE, "ALLY_CORPSE" },
		{ TargetType::ALLY_SPOT, "ALLY_SPOT" }
	};
};

}
```

This is the reason you have to specify the `MAX_VALUE` constant in the specialization - this way, if you keep adding values to the enum and forget to make the mask large enough, the system can check (at compile time) and ensure that your maximum value can fit inside the mask.

There's no limit to the amount of flags, other than of course the available RAM. If you request 100 bits, that would require 13 bytes of memory, but right now storage is aligned to an integer so, the Mask will occupy 16 bytes of memory. I will probably remove this alignment later, it was supposed to improve performance but I don't believe it does, I haven't had time to test.