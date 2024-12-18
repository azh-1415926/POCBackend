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

    Json::Value ret;

    // 添加用户的逻辑

    ret["success"]="true";
    ret["info"]="成功添加用户";

    azh::drogon::returnTrue(callback,"添加成功",ret);
}
