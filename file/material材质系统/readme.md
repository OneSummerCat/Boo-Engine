// 初步格式制定
{
    "type": "material",
    "vert": "builtin::default.vert", // 编辑器使用glsl格式，构建时进行编译，编译完后使用spv格式
    "frag": "builtin::default.frag", // 编辑器使用glsl格式，构建时进行编译，编译完后使用spv格式
    //指定shader后，自动更具定义的pushConstant，生成对应的json数据结构
    //以及可以刷新pushConstant的值
    "pushConstant":{
        "key":{
            "type":"float",
            "value": [0.0]
        }
    },
    //指定shader后，自动更具定义的texture，生成对应的json数据结构
    //以及可以刷新texture的值
    "textures": {
        "key": "",
        ....
    },
    //渲染管线的状态
    "pipeline State":{
        
    }
}
//更具绑定的