#include "gfx-shader-compile.h"

GfxShaderCompile::GfxShaderCompile() 
{
}
GfxShaderCompile::~GfxShaderCompile()
{
}
void GfxShaderCompile::init()
{
}
std::vector<uint32_t> GfxShaderCompile::compile(const std::string &type,const std::string &param,const std::string &path)
{
    // //配置编译选项
    // shaderc::CompileOptions compileOptions;
    // // 设置目标环境
    // compileOptions.SetTargetEnvironment(
    //     shaderc_target_env_vulkan,
    //     shaderc_env_version_vulkan_1_2);
    // // 优化级别
    // compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
    // // 生成调试信息
    // compileOptions.SetGenerateDebugInfo();
    // // 添加宏定义
    // compileOptions.AddMacroDefinition("GL_SPIRV", "1");
    // compileOptions.AddMacroDefinition("VULKAN", "100");

    // // 编译着色器
    // shaderc::SpvCompilationResult result = this->_compiler.CompileGlslToSpv(
    //     path.c_str(), shaderc_vertex_shader, path.c_str(), compileOptions);
    // shaderc_compilation_status status = result.GetCompilationStatus();
    // if (status != shaderc_compilation_status_success)
    // {
    //     std::string errorMsg = "Shader compilation failed:\n";
    //     errorMsg += "File: " + path + "\n";
    //     errorMsg += "Error: " + result.GetErrorMessage();
    //     errorMsg += "Status: " + std::to_string(status);

    //     throw std::runtime_error(errorMsg);
    // }
    // // 输出警告信息
    // if (result.GetNumWarnings() > 0)
    // {
    //     std::cout << "Shader compilation warnings for " << path << ":\n"
    //               << result.GetErrorMessage() << std::endl;
    // }
    // std::vector<uint32_t> spirvCode(result.cbegin(), result.cend());
    // std::cout << "Successfully compiled " << path
    //           << " (" << spirvCode.size() << " SPIR-V words)" << std::endl;
    // return spirvCode;
}
