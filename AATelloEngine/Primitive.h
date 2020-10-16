#include "Application.h"
#include "glmath.h"

struct Mesh;

class Primitive
{
public:
	Primitive();
	Primitive(float*, std::size_t, unsigned int*, std::size_t, vec3& position, float angle, vec3& rotation, float red = 0.f, 
			  float green = 0.f, float blue = 1.f, float alpha = 1.f);

	virtual ~Primitive();

	void SetPosition(vec3& );
	void SetRotation(float angle, vec3& );
	void SetEscale(vec3& );

	Mesh* GetMesh() const;

protected:
	Mesh* mesh;
};


class Cube : public Primitive
{
public:
	Cube(vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
	~Cube();
};


class Piramid : public Primitive
{
public:
	Piramid(vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
	~Piramid();
};


class Sphere : public Primitive
{
public:
	Sphere(float radius, unsigned int rings, unsigned int sectors, vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
	~Sphere();
};


class Cilinder : public Primitive
{
public:
	Cilinder(unsigned int sectorCount, float height, float radius, vec3& position, float angle, vec3& rotation, float red = 0.f, float green = 0.f, float blue = 1.f, float alpha = 1.f);
	~Cilinder();
};


class Plane
{
public:
	Plane(float x, float y, float z, float d);
	~Plane();
	void Draw() const;

public:
	vec3 normal;
	float constant;
};