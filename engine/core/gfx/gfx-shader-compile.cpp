#include "gfx-shader-compile.h"

GfxShaderCompile::GfxShaderCompile() : _initialized(false)
{
}
GfxShaderCompile::~GfxShaderCompile()
{
}
void GfxShaderCompile::init()
{
    if (this->_initialized)
    {
        return;
    }
    try
    {
        // 设置目标环境
        this->_compileOptions.SetTargetEnvironment(
            shaderc_target_env_vulkan,
            shaderc_env_version_vulkan_1_2);
        // 优化级别
        this->_compileOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
        // 生成调试信息
        this->_compileOptions.SetGenerateDebugInfo();
        // 添加宏定义
        this->_compileOptions.AddMacroDefinition("GL_SPIRV", "1");
        this->_compileOptions.AddMacroDefinition("VULKAN", "100");

        this->_initialized = true;
        std::cout << "ShaderCompiler initialized successfully" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to initialize ShaderCompiler: " << e.what() << std::endl;
    }
}
std::vector<uint32_t> GfxShaderCompile::compile(const std::string &path)
{
    shaderc::SpvCompilationResult result = this->_compiler.CompileGlslToSpv(
        path.c_str(), shaderc_vertex_shader, path.c_str(), this->_compileOptions);
    shaderc_compilation_status status = result.GetCompilationStatus();
    if (status != shaderc_compilation_status_success)
    {
        std::string errorMsg = "Shader compilation failed:\n";
        errorMsg += "File: " + path + "\n";
        errorMsg += "Error: " + result.GetErrorMessage();
        errorMsg += "Status: " + std::to_string(status);

        throw std::runtime_error(errorMsg);
    }
    // 输出警告信息
    if (result.GetNumWarnings() > 0)
    {
        std::cout << "Shader compilation warnings for " << path << ":\n"
                  << result.GetErrorMessage() << std::endl;
    }
    std::vector<uint32_t> spirvCode(result.cbegin(), result.cend());
    std::cout << "Successfully compiled " << path
              << " (" << spirvCode.size() << " SPIR-V words)" << std::endl;
    return spirvCode;
}
