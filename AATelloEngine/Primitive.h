#include "Application.h"
#include "glmath.h"
#include "Color.h"

class Primitive
{
public:
	Primitive();
	Primitive(float*, std::size_t, unsigned int*, std::size_t, vec3& position, float angle, vec3& rotation, float red = 0.f, 
			  float green = 0.f, float blue = 1.f, float alpha = 1.f);
	Primitive(vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);

	~Primitive();

	void SetPosition(vec3& );
	void SetRotation(float angle, vec3& );
	void SetEscale(vec3& );

	virtual void Draw() const;

protected:
	mat4x4 transform;
	Color color;

	unsigned int vertexId;
	unsigned int indexId;

	std::size_t vertexArrSize;
	int indexArrSize;
};


class Cube : public Primitive
{
public:
	Cube(vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
};


class Sphere : public Primitive
{
public:
	Sphere(float radius, unsigned int rings, unsigned int sectors, vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
	void Draw() const;

private:
	unsigned int normalsId;
	std::size_t normalsArrSize;
};


class Plane : public Primitive
{
public:
	Plane(float x, float y, float z, float d);
	void Draw();

public:
	vec3 normal;
	float constant;
};