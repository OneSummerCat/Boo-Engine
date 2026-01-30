import os
import glob
import platform


def find_source_files(directories):
    """
    在指定目录中查找所有.cpp和.h文件
    """
    print("directories:",directories)

    source_files = []
    for directory in directories:
        # 查找.cpp文件
        cpp_files = glob.glob(os.path.join(directory, '**/*.cpp'), recursive=True)
        # # 查找.h文件（可选，根据你的需求）
        # h_files = glob.glob(os.path.join(directory, '**/*.h'), recursive=True)
        
        source_files.extend(cpp_files)
        # source_files.extend(h_files)
    
    # 对结果排序以保证一致性
    return sorted(source_files)

def update_cmakelists(source_files):
    """
    更新CMakeLists.txt文件
    """
    # 读取原始CMakeLists.txt内容
    with open('CMakeLists.txt', 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # 找到ENGINE_SOURCES定义的位置
    start_idx = -1
    end_idx = -1
    for i, line in enumerate(lines):
        if line.strip().startswith('set(ENGINE_SOURCES'):
            start_idx = i
        if start_idx != -1 and line.strip().endswith(')'):
            end_idx = i
            break
    
    if start_idx == -1 or end_idx == -1:
        print("Error: Could not find ENGINE_SOURCES section in CMakeLists.txt")
        return
    
    # 生成新的源文件列表内容
    new_content = ['set(ENGINE_SOURCES\n']
    for file in source_files:
        cmake_path=file
        if platform.system() == 'Windows':
            cmake_path = cmake_path.replace('\\', '/')
        new_content.append(f'    {cmake_path}\n')
    new_content.append(')\n')
    
    # 替换原有内容
    updated_lines = lines[:start_idx] + new_content + lines[end_idx+1:]
    
    # 写回文件
    with open('CMakeLists.txt', 'w', encoding='utf-8') as f:
        f.writelines(updated_lines)
    
    print("CMakeLists.txt updated successfully")


if __name__ == "__main__":
    # 在这里添加你想要扫描的目录
    source_directories = [
        'engine',
        'editor',
    ]
    print(f"Foundsource files")
    # 查找源文件
    source_files = find_source_files(source_directories)
    print(f"Found {len(source_files)} source files")
    
    # 更新CMakeLists.txt
    update_cmakelists(source_files)