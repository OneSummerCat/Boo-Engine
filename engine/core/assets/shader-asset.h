#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

namespace Boo
{

	class ShaderAsset : public Asset
	{
	private:
		std::string _glslData = "";
		std::vector<uint32_t> _spirvData;

	public:
		ShaderAsset();
		ShaderAsset(const std::string uuid);
		void create(std::string path) override;
		void create(const uint32_t *data);
		void destroy() override;
		~ShaderAsset() {}
	};

} // namespace Boo