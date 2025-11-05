#pragma once
#include "asset.h"
#include <string>
#include <iostream>
class Texture : public Asset
{
private:
	int _width = 0;
    int _height = 0;
    int _channels = 0;
	std::vector<uint8_t> _pixelsVector;
	std::string _gfxTexture = "";
protected:
	void _load() override;
public:
	Texture(const std::string key, const std::string path);
	/**
	 * @brief 清除缓存
	 * 
	 */	
	void clearCache();
	~Texture() {}
};