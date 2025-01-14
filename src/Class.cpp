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
