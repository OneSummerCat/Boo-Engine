#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
class Shader : public Asset
{
private:
	std::string _shaderData = "";
	std::string _shaderType = "";
protected:
	void _load() override;
public:
	Shader(const std::string key, const std::string type, const std::string path);
	virtual void destroy() override;
	~Shader() {}
};