#include "Config.h"
#include "parson/parson.h"

#include "Application.h"
#include "M_Editor.h"

#include <assert.h>

Config::Config() :
	rootValue(nullptr),
	node(nullptr)
{
	rootValue = json_value_init_object();
	node = json_value_get_object(rootValue);
}


Config::Config(char* buffer) :
	rootValue(nullptr),
	node(nullptr)
{
	rootValue = json_parse_string(buffer);

	if (rootValue != nullptr)
		node = json_value_get_object(rootValue);
}


Config::Config(json_object_t* obj) :
	rootValue(nullptr),
	node(obj)
{
}


Config::~Config()
{
	if (rootValue != nullptr)
		json_value_free(rootValue);
}


void Config::AppendNum(const char* name, double data)
{
	json_object_set_number(node, name, data);
}


void Config::AppendBool(const char* name, bool data)
{
	json_object_set_boolean(node, name, data);
}


void Config::AppendString(const char* name, const char* data)
{
	json_object_set_string(node, name, data);
}


Config Config::AppendNode(const char* name)
{
	json_object_set_value(node, name, json_value_init_object());

	return Config(json_object_get_object(node, name));
}


ConfigArray Config::AppendArray(const char* name)
{
	json_object_set_value(node, name, json_value_init_array());

	return ConfigArray(json_object_get_array(node, name));
}


double Config::GetNum(const char* name) const
{
	if (json_object_has_value_of_type(node, name, JSONNumber))
		return json_object_get_number(node, name);

	return 0;
}


bool Config::GetBool(const char* name) const
{
	if (json_object_has_value_of_type(node, name, JSONBoolean))
		return json_object_get_boolean(node, name);

	return false;
}


const char* Config::GetString(const char* name) const
{
	if (json_object_has_value_of_type(node, name, JSONString))
		return json_object_get_string(node, name);

	return "";
}


Config Config::GetNode(const char* name) const
{
	return Config(json_object_get_object(node, name));
}


ConfigArray Config::GetArray(const char* name) const
{
	return ConfigArray(json_object_get_array(node, name));
}


//Fills a buffer
unsigned int Config::Serialize(char** buffer) const
{
	unsigned int size = json_serialization_size_pretty(rootValue);

	*buffer = new char[size];
	json_serialize_to_buffer_pretty(rootValue, *buffer, size);
	return size;
}



ConfigArray::ConfigArray() : 
	arr(nullptr),
	size(0)
{
	arr = json_value_get_array(json_value_init_array());
}


ConfigArray::ConfigArray(json_array_t* arr) : 
	arr(arr),
	size(0)
{
	size = json_array_get_count(arr);
}


void ConfigArray::AppendNum(double data)
{
	json_array_append_number(arr, data);
	size++;
}


void ConfigArray::AppendBool(bool data)
{
	json_array_append_boolean(arr, data);
	size++;
}


void ConfigArray::AppendString(const char* data)
{
	json_array_append_string(arr, data);
	size++;
}


Config ConfigArray::AppendNode()
{
	json_array_append_value(arr, json_value_init_object());
	size++;

	return Config(json_array_get_object(arr, size - 1));
}


double ConfigArray::GetNum(unsigned int index) const
{
	if (index < size)
		return json_array_get_number(arr, index);

	else
	{
		App->editor->AddLog("[WARNING] Json index array invalid");
		return 0;
	}
}


bool ConfigArray::GetBool(unsigned int index) const
{
	if (index < size)
		return json_array_get_boolean(arr, index);

	else
	{
		App->editor->AddLog("[WARNING] Json index array invalid");
		return false;
	}
}


const char* ConfigArray::GetString(unsigned int index) const
{
	if (index < size)
		return json_array_get_string(arr, index);

	else
	{
		App->editor->AddLog("[WARNING] Json index array invalid");
		return "";
	}
}


Config ConfigArray::GetNode(unsigned int index) const
{
	if (index > size)
		assert("[WARNING] Json index array invalid");

	return json_array_get_object(arr, index);
}


unsigned int ConfigArray::GetSize()
{
	return size;
}