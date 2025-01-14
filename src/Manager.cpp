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

    clientPtr->execSqlSync("insert into class values('"+data["id"].as<std::string>()+"','"+data["name"].as<std::string>()+"','"+data["teacher_id"].as<std::string>()+"');");

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

    if(!data.find("teacher_id"))
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

    clientPtr->execSqlSync("update class set name='"+data["name"].as<std::string>()+"',teacher_id='"+data["teacher_id"].as<std::string>()+"' where id='"+data["id"].as<std::string>()+"'");

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

    azh::drogon::returnTrue(callback,"获取成功");
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

    azh::drogon::returnTrue(callback,"获取成功");
}
