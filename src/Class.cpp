#include "Class.h"

void Class::getClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &classId)
{
    Json::Value ret;

    if(classId.empty())
    {
        ret["success"] = "false";
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        resp->addHeader("Access-Control-Allow-Origin","*");

        callback(resp);
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where class_id="+classId);

    bool isFound=false;

    for (auto row : result)
    {
        ret["name"]=row["name"].as<std::string>();
        ret["teacher_id"]=row["teacher_id"].as<std::string>();

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
