# About POCBackend

编译原理课程教学小助手后端部分(PrincipleOfCompilation Backend)

本项目为毕业设计，前端基于 Qt+WebAssembly 技术，[前端链接](https://github.com/azh-1415926/POCAssistant)，在浏览器运行 Qt 项目，同样也可作为客户端，后端基于 C++ Drogon 框架，[后端链接](https://github.com/azh-1415926/POCBackend)。

**请求接口**

大部分使用 Post 请求，少部分使用 Get 请求。
当前控制器有 User、Class、Course、Code、Quiz、Manager。
可分为用户、班级、课程、代码、题目、管理模块，均会返回result(string),info(string)。

**用户模块**

文档里已有接口：/User/login、/User/info、/User/getUnallocatedStudent

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
3. 获取未分配班级的用户
    * Url : `/User/getUnallocatedStudent`
    * Method : Post
    * RequiredData : none
        ```json
        {
            "token": "07d80879-010c-4793-aa8c-9305f5ac248d"
        }
        ```
    * Response : 返回全部未分配班级的用户id
        ```json
        {
            "0":"2109104047",
            "count":1,
            "info":"\u83b7\u53d6\u6210\u529f",
            "result":"true"
        }
        ```


**班级模块**

文档里已有接口：/Class/info、/Class/allocate、/Class/getClassByTeacher、/Class/getStudentByClass

1. 班级信息
    * Url : `/Class/info`
    * Method : Post
    * RequiredData : id(string)
        ```json
        {
            "id": "21091040",
            "token": "afe2a8de-02c8-41ea-b2e7-ae3820e32311"
        }
        ```
    * Response : 正确返回 name(string)、teacherId(string)
        ```json
        {
            "info": "\u83b7\u53d6\u6210\u529f",
            "name": "21\u8f6f\u4ef62\u73ed",
            "result": "true",
            "teacherId": "20250114"
        }
        ```
2. 班级分配
* Url : `/Class/allocate`
    * Method : Post
    * RequiredData : id(string),userId(string)
        ```json
        {
            "id": "21091040",
            "token": "07d80879-010c-4793-aa8c-9305f5ac248d",
            "userId": "2109104047"
        }
        ```
    * Response : 正确返回
        ```json
        {
            "info": "\u5206\u914d\u5931\u8d25\uff0c\u8be5\u7528\u6237\u5df2\u5206\u914d\u8fc7\u73ed\u7ea7",
            "result": "false"
        }
        ```
3. 获取教师管理的班级
* Url : `/Class/getClassByTeacher`
    * Method : Post
    * RequiredData : id(string),userId(string)
        ```json
        {
            "id": "20250115"
        }
        ```
    * Response : 正确返回
        ```json
        {
            "0": {
                "id": "21091040",
                "name": "21\u8f6f\u4ef62\u73ed"
            },
            "count": 1,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```

4. 获取班级内的所有学生
* Url : `/Class/getStudentByClass`
    * Method : Post
    * RequiredData : id(string),userId(string)
        ```json
        {
            "id": "21091040"
        }
        ```
    * Response : 正确返回
        ```json
                {
            "0": {
                "id": "2109104039",
                "name": "czs"
            },
            "1": {
                "id": "2109104047",
                "name": "azh"
            },
            "count": 2,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```

**课程模块**

文档里已有接口：/Course/getOutline、/Course/getCourse、/Course/update

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

文档里已有接口：/Code/compile、/Code/releaseExperiment、/Code/getUnfinishedExperiment、/Code/getExperiment、/Code/submit、/Code/getExperimentByStudent、/Code/submitScore

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
2. 发布实验
    * Url : `/Code/releaseExperiment`
    * Method : Post
    * RequiredData : studentId(string)
        ```json
        {
            "classId": "21091040",
            "content": "语法分析",
            "name": "第二章 语法分析"
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "info":"\u6dfb\u52a0\u6210\u529f",
            "result":"true"
        }
        ```
3. 获取未完成的实验列表
    * Url : `/Code/getUnfinishedExperiment`
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
            "0": {
                "content": "\u7f16\u5199\u4e00\u6bb5\u7a0b\u5e8f\uff0c\u8fd0\u884c\u67e5\u770b\u7ed3\u679c",
                "createTime": "2025-02-10 19:20:09",
                "experimentId": "1",
                "name": "\u7b2c\u4e00\u7ae0-\u719f\u6089 C \u7f16\u8bd1\u73af\u5883"
            },
            "count": 1,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```
4. 获取已完成实验的记录
    * Url : `/Code/getExperiment`
    * Method : Post
    * RequiredData : studentId(string)
        ```json
        {
            "userId": "2109104047"
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "0": {
                "code": "/* Input two numbers, output the product */\n#include <stdio.h>\nint main()\n{\n\tint x,y,m;\n\tprintf(\"Please input x and y\n\");\n\tscanf(\"%d%d\",&x,&y);\n\tm=x*y;\n\tprintf(\"%d * %d = %d\n\",x,y,m);\n\n\treturn 0;\n}",
                "content": "\u7f16\u5199\u4e00\u6bb5\u7a0b\u5e8f\uff0c\u8fd0\u884c\u67e5\u770b\u7ed3\u679c",
                "createTime": "2025-02-10 19:20:09",
                "experimentId": "1",
                "finishTime": "2025-05-02 15:48:31",
                "id": 13,
                "isCorrect": 1,
                "name": "\u7b2c\u4e00\u7ae0-\u719f\u6089 C \u7f16\u8bd1\u73af\u5883",
                "score": 100,
                "studentId": "2109104047"
            },
            "1": {
                "code": "/* Input two numbers, output the product */\n#include <stdio.h>\nint main()\n{\n\tint x,y,m;\n\tprintf(\"Please input x and y\n\");\n\tscanf(\"%d%d\",&x,&y);\n\tm=x*y;\n\tprintf(\"%d * %d = %d\n\",x,y,m);\n\n\treturn 0;\n}",
                "content": "\u8bed\u6cd5\u5206\u6790",
                "createTime": "2025-02-10 20:47:52",
                "experimentId": "2",
                "finishTime": "2025-05-02 15:48:32",
                "id": 14,
                "isCorrect": 1,
                "name": "\u7b2c\u4e8c\u7ae0 \u8bed\u6cd5\u5206\u6790",
                "score": 100,
                "studentId": "2109104047"
            },
            "2": {
                "code": "/* Input two numbers, output the product */\n#include <stdio.h>\nint main()\n{\n\tint x,y,m;\n\tprintf(\"Please input x and y\n\");\n\tscanf(\"%d%d\",&x,&y);\n\tm=x*y;\n\tprintf(\"%d * %d = %d\n\",x,y,m);\n\n\treturn 0;\n}",
                "content": "\u8bcd\u6cd5\u5206\u6790",
                "createTime": "2025-02-13 11:24:57",
                "experimentId": "3",
                "finishTime": "2025-05-02 15:48:32",
                "id": 15,
                "isCorrect": 1,
                "name": "\u7b2c\u4e09\u6b21\u5b9e\u9a8c \u8bcd\u6cd5\u5206\u6790",
                "score": 100,
                "studentId": "2109104047"
            },
            "3": {
                "code": "/* Input two numbers, output the product */\n#include <stdio.h>\nint main()\n{\n\tint x,y,m;\n\tprintf(\"Please input x and y\n\");\n\tscanf(\"%d%d\",&x,&y);\n\tm=x*y;\n\tprintf(\"%d * %d = %d\n\",x,y,m);\n\n\treturn 0;\n}",
                "content": "\u7406\u89e3\u5e76\u638c\u63e1\u5c5e\u6027\u6587\u6cd5",
                "createTime": "2025-02-26 19:43:32",
                "experimentId": "4",
                "finishTime": "2025-05-02 15:48:32",
                "id": 16,
                "isCorrect": 1,
                "name": "\u7b2c\u56db\u7ae0 \u5c5e\u6027\u6587\u6cd5",
                "score": 100,
                "studentId": "2109104047"
            },
            "count": 4,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```
5. 提交实验
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
6. 获取学生完成且未批改的实验
    * Url : `/Code/getExperimentByStudent`
    * Method : Post
    * RequiredData : studentId(string)
        ```json
        {
            "id": "2109104047"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "0": {
                "code": "#include <stdio.h>\n\nint main()\n{\n\tprintf(\"Hello World!\n\");\n\treturn 0;\n}",
                "content": "\u7f16\u5199\u4e00\u6bb5\u7a0b\u5e8f\uff0c\u8fd0\u884c\u67e5\u770b\u7ed3\u679c",
                "id": "1",
                "name": "\u7b2c\u4e00\u7ae0-\u719f\u6089 C \u7f16\u8bd1\u73af\u5883"
            },
            "1": {
                "code": "#include <stdio.h>\n\nint main()\n{\n\tprintf(\"Hello World!\n\");\n\treturn 0;\n}",
                "content": "\u8bed\u6cd5\u5206\u6790",
                "id": "2",
                "name": "\u7b2c\u4e8c\u7ae0 \u8bed\u6cd5\u5206\u6790"
            },
            "2": {
                "code": "#include <stdio.h>\n\nint main()\n{\n\tprintf(\"Hello World!\n\");\n\treturn 0;\n}",
                "content": "\u8bcd\u6cd5\u5206\u6790",
                "id": "3",
                "name": "\u7b2c\u4e09\u6b21\u5b9e\u9a8c \u8bcd\u6cd5\u5206\u6790"
            },
            "3": {
                "code": "#include <stdio.h>\n\nint main()\n{\n\tprintf(\"Hello World!\n\");\n\treturn 0;\n}",
                "content": "\u7406\u89e3\u5e76\u638c\u63e1\u5c5e\u6027\u6587\u6cd5",
                "id": "4",
                "name": "\u7b2c\u56db\u7ae0 \u5c5e\u6027\u6587\u6cd5"
            },
            "4": {
                "code": "#include <stdio.h>\n\nint main()\n{\n\tprintf(\"Hello World!\n\");\n\treturn 0;\n}",
                "content": "\u7406\u89e3\u5e76\u638c\u63e1\u5c5e\u6027\u6587\u6cd5",
                "id": "5",
                "name": "\u7b2c\u56db\u7ae0 \u5c5e\u6027\u6587\u6cd5"
            },
            "count": 5,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```

7. 提交批改分数
    * Url : `/Code/submitScore`
    * Method : Post
    * RequiredData : studentId(string)、experimentId(string)、score(int)
        ```json
        {
            "experimentId": "1",
            "score": 99,
            "studentId": "2109104047"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info":"\u63d0\u4ea4\u6210\u529f",
            "result":"true"
        }
        ```

**题目模块**

文档里已有接口：/Quiz/getquiz、/Quiz/getCollectedQuiz、/Quiz/collectQuiz、/Quiz/uncollectQuiz、/Quiz/getChapter

1. 获取题目
    * Url : `/Quiz/getquiz`
    * Method : Post
    * RequiredData : chapter(string)、studentId(string)
        ```json
        {
            "chapter": 1,
            "studentId": "2109104047"
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
                // "answer": "B",
                "collected": 1,
                "content": "\u7f16\u8bd1\u5668\u7684\u4e3b\u8981\u529f\u80fd\u662f\u4ec0\u4e48\uff1f",
                "type": 0,
                ""
            },
            "1": {
                "A": "\u5c06\u6e90\u4ee3\u7801\u5206\u89e3\u6210\u591a\u4e2a\u8bed\u53e5",
                "B": "\u5c06\u6e90\u4ee3\u7801\u5206\u89e3\u6210\u591a\u4e2a\u5355\u8bcd",
                "C": "\u68c0\u67e5\u6e90\u4ee3\u7801\u7684\u8bed\u6cd5\u6b63\u786e\u6027",
                "D": "\u5c06\u5355\u8bcd\u8f6c\u6362\u4e3a\u76f8\u5e94\u7684\u8bed\u6cd5\u5355\u4f4d",
                // "answer": "B",
                "collected": 0,
                "content": "\u8bcd\u6cd5\u5206\u6790\u5668\u7684\u4e3b\u8981\u4efb\u52a1\u662f\u4ec0\u4e48\uff1f",
                "type": 0
            },
            "count": 2,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```
2. 获取收藏题目
    * Url : `/Quiz/getCollectedQuiz`
    * Method : Post
    * RequiredData : chapter(string)、studentId(string)
        ```json
        {
            "chapter": 1,
            "studentId": "2109104047"
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
                // "answer": "B",
                "collected": 1,
                "content": "\u7f16\u8bd1\u5668\u7684\u4e3b\u8981\u529f\u80fd\u662f\u4ec0\u4e48\uff1f",
                "type": 0
            },
            "count": 1,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true"
        }
        ```
3. 收藏题目
    * Url : `/Quiz/collectQuiz`
    * Method : Post
    * RequiredData : chapter(string)、studentId(string)、data(array)
        ```json
        {
            "chapter": 1,
            "data": [
                0,
                2
            ],
            "studentId": "2109104047"
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "info":"\u6267\u884c\u6210\u529f",
            "result":"true"
        }
        ```
4. 收藏题目
    * Url : `/Quiz/uncollectQuiz`
    * Method : Post
    * RequiredData : chapter(string)、studentId(string)、data(array)
        ```json
        {
            "chapter": 1,
            "data": [
            ],
            "studentId": "2109104047"
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "info":"\u6267\u884c\u6210\u529f",
            "result":"true"
        }
        ```
4. 获取最高章节
    * Url : `/Quiz/getChapter`
    * Method : Post
    * RequiredData : None
        ```json
        {
        }
        ```
    * Response : 返回 name(string)、content(string)，如下所示:
        ```json
        {
            "chapter":1,"info":"\u83b7\u53d6\u6210\u529f",
            "result":"true"
        }
        ```
**管理模块**

文档里已有接口：/Manager/isValid?userId={1}、/Manager/addUser、/Manager/removeUser、/Manager/alterUser、/Manager/addClass、/Manager/removeClass、/Manager/alterClass、/Manager/getUser、/Manager/getClass、/Manager/statUser、/Manager/statClass、/Manager/statQuiz、/Manager/userBatch、/Manager/classBatch

1. 判断用户是否存在
    * Url : `/Manager/isValid?userId={1}`
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
            "id": "21091040",
            "name": "21软件2班",
            "teacherId": "20250114",
            "token": "f93decef-71fa-4c55-a651-93b634928271"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info":"\u6dfb\u52a0\u6210\u529f",
            "result":"true"
        }
        ```
6. 删除班级
    * Url : `/Manager/removeClass`
    * Method : Post
    * RequiredData : 暂无
        ```json
        {
            "id": "21091040",
            "token": "afe2a8de-02c8-41ea-b2e7-ae3820e32311"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info":"\u5220\u9664\u6210\u529f",
            "result":"true"
        }
        ```
7. 修改班级
    * Url : `/Manager/alterClass`
    * Method : Post
    * RequiredData : 暂无
        ```json
        {
            "id": "21091040",
            "name": "21软件2班",
            "teacherId": "20250114",
            "token": "afe2a8de-02c8-41ea-b2e7-ae3820e32311"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "info": "\u4fee\u6539\u5931\u8d25\uff0c\u8bf7\u8f93\u5165\u4efb\u8bfe\u6559\u5e08id\uff0c\u8bf7\u6c42\u683c\u5f0f\u6709\u8bef",
            "result": "false"
        }
        ```
8. 获取用户详细信息
    * Url : `/Manager/getUser`
    * Method : Post
    * RequiredData : token(string)
        ```json
        {
            "token": "e940aaa4-16f0-4d49-b62d-53a3652b1d02"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "0": {
                "0": "\u59d3\u540d",
                "1": "\u7f16\u53f7",
                "2": "\u804c\u4f4d",
                "3": "\u73ed\u7ea7\u540d",
                "4": "\u6700\u540e\u767b\u5f55\u65f6\u95f4"
            },
            "1": {
                "0": "czs",
                "1": "2109104039",
                "2": "\u5b66\u751f",
                "3": "21\u8f6f\u4ef62\u73ed",
                "4": "2025-02-05 16:17:16"
            },
            "2": {
                "0": "azh",
                "1": "2109104047",
                "2": "\u5b66\u751f",
                "3": "21\u8f6f\u4ef62\u73ed",
                "4": "2024-11-18 09:19:44"
            },
            "3": {
                "0": "hhhhhh",
                "1": "1233",
                "2": "\u6559\u5e08",
                "3": "",
                "4": "2024-12-27 15:29:29"
            },
            "4": {
                "0": "zjt",
                "1": "20250114",
                "2": "\u6559\u5e08",
                "3": "",
                "4": "2025-01-14 17:00:07"
            },
            "5": {
                "0": "gc",
                "1": "20250115",
                "2": "\u6559\u5e08",
                "3": "",
                "4": "2025-01-15 10:27:35"
            },
            "col": 5,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true",
            "row": 6
        }
        ```
9. 获取班级详细信息
    * Url : `/Manager/getClass`
    * Method : Post
    * RequiredData : token(string)
        ```json
        {
            "token": "e940aaa4-16f0-4d49-b62d-53a3652b1d02"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "0": {
                "0": "\u540d\u79f0",
                "1": "\u7f16\u53f7",
                "2": "\u4efb\u8bfe\u6559\u5e08",
                "3": "\u5b66\u751f\u603b\u6570"
            },
            "1": {
                "0": "21\u8f6f\u4ef62\u73ed",
                "1": "21091040",
                "2": "gc",
                "3": "2"
            },
            "col": 4,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true",
            "row": 2
        }
        ```

10. 统计用户信息
    * Url : `/Manager/statUser`
    * Method : Post
    * RequiredData : token(string)
        ```json
        {
            "token": "e940aaa4-16f0-4d49-b62d-53a3652b1d02"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "0": {
                "0": "\u540d\u79f0",
                "1": "\u7edf\u8ba1"
            },
            "1": {
                "0": "\u7528\u6237\u603b\u6570",
                "1": "6"
            },
            "2": {
                "0": "\u5b66\u751f\u603b\u6570",
                "1": "2"
            },
            "3": {
                "0": "\u6559\u5e08\u603b\u6570",
                "1": "3"
            },
            "4": {
                "0": "\u6700\u8fd1\u767b\u5f55\u7528\u6237",
                "1": "gc"
            },
            "5": {
                "0": "\u4e0d\u6d3b\u8dc3\u7528\u6237",
                "1": "hhhhhh"
            },
            "col": 2,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true",
            "row": 6
        }
        ```

11. 统计班级信息
    * Url : `/Manager/statClass`
    * Method : Post
    * RequiredData : token(string)
        ```json
        {
            "token": "e940aaa4-16f0-4d49-b62d-53a3652b1d02"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
        {
            "0": {
                "0": "\u540d\u79f0",
                "1": "\u7edf\u8ba1"
            },
            "1": {
                "0": "\u73ed\u7ea7\u603b\u6570",
                "1": "1"
            },
            "2": {
                "0": "\u7ba1\u7406\u73ed\u7ea7\u6700\u591a\u7684\u6559\u5e08",
                "1": "20250115"
            },
            "3": {
                "0": "\u4eba\u6570\u6700\u591a\u7684\u73ed\u7ea7",
                "1": "21091040"
            },
            "4": {
                "0": "\u4eba\u6570\u6700\u5c11\u7684\u73ed\u7ea7",
                "1": "21091040"
            },
            "col": 2,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true",
            "row": 5
        }
        ```

12. 统计题目信息
    * Url : `/Manager/statQuiz`
    * Method : Post
    * RequiredData : token(string)
        ```json
        {
            "token": "e940aaa4-16f0-4d49-b62d-53a3652b1d02"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
            {
            "0": {
                "0": "\u540d\u79f0",
                "1": "\u7edf\u8ba1"
            },
            "1": {
                "0": "\u9898\u76ee\u603b\u6570",
                "1": "62"
            },
            "2": {
                "0": "\u9009\u62e9\u9898\u603b\u6570",
                "1": "40"
            },
            "3": {
                "0": "\u5224\u65ad\u9898\u603b\u6570",
                "1": "11"
            },
            "4": {
                "0": "\u586b\u7a7a\u9898\u603b\u6570",
                "1": "10"
            },
            "5": {
                "0": "\u7f16\u7a0b\u9898\u603b\u6570",
                "1": "1"
            },
            "6": {
                "0": "\u6700\u591a\u6536\u85cf\u7684\u9898\u76eeid",
                "1": "1"
            },
            "7": {
                "0": "\u6700\u6613\u9519\u7684\u9898\u76eeid",
                "1": "N/A"
            },
            "col": 2,
            "info": "\u83b7\u53d6\u6210\u529f",
            "result": "true",
            "row": 8
        }
        ```
13. 批量操作用户
    * Url : `/Manager/userBatch`
    * Method : Post
    * RequiredData : token(string)、count(int)
        ```json
        {
            "0": {
                "id": "2109104047",
                "name": "azh",
                "password": "123456",
                "role": "0"
            },
            "1": {
                "id": "2109104054",
                "name": "zty",
                "password": "123456",
                "role": "0"
            },
            "count": 2,
            "token": "f2a72ce1-2f9f-49d5-9398-ba424a1d5c10"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
            {
                "info": "\u5904\u7406\u6210\u529f,\u65b0\u589e1\u6761\u8bb0\u5f55,\u65b0\u589e1\u6761\u8bb0\u5f55",
                "result": "true"
            }
        ```
14. 批量操作班级
    * Url : `/Manager/classBatch`
    * Method : Post
    * RequiredData : token(string)、count(int)
        ```json
        {
            "0": {
                "id": "21091040",
                "name": "21软件本(2)班",
                "teacherId": "20250115"
            },
            "1": {
                "id": "21091030",
                "name": "21软件本(1)班",
                "teacherId": "20250114"
            },
            "count": 2,
            "token": "fb78630a-8871-4bb1-9215-2d6259e7240e"
        }
        ```
    * Response : 返回数据，如下所示:
        ```json
            {
                "info": "\u5904\u7406\u6210\u529f,\u65b0\u589e1\u6761\u8bb0\u5f55,\u4fee\u65391\u6761\u8bb0\u5f55,\u5931\u8d25\u4e860\u6b21(\u6559\u5e08id\u9519\u8bef)",
                "result": "true"
            }
        ```