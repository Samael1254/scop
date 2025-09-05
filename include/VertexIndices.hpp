#include <cstdint>
#include <functional>

struct VertexIndices
{
	VertexIndices();
	VertexIndices(int positionID);
	VertexIndices(int positionID, int textureID, int normalID);

	bool operator==(const VertexIndices &other) const;

	int positionID;
	int textureID;
	int normalID;
};

template <>
struct std::hash<VertexIndices>
{
	uint64_t operator()(const VertexIndices &vi) const noexcept;
};
