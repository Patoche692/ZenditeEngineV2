#pragma once
#include <bitset>
#include <unordered_map>
#include <memory>
#include <set>
#include <vector>
#include <queue>
#include <array>

// Define a DEBUG flag - change this to 0 to disable asserts being run.
#define DEBUG_MODE 1

// Define your custom assertion macro
#if DEBUG_MODE
#include <cassert>
#define DEBUG_ASSERT(condition, message) assert((condition) && (message))
#else
#define CUSTOM_ASSERT(condition, message) ((void)0)
#endif

const std::uint32_t MAX_ENTITIES = 1000;
const std::uint8_t MAX_COMPONENTS = 32;

using Entity = std::uint32_t;
using ComponentBitsetPos = std::uint8_t;
using Signature = std::bitset<MAX_COMPONENTS>;

struct R_DataHandle
{
	//vec3 pos;
	//vec2 texCoords

	std::string name;
};