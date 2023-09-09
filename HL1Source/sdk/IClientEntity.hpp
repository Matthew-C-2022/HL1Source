#pragma once
#include "Vector.hpp"
#include "IClientNetworkable.hpp"
#include "IClientUnknown.hpp"
#include "IClientRenderable.hpp"
#include "IClientThinable.hpp"


class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	// Delete yourself.
	virtual void			Release(void) = 0;

	// Network origin + angles
	virtual const Vector& GetAbsOrigin(void) const = 0;
	virtual const Vector& GetAbsAngles(void) const = 0;

	virtual void* GetMouth(void) = 0;

	// Retrieve sound spatialization info for the specified sound on this entity
	// Return false to indicate sound is not audible
	virtual bool			GetSoundSpatialization(int& info) = 0;
};