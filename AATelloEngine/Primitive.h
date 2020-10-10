#include "Application.h"
#include "glmath.h"
#include "Color.h"

class Primitive
{
public:
	Primitive(float*, std::size_t, unsigned int*, std::size_t, vec3& position, float angle, vec3& rotation, float red = 0.f, 
			  float green = 0.f, float blue = 1.f, float alpha = 1.f);
	~Primitive();

	void SetPosition(vec3& );
	void SetRotation(float angle, vec3& );
	void Draw() const;

private:
	unsigned int vertexId;
	unsigned int indexId;

	std::size_t vertexArrSize;
	std::size_t indexArrSize;

	mat4x4 transform;
	Color color;
};


class Cube : public Primitive
{
public:
	Cube(vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
};