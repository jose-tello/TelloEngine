#ifndef __CONFIG_H__
#define __CONFIG_H__

struct json_object_t;
struct json_value_t;
struct json_array_t;

struct ConfigArray;

struct Config
{
public:
	//When creating a new file
	Config();
	
	//To read a buffer
	Config(char* buffer);

	//To append
	Config(json_object_t*);
	
	~Config();

	void		AppendNum(const char* name, double data);
	void		AppendBool(const char* name, bool data);
	void		AppendString(const char* name, const char* data);
	Config		AppendNode(const char* name);
	ConfigArray AppendArray(const char* name);

	double		GetNum(const char* name) const;
	bool		GetBool(const char* name) const;
	const char* GetString(const char* name) const;
	Config		GetNode(const char* name) const;
	ConfigArray GetArray(const char* name) const;

	unsigned int Serialize(char** buffer) const;

private:
	json_value_t* rootValue = nullptr;
	json_object_t* node = nullptr;
};


struct ConfigArray
{
public:
	ConfigArray();
	ConfigArray(json_array_t*);

	void	AppendNum(double data);
	void	AppendBool(bool data);
	void	AppendString(const char* data);
	Config	AppendNode();

	double		GetNum(unsigned int index) const;
	bool		GetBool(unsigned int index) const;
	const char* GetString(unsigned int index) const;
	Config		GetNode(unsigned int index) const;

private:
	json_array_t* arr = nullptr;
	unsigned int size;

};
#endif // !__CONFIG_H__

