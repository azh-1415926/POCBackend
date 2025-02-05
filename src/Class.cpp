#include "Class.h"

#include <Base.h>

void Class::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"查询失败，未知的请求，请带上id和管理员token");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"查询失败，班级id为空");
        return;
    }

    std::string classId=data["id"].as<std::string>();

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where id='"+classId+"'");

    Json::Value ret;
    bool isFound=false;

    for (auto row : result)
    {
        ret["name"]=row["name"].as<std::string>();
        ret["teacherId"]=row["teacher_id"].as<std::string>();

        isFound=true;
    }

    if(!isFound)
        azh::drogon::returnFalse(callback,"获取失败，该班级不存在");
    else
        azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Class::allocate(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"分配失败，未知的请求，请带上id和管理员token");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"分配失败，请输入班级id，请求格式有误");
        return;
    }

    if(!data.find("userId"))
    {
        azh::drogon::returnFalse(callback,"分配失败，请输入用户id，请求格式有误");
        return;
    }

    std::string classId=data["id"].as<std::string>();
    std::string teacherId;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &resultOfClass=clientPtr->execSqlSync("select * from class where id='"+classId+"'");

    bool isFound=false;

    for (auto row : resultOfClass)
    {
        teacherId=row["teacher_id"].as<std::string>();

        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"分配失败，该班级不存在");
    }
    
    std::string userId=data["userId"].as<std::string>();

    const drogon::orm::Result &resultOfUser=clientPtr->execSqlSync("select * from users where id='"+userId+"'");

    isFound=false;

    for (auto row : resultOfUser)
    {
        int role=row["role"].as<int>();

        if(role!=0)
        {
            azh::drogon::returnFalse(callback,"分配失败，该用户不为学生");
            return;
        }

        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"分配失败，该用户不存在");
        return;
    }

    const drogon::orm::Result &resultOfStudent=clientPtr->execSqlSync("select * from student where id='"+userId+"'");


    for (auto row : resultOfStudent)
    {
        azh::drogon::returnFalse(callback,"分配失败，该用户已分配过班级");
        return;
    }

    clientPtr->execSqlSync("insert into student values('"+userId+"','"+classId+"','"+teacherId+"');");

    azh::drogon::returnTrue(callback,"分配成功");
}
