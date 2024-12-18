#include "Class.h"

#include <Base.h>

void Class::getClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &classId)
{
    if(classId.empty())
    {
        azh::drogon::returnFalse(callback,"获取失败，班级id为空");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where class_id="+classId);

    Json::Value ret;
    bool isFound=false;

    for (auto row : result)
    {
        ret["name"]=row["name"].as<std::string>();
        ret["teacher_id"]=row["teacher_id"].as<std::string>();

        isFound=true;
    }

    if(!isFound)
        azh::drogon::returnFalse(callback,"获取失败，该班级不存在");
    else
        azh::drogon::returnFalse(callback,"获取成功",ret);
}
