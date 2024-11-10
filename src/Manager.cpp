#include "Manager.h"

void Manager::isValid(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &userId)
{
    Json::Value ret;

    if(userId.empty())
    {
        ret["success"] = "false";
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        resp->addHeader("Access-Control-Allow-Origin","*");

        callback(resp);
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
        ret["success"]="false";
    else
        ret["success"]="true";
    
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");
    callback(resp);
}