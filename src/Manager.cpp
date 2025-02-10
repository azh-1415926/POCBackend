#include "Manager.h"

#include "Base.h"

void Manager::isValid(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &userId)
{
    Json::Value ret;

    if(userId.empty())
    {
        azh::drogon::returnFalse(callback,"输入为空，请输入用户id");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+userId+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"判定失败，无该用户");
        return;
    }
    
    azh::drogon::returnTrue(callback,"判定成功，该用户存在");
}

void Manager::addUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"添加失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"添加失败，管理员token无效，请重新登陆");
        return;
    }

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"添加失败，请输入用户id，请求格式有误");
        return;
    }

    if(!data.find("name"))
    {
        azh::drogon::returnFalse(callback,"添加失败，请输入用户姓名，请求格式有误");
        return;
    }

    if(!data.find("password"))
    {
        azh::drogon::returnFalse(callback,"添加失败，请输入用户密码，请求格式有误");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
        azh::drogon::returnFalse(callback,"添加失败，该id已存在");
        return;
    }

    clientPtr->execSqlSync("insert into users values('"+data["id"].as<std::string>()+"','"+data["name"].as<std::string>()+"','"+data["password"].as<std::string>()+"',"+(data["role"].as<std::string>())+",NOW(),NOW(),NOW());");

    azh::drogon::returnTrue(callback,"添加成功");
}

void Manager::removeUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"删除失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"删除失败，管理员token无效，请重新登陆");
        return;
    }

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"删除失败，请输入用户id，请求格式有误");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;

        if(row["name"].as<std::string>()=="admin")
        {
            azh::drogon::returnFalse(callback,"删除失败，无法删除管理员");
            return;
        }
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"删除失败，该用户不存在");
        return;
    }

    clientPtr->execSqlSync("delete from users where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"删除成功");
}

void Manager::alterUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"修改失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"修改失败，管理员token无效，请重新登陆");
        return;
    }

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"修改失败，请输入用户id，请求格式有误");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;

        if(row["name"].as<std::string>()=="admin")
        {
            azh::drogon::returnFalse(callback,"修改失败，无法修改管理员");
            return;
        }
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"修改失败，该用户不存在");
        return;
    }

    clientPtr->execSqlSync("update users set name='"+data["name"].as<std::string>()+"',password='"+data["password"].as<std::string>()+"',role="+data["role"].as<std::string>()+" where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"修改成功");
}

void Manager::addClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"添加失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"添加失败，管理员token无效，请重新登陆");
        return;
    }

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"添加失败，请输入班级id，请求格式有误");
        return;
    }

    if(!data.find("teacherId"))
    {
        azh::drogon::returnFalse(callback,"添加失败，请输入任课教师id，请求格式有误");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(isFound)
    {
        azh::drogon::returnFalse(callback,"添加失败，该班级已存在");
        return;
    }

    clientPtr->execSqlSync("insert into class values('"+data["id"].as<std::string>()+"','"+data["name"].as<std::string>()+"','"+data["teacherId"].as<std::string>()+"');");

    azh::drogon::returnTrue(callback,"添加成功");
}

void Manager::removeClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"删除失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"删除失败，管理员token无效，请重新登陆");
        return;
    }

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"删除失败，请输入班级id，请求格式有误");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"删除失败，该班级不存在");
        return;
    }

    clientPtr->execSqlSync("delete from class where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"删除成功");
}

void Manager::alterClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"修改失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"修改失败，管理员token无效，请重新登陆");
        return;
    }

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"修改失败，请输入班级id，请求格式有误");
        return;
    }

    if(!data.find("teacherId"))
    {
        azh::drogon::returnFalse(callback,"修改失败，请输入任课教师id，请求格式有误");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"修改失败，该班级不存在");
        return;
    }

    clientPtr->execSqlSync("update class set name='"+data["name"].as<std::string>()+"',teacher_id='"+data["teacherId"].as<std::string>()+"' where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"修改成功");
}

void Manager::getUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"获取失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"获取失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT u.id,u.name,'学生' AS user_role,c_student.name AS class_name,u.last_login_time "
        "FROM users u "
        "LEFT JOIN student s ON u.id = s.id "
        "LEFT JOIN class c_student ON s.class_id = c_student.id "
        "WHERE u.role = 0 "
        // "WHERE s.id IS NOT NULL "
        "UNION "
        "SELECT u.id,u.name,'教师' AS user_role,'' AS class_name,u.last_login_time "
        "FROM users u "
        //"LEFT JOIN class c_teacher ON u.id = c_teacher.teacher_id "
        "WHERE u.role = 1;";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    int rowCount=1;

    Json::Value ret;
    
    Json::Value tittle;
    tittle["0"]="姓名";
    tittle["1"]="编号";
    tittle["2"]="职位";
    tittle["3"]="班级名";
    tittle["4"]="最后登录时间";

    ret["col"]=5;
    ret["0"]=tittle;

    for (auto row : result)
    {
        Json::Value userInfo;

        userInfo["0"]=row["name"].as<std::string>();
        userInfo["1"]=row["id"].as<std::string>();
        userInfo["2"]=row["user_role"].as<std::string>();
        userInfo["3"]=row["class_name"].as<std::string>();
        userInfo["4"]=row["last_login_time"].as<std::string>();
        
        ret[std::to_string(rowCount)]=userInfo;
        rowCount++;
    }

    ret["row"]=rowCount;

    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Manager::getClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"获取失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"获取失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT c.id AS class_id,c.name AS class_name,u_teacher.name AS teacher_name,COUNT(s.id) AS population "
            "FROM class c JOIN users u_teacher ON c.teacher_id = u_teacher.id AND u_teacher.role = 1 "
            "LEFT JOIN " 
            "student s ON c.id = s.class_id "
            "GROUP BY "
            "c.id, c.name, u_teacher.name;";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    int rowCount=1;

    Json::Value ret;
    
    Json::Value title;
    title["0"]="名称";
    title["1"]="编号";
    title["2"]="任课教师";
    title["3"]="学生总数";

    ret["col"]=4;
    ret["0"]=title;

    for (auto row : result)
    {
        Json::Value userInfo;

        userInfo["0"]=row["class_name"].as<std::string>();
        userInfo["1"]=row["class_id"].as<std::string>();
        userInfo["2"]=row["teacher_name"].as<std::string>();
        userInfo["3"]=row["population"].as<std::string>();
        
        ret[std::to_string(rowCount)]=userInfo;
        rowCount++;
    }

    ret["row"]=rowCount;

    azh::drogon::returnTrue(callback,"获取成功",ret);
}
