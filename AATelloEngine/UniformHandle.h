#ifndef __UNIFORM_HANDLE_H__
#define __UNIFORM_HANDLE_H__

#include <string>

enum class VARIABLE_TYPE : int
{
	NONE = -1,
	
	BOOL,
	
	UINT,
	UINT_VEC2,
	UINT_VEC3,
	UINT_VEC4,
	
	INT,
	INT_VEC2,
	INT_VEC3,
	INT_VEC4,

	FLOAT,
	FLOAT_VEC2,
	FLOAT_VEC3,
	FLOAT_VEC4,

	FLOAT_MAT2,
	FLOAT_MAT3,
	FLOAT_MAT4,
};

struct UniformHandle
{
public:
	UniformHandle(std::string& name, VARIABLE_TYPE type);

	const char* GetName() const;
	VARIABLE_TYPE GetVartiableType() const;

	bool GetBool() const;

	unsigned int GetUint() const;
	void GetUintVec2(unsigned int*) const;
	void GetUintVec3(unsigned int*) const;
	void GetUintVec4(unsigned int*) const;

	int GetInt() const;
	void GetIntVec2(int*) const;
	void GetIntVec3(int*) const;
	void GetIntVec4(int*) const;

	float GetFloat() const;
	void GetFloatVec2(float*) const;
	void GetFloatVec3(float*) const;
	void GetFloatVec4(float*) const;

	void GetMat2(float*) const;
	void GetMat3(float*) const;
	void GetMat4(float*) const;

	void SetBool(bool);

	void SetUint(unsigned int);
	void SetUintVec2(unsigned int*);
	void SetUintVec3(unsigned int*);
	void SetUintVec4(unsigned int*);

	void SetInt(int);
	void SetIntVec2(int*);
	void SetIntVec3(int*);
	void SetIntVec4(int*);

	void SetFloat(float);
	void SetFloatVec2(float*);
	void SetFloatVec3(float*);
	void SetFloatVec4(float*);

	void SetMat2(float*);
	void SetMat3(float*);
	void SetMat4(float*);
	
private:
	std::string name;
	VARIABLE_TYPE type;

	union
	{
		bool boolValue;

		unsigned int uintValue;
		unsigned int uintVec2[2];
		unsigned int uintVec3[3];
		unsigned int uintVec4[4];

		int intValue;
		int intVec2[2];
		int intVec3[3];
		int intVec4[4];

		float floatValue;
		float floatVec2[2];
		float floatVec3[3];
		float floatVec4[4];

		float mat2[4];
		float mat3[9];
		float mat4[16];
	};
};
#endif // !__UNIFORM_HANDLE_H__

