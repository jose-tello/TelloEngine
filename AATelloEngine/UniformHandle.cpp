#include "UniformHandle.h"


UniformHandle::UniformHandle(std::string& name, VARIABLE_TYPE type) :
	name(name),
	type(type)

{
	memset(mat4, 0, sizeof(mat4));
}


const char* UniformHandle::GetName() const
{
	return name.c_str();
}


VARIABLE_TYPE UniformHandle::GetVartiableType() const
{
	return type;
}


bool UniformHandle::GetBool() const
{
	return boolValue;
}


unsigned int UniformHandle::GetUint() const
{
	return uintValue;
}


void UniformHandle::GetUintVec2(unsigned int* aux) const
{
	aux[0] = uintVec2[0];
	aux[1] = uintVec2[1];
}


void UniformHandle::GetUintVec3(unsigned int* aux) const
{
	aux[0] = uintVec3[0];
	aux[1] = uintVec3[1];
	aux[2] = uintVec3[2];
}


void UniformHandle::GetUintVec4(unsigned int* aux) const
{
	aux[0] = uintVec4[0];
	aux[1] = uintVec4[1];
	aux[2] = uintVec4[2];
	aux[3] = uintVec4[3];
}


int UniformHandle::GetInt() const
{
	return intValue;
}


void UniformHandle::GetIntVec2(int* aux) const
{
	aux[0] = intVec2[0];
	aux[1] = intVec2[1];
}


void UniformHandle::GetIntVec3(int* aux) const
{
	aux[0] = intVec3[0];
	aux[1] = intVec3[1];
	aux[2] = intVec3[2];
}


void UniformHandle::GetIntVec4(int* aux) const
{
	aux[0] = intVec4[0];
	aux[1] = intVec4[1];
	aux[2] = intVec4[2];
	aux[3] = intVec4[3];
}


float UniformHandle::GetFloat() const
{
	return floatValue;
}


void UniformHandle::GetFloatVec2(float* aux) const
{
	aux[0] = floatVec2[0];
	aux[1] = floatVec2[1];
}


void UniformHandle::GetFloatVec3(float* aux) const
{
	aux[0] = floatVec3[0];
	aux[1] = floatVec3[1];
	aux[2] = floatVec3[2];
}


void UniformHandle::GetFloatVec4(float* aux) const
{
	aux[0] = floatVec4[0];
	aux[1] = floatVec4[1];
	aux[2] = floatVec4[2];
	aux[3] = floatVec4[3];
}


void UniformHandle::GetMat2(float* aux) const
{
	aux[0] = mat2[0];
	aux[1] = mat2[1];
	aux[2] = mat2[2];
	aux[3] = mat2[3];
}


void UniformHandle::GetMat3(float* aux) const
{
	aux[0] = mat3[0];
	aux[1] = mat3[1];
	aux[2] = mat3[2];
	aux[3] = mat3[3];
	aux[4] = mat3[4];
	aux[5] = mat3[5];
	aux[6] = mat3[6];
	aux[7] = mat3[7];
	aux[8] = mat3[8];
}


void UniformHandle::GetMat4(float* aux) const
{
	aux[0] = mat4[0];
	aux[1] = mat4[1];
	aux[2] = mat4[2];
	aux[3] = mat4[3];
	aux[4] = mat4[4];
	aux[5] = mat4[5];
	aux[6] = mat4[6];
	aux[7] = mat4[7];
	aux[8] = mat4[8];
	aux[9] = mat4[9];
	aux[10] = mat4[10];
	aux[11] = mat4[11];
	aux[12] = mat4[12];
	aux[13] = mat4[13];
	aux[14] = mat4[14];
	aux[15] = mat4[15];
}


void UniformHandle::SetBool(bool value)
{
	boolValue = value;
}


void UniformHandle::SetUint(unsigned int value)
{
	uintValue = value;
}


void UniformHandle::SetUintVec2(unsigned int* value)
{
	uintVec2[0] = value[0];
	uintVec2[1] = value[1];
}


void UniformHandle::SetUintVec3(unsigned int* value)
{
	uintVec3[0] = value[0];
	uintVec3[1] = value[1];
	uintVec3[2] = value[2];
}


void UniformHandle::SetUintVec4(unsigned int* value)
{
	uintVec4[0] = value[0];
	uintVec4[1] = value[1];
	uintVec4[2] = value[2];
	uintVec4[3] = value[3];
}


void UniformHandle::SetInt(int value)
{
	intValue = value;
}


void UniformHandle::SetIntVec2(int* value)
{
	intVec2[0] = value[0];
	intVec2[1] = value[1];
}


void UniformHandle::SetIntVec3(int* value)
{
	uintVec3[0] = value[0];
	uintVec3[1] = value[1];
	uintVec3[2] = value[2];
}


void UniformHandle::SetIntVec4(int* value)
{
	uintVec4[0] = value[0];
	uintVec4[1] = value[1];
	uintVec4[2] = value[2];
	uintVec4[3] = value[3];
}


void UniformHandle::SetFloat(float value)
{
	floatValue = value;
}


void UniformHandle::SetFloatVec2(float* value)
{
	floatVec2[0] = value[0];
	floatVec2[1] = value[1];
}


void UniformHandle::SetFloatVec3(float* value)
{
	floatVec3[0] = value[0];
	floatVec3[1] = value[1];
	floatVec3[2] = value[2];
}


void UniformHandle::SetFloatVec4(float* value)
{
	floatVec4[0] = value[0];
	floatVec4[1] = value[1];
	floatVec4[2] = value[2];
	floatVec4[3] = value[3];
}


void UniformHandle::SetMat2(float* value)
{
	mat2[0] = value[0];
	mat2[1] = value[1];
	mat2[2] = value[2];
	mat2[3] = value[3];
}


void UniformHandle::SetMat3(float* value)
{
	mat3[0] = value[0];
	mat3[1] = value[1];
	mat3[2] = value[2];
	mat3[3] = value[3];
	mat3[4] = value[4];
	mat3[5] = value[5];
	mat3[6] = value[6];
	mat3[7] = value[7];
	mat3[8] = value[8];
}


void UniformHandle::SetMat4(float* value)
{
	mat4[0] = value[0];
	mat4[1] = value[1];
	mat4[2] = value[2];
	mat4[3] = value[3];
	mat4[4] = value[4];
	mat4[5] = value[5];
	mat4[6] = value[6];
	mat4[7] = value[7];
	mat4[8] = value[8];
	mat4[9] = value[9];
	mat4[10] = value[10];
	mat4[11] = value[11];
	mat4[12] = value[12];
	mat4[13] = value[13];
	mat4[14] = value[14];
	mat4[15] = value[15];
}