#pragma once

#include "GnmCommon.h"
#include "UtilFlag.h"

enum class GnmMemoryProtect : uint32_t
{
	GpuReadOnly,
	GpuReadWrite,
};

enum class GnmMemoryAccess : uint32_t
{
	None,  // Haven't been accessed yet.
	Read,
	Write
};

struct GnmMemoryRange
{
	void*  start = nullptr;
	size_t size  = 0;

	bool operator==(const GnmMemoryRange& other) const
	{
		return start == other.start &&
			   size == other.size;
	}
};

struct GnmMemoryHash
{
	std::size_t operator()(GnmMemoryRange const& range) const noexcept
	{
		// return reinterpret_cast<size_t>(range.start) << 32 | static_cast<size_t>(range.size);
		return reinterpret_cast<size_t>(range.start);
	}
};

/**
 * \brief GPU resource memory
 * 
 */
class GnmResourceMemory
{
	constexpr static uint32_t ForceUpdateCount = 10;

public:
	GnmResourceMemory();

	GnmResourceMemory(
		void*            start,
		uint32_t         size,
		GnmMemoryProtect prot);

	GnmResourceMemory(
		const GnmMemoryRange& range,
		GnmMemoryProtect      prot);

	~GnmResourceMemory();

	const GnmMemoryRange& range() const;

	void setRange(const GnmMemoryRange& range);

	GnmMemoryAccess access() const;

	void setAccess(GnmMemoryAccess access);

	GnmMemoryProtect protect() const;

	void setProtect(GnmMemoryProtect prot);

	bool pendingSync() const;

	void setPendingSync(bool sync);

	bool forceUpdate();

	void reset();

private:
	GnmMemoryRange   m_range;
	GnmMemoryProtect m_protect;
	GnmMemoryAccess  m_access      = GnmMemoryAccess::None;
	uint32_t         m_accessCount = 0;
	bool             m_pendingSync = false;
};
