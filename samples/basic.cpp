#include <iostream>
#define METAENUMERATOR_NAMESPACE MyRootNamespace
#include "MetaEnumerator.hpp"


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
	static constexpr const bool logic_operators = true;
	static constexpr const bool string_operators = true;
	static constexpr const TargetType MAX_VALUE = TargetType::MAX;
	
	static constexpr const EnumEntry enum_entries[]{
		{ TargetType::ENEMY_ALIVE, "ENEMY_ALIVE" },
		{ TargetType::ENEMY_CORPSE, "ENEMY_CORPSE" },
		{ TargetType::ENEMY_SPOT, "ENEMY_SPOT" },
		{ TargetType::ALLY_ALIVE, "ALLY_ALIVE" },
		{ TargetType::ALLY_CORPSE, "ALLY_CORPSE" },
		{ TargetType::ALLY_SPOT, "ALLY_SPOT" }
	};
};

constexpr EnumeratorMeta<TargetType>::EnumEntry EnumeratorMeta<TargetType>::enum_entries[];

}


int main(int argc, char *argv[])
{
	using namespace MyRootNamespace;
	
	{
		// creates a mask named accepted_targets
		auto accepted_targets = TargetType::ENEMY_ALIVE | TargetType::ALLY_ALIVE;
		std::cout << "Accepted Targets: " << accepted_targets << "\n";
		// prints: Accepted Targets: ENEMY_ALIVE, ALLY_ALIVE	
	}
	
	{
		// creates a mask named accepted_targets
		auto accepted_targets = TargetType::ENEMY_ALIVE | TargetType::ALLY_ALIVE;
		if (accepted_targets.has(TargetType::ENEMY_ALIVE))
			std::cout << "Looking for a live enemy...\n";
		// prints: Looking for a live enemy...
	}
	
	{
		// support for ostream operator<<
		auto target = TargetType::ALLY_CORPSE;
		std::cout << "We got this target: " << target;
		// prints: We got this target: ALLY_CORPSE
	}
	
	std::cout << "\n";
}