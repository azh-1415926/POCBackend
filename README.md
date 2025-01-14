# About POCBackend

编译原理课程教学小助手后端部分(PrincipleOfCompilation Backend)

本项目为毕业设计，前端基于 Qt+WebAssembly 技术，[前端链接](https://github.com/azh-1415926/POCAssistant)，在浏览器运行 Qt 项目，同样也可作为客户端，后端基于 C++ Drogon 框架，[后端链接](https://github.com/azh-1415926/POCBackend)。

**请求接口**

大部分使用 Post 请求，少部分使用 Get 请求。
当前控制器有 User、Class、Course、Code、Quiz、Manager。
可分为用户、班级、课程、代码、题目、管理模块，均会返回result(string),info(string)。

**用户模块**
1. 用户登录
    * Url : `/User/login`
    * Method : Post
    * RequiredData : id(string)、password(string)
        ```json
        {
            "id": "2109104047",
            "password": "123456"
        }
        ```
    * Response : 管理员额外返回 token(string)
        ```json
        {
            "info":"\u767b\u9646\u6210\u529f",
            "result":"true"
        }
        ```

2. 获取用户信息
    * Url : `/User/info`
    * Method : Post
    * RequiredData : id(string)
    * Response : 正确返回 name(string)、role(int)，管理员额外返回 password(string)
        ```json
        {
            "info": "\u83b7\u53d6\u7528\u6237\u6210\u529f",
            "name": "azh",
            "result": "true",
            "role": 0
        }
        ```

**班级模块**
1. 班级信息
    * Url : `/Class/info`
    * Method : Post
    * RequiredData : id(string)
    * Response : 正确返回 name(string)、teacher_id(string)

**课程模块**
1. 获取大纲数据
    * Url : `/Course/getOutline`
    * Method : Post
    * Response : 返回 count(int)、以及大纲数据，如下所示:
        ```json
        {
            "0": {
                "0": "程序设计语言",
                "1": "形式语言",
                "count": 2,
                "value": "引论"
            },
            "count": 1,
            "info":"获取成功",
            "result": "true"
        }
        ```
2. 获取课程内容
    * Url : `/Course/getCourse`
    * Method : Post
    * RequiredData : chapter(string)、section(string)
        ```json
        {
            "chapter": "0",
            "section": "1"
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "content": "# \u7a0b\u5e8f\u8bbe\u8ba1\u8bed\u8a00\n\u8fd9\u662f\u6b63\u6587",
            "info": "\u83b7\u53d6\u6210\u529f",
            "name": "\u7a0b\u5e8f\u8bbe\u8ba1\u8bed\u8a00\u4e0e\u7f16\u8bd1\u7a0b\u5e8f\u7684\u8054\u7cfb",
            "result": "true"
        }
        ```
3. 更新课程
    * Url : `/Course/update`
    * Method : Post
    * RequiredData : chapter(string)、section(string)
        ```json
        {
            "chapter": 0,
            "content": "# 程序设计语言\n",
            "section": 1,
            "token": "f48637f8-4c9d-484a-9bbc-32fe59a252e7"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info":"\u66f4\u65b0\u6210\u529f",
            "result":"true"
        }
        ```
**代码模块**
1. 获取编译结果
    * Url : `/Code/compile`
    * Method : Post
    * RequiredData : code(string)、input(string)
        ```json
        {
            "code": "#include <stdio.h>\n\nint main()\n{\n\tprintf(\"Hello World!\\n\");\n\treturn 0;\n}",
            "input": ""
        }
        ```
    * Response
        * 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "info":"\u7f16\u8bd1\u6210\u529f",
            "output":"Hello World!\n",
            "result":"true"
        }
        ```
2. 获取实验
    * Url : `/Code/getExperiment`
    * Method : Post
    * RequiredData : studentId(string)
        ```json
        {
            "studentId": "2109104047"
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "count":0,"info":"\u83b7\u53d6\u6210\u529f",
            "result":"true"
        }
        ```
3. 提交实验
    * Url : `/Code/submit`
    * Method : Post
    * RequiredData : code(string)、experimentId(string)
        ```json
        {
            "code": "#include <stdio.h>\n\nint main()\n{\n\tprintf(\"Hello World!\\n\");\n\treturn 0;\n}",
            "experimentId": ""
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        // 暂无
        ```
**题目模块**
1. 获取题目
    * Url : `/Code/getquiz`
    * Method : Post
    * RequiredData : chapter(string)
        ```json
        {
            "chapter": 1
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "0": {
                "A": "\u7a0b\u5e8f\u8bbe\u8ba1",
                "B": "\u7a0b\u5e8f\u7ffb\u8bd1",
                "C": "\u7a0b\u5e8f\u8c03\u8bd5",
                "D": "\u6570\u636e\u5904\u7406",
                "answer": "B",
                "content": "\u7f16\u8bd1\u5668\u7684\u4e3b\u8981\u529f\u80fd\u662f\u4ec0\u4e48\uff1f",
                "type": 0
            },
            "1": {
                "A": "\u5c06\u6e90\u4ee3\u7801\u5206\u89e3\u6210\u591a\u4e2a\u8bed\u53e5",
                "B": "\u5c06\u6e90\u4ee3\u7801\u5206\u89e3\u6210\u591a\u4e2a\u5355\u8bcd",
                "C": "\u68c0\u67e5\u6e90\u4ee3\u7801\u7684\u8bed\u6cd5\u6b63\u786e\u6027",
                "D": "\u5c06\u5355\u8bcd\u8f6c\u6362\u4e3a\u76f8\u5e94\u7684\u8bed\u6cd5\u5355\u4f4d",
                "answer": "B",
                "content": "\u8bcd\u6cd5\u5206\u6790\u5668\u7684\u4e3b\u8981\u4efb\u52a1\u662f\u4ec0\u4e48\uff1f",
                "type": 0
            },
            "count": 2,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```
**管理模块**
1. 判断用户是否存在
    * Url : `/isValid?userId={1}`
    * Method : Get
    * RequiredData : userId(string)
    * Response : 返回数据，如下所示:
        ```json
        // 暂无
        ```
2. 添加用户
    * Url : `/Manager/addUser`
    * Method : Post
    * RequiredData : id(string)、name(string)、password(string)、role(int)、token(string)
        ```json
        {
            "id": "2109104039",
            "name": "czs",
            "password": "123456",
            "role": 0,
            "token": "f48637f8-4c9d-484a-9bbc-32fe59a252e7"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info": "\u6dfb\u52a0\u5931\u8d25\uff0c\u8be5id\u5df2\u5b58\u5728",
            "result": "false"
        }
        ```
3. 删除用户
    * Url : `/Manager/removeUser`
    * Method : Post
    * RequiredData : id(string)、token(string)
        ```json
        {
            "id": "2109104039",
            "token": "f48637f8-4c9d-484a-9bbc-32fe59a252e7"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info":"\u5220\u9664\u6210\u529f",
            "result":"true"
        }
        ```
4. 修改用户
    * Url : `/Manager/alterUser`
    * Method : Post
    * RequiredData : id(string)、name(string)、password(string)、role(int)、token(string)
        ```json
        {
            "id": "2109104047",
            "name": "azh",
            "password": "1",
            "role": 0,
            "token": "f48637f8-4c9d-484a-9bbc-32fe59a252e7"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info":"\u4fee\u6539\u6210\u529f",
            "result":"true"
        }
        ```
5. 添加班级
    * Url : `/Manager/addClass`
    * Method : Post
    * RequiredData : id(string)、name(string)teacher(string)、token(string)
        ```json
        {
            "id": "2109104047",
            "name": "azh",
            "teacherId": "0",
            "token": "f48637f8-4c9d-484a-9bbc-32fe59a252e7"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        // 暂无
        ```
6. 删除班级
    * Url : `/Manager/removeClass`
    * Method : Post
    * RequiredData : 暂无
        ```json
        // 暂无
        ```
    * Response : 返回数据，如下所示:
        ```json
        // 暂无
        ```
7. 修改班级
    * Url : `/Manager/alterClass`
    * Method : Post
    * RequiredData : 暂无
        ```json
        // 暂无
        ```
    * Response : 返回数据，如下所示:
        ```json
        // 暂无
        ```
8. 获取用户详细信息
    * Url : `/Manager/getUser`
    * Method : Post
    * RequiredData : 暂无
        ```json
        // 暂无
        ```
    * Response : 返回数据，如下所示:
        ```json
        // 暂无
        ```
9. 获取班级详细信息
    * Url : `/Manager/getClass`
    * Method : Post
    * RequiredData : 暂无
        ```json
        // 暂无
        ```
    * Response : 返回数据，如下所示:
        ```json
        // 暂无
        ```