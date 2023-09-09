#pragma once
#include "FNV.hpp"
#include <unordered_map>
#include "../sdk/ClientClass.hpp"
#include "../Interfaces/Interfaces.hpp"


void SetupNetvars();



void Dump(const char* Baseclass, RecvTable* table, std::uint32_t offset = 0);


inline std::unordered_map<uint32_t, uint32_t> offsets;


#define NETVAR(funcname, netvar, type) type& funcname() \
{ \
	static auto offset = offsets[fnv::HashConst(netvar)]; \
	return *reinterpret_cast<type*>(std::uint32_t(this) + offset); \
}