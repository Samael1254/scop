#include "VertexIndices.hpp"
#include <cstdint>
#include <functional>

VertexIndices::VertexIndices() : positionID(-1), textureID(-1), normalID(-1) {};
VertexIndices::VertexIndices(int positionID) : positionID(positionID), textureID(-1), normalID(-1) {};
VertexIndices::VertexIndices(int positionID, int textureID, int normalID)
    : positionID(positionID), textureID(textureID), normalID(normalID) {};

bool VertexIndices::operator==(const VertexIndices &other) const
{
	return positionID == other.positionID && textureID == other.textureID && normalID == other.normalID;
}

uint64_t std::hash<VertexIndices>::operator()(const VertexIndices &vi) const noexcept
{
	const int values[3] = {vi.positionID, vi.textureID, vi.normalID};

	uint64_t hash = 0xcbf29ce484222325;
	for (unsigned int i = 0; i < 3; ++i)
	{
		const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&values[i]);
		for (unsigned int j = 0; j < sizeof(int); ++j)
		{
			hash ^= bytes[j];
			hash *= 0x100000001b3;
		}
	}
	return hash;
}
