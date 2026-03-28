import cv2
import numpy as np
import sys
import os


def extract_pixels(image_path, max_size=100):
    """
    从图片中提取像素数据
    :param image_path: 图片路径
    :param max_size: 最大尺寸
    :return: 二值像素数据
    """
    # 读取图片
    img = cv2.imread(image_path)
    if img is None:
        print(f"无法读取图片: {image_path}")
        return None
    
    # 转换为灰度图
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    # 提取像素
    height, width = gray.shape
    
    # 自动确定采样间隔，确保最终数据不超过max_size
    max_dim = max(width, height)
    if max_dim <= max_size:
        interval = 1
    else:
        interval = max_dim // max_size
    
    extracted_width = (width // interval) + 1
    extracted_height = (height // interval) + 1
    
    # 缩放数据到max_size x max_size
    if extracted_width > max_size or extracted_height > max_size:
        if extracted_width > extracted_height:
            scale = max_size / extracted_width
        else:
            scale = max_size / extracted_height
        
        new_width = int(extracted_width * scale)
        new_height = int(extracted_height * scale)
    else:
        new_width = extracted_width
        new_height = extracted_height
    
    # 初始化结果数据
    result = [[0 for _ in range(new_width)] for _ in range(new_height)]
    
    # 填充数据
    for y in range(new_height):
        for x in range(new_width):
            # 计算原始图片中的位置
            orig_x = int(x * (extracted_width / new_width) * interval)
            orig_y = int(y * (extracted_height / new_height) * interval)
            
            # 确保位置在有效范围内
            orig_x = min(orig_x, width - 1)
            orig_y = min(orig_y, height - 1)
            
            # 检查像素值（低于128视为需要显示）
            if gray[orig_y, orig_x] < 128:
                result[y][x] = 1
    
    return result


def generate_cpp_code(data, variable_name="pixelData"):
    """
    生成C++代码
    :param data: 像素数据
    :param variable_name: 变量名
    :return: C++代码字符串
    """
    height = len(data)
    width = len(data[0]) if height > 0 else 0
    
    # 确保数据大小不超过100x100
    max_size = 100
    if width > max_size or height > max_size:
        print(f"警告: 数据大小 ({width}x{height}) 超过最大限制 ({max_size}x{max_size})，将进行缩放")
        if width > height:
            scale = max_size / width
        else:
            scale = max_size / height
        
        new_width = int(width * scale)
        new_height = int(height * scale)
        
        # 初始化缩放后的数据
        scaled_data = [[0 for _ in range(new_width)] for _ in range(new_height)]
        
        # 填充缩放后的数据
        for y in range(new_height):
            for x in range(new_width):
                orig_x = int(x * (width / new_width))
                orig_y = int(y * (height / new_height))
                orig_x = min(orig_x, width - 1)
                orig_y = min(orig_y, height - 1)
                scaled_data[y][x] = data[orig_y][orig_x]
        
        data = scaled_data
        height = new_height
        width = new_width
    
    code = f"const int {variable_name}Width = {width};\n"
    code += f"const int {variable_name}Height = {height};\n"
    code += f"int {variable_name}[{height}][{width}] = {{\n"
    
    for row in data:
        code += "    {" + ", ".join(map(str, row)) + "},\n"
    
    code += "};\n"
    return code


def main():
    if len(sys.argv) != 2:
        print("用法: python pixel_extractor.py <图片路径>")
        return
    
    image_path = sys.argv[1]
    data = extract_pixels(image_path)
    
    if data:
        cpp_code = generate_cpp_code(data)
        print("=== C++ 代码 ===")
        print(cpp_code)
        
        # 确保.temp目录存在于技能目录下
        script_dir = os.path.dirname(os.path.abspath(__file__))
        skill_dir = os.path.dirname(script_dir)
        temp_dir = os.path.join(skill_dir, ".temp")
        if not os.path.exists(temp_dir):
            os.makedirs(temp_dir)
        
        # 保存到文件
        output_path = os.path.join(temp_dir, "pixel_data.cpp")
        with open(output_path, "w") as f:
            f.write(cpp_code)
        print(f"\n代码已保存到 {output_path}")


if __name__ == "__main__":
    main()