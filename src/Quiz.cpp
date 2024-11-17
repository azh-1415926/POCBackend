#include "Quiz.h"

void Quiz::getquiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,const std::string& chapter)
{
    Json::Value ret;

    if(chapter.empty())
    {
        ret["success"] = "false";
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        resp->addHeader("Access-Control-Allow-Origin","*");

        callback(resp);
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from quiz where chapter="+chapter);

    int count=0;

    for (auto row : result)
    {
        Json::Value quiz;
        quiz["type"]=row["type"].as<int>();
        quiz["content"]=row["content"].as<std::string>();
        quiz["A"]=row["optionA"].as<std::string>();
        quiz["B"]=row["optionB"].as<std::string>();
        quiz["C"]=row["optionC"].as<std::string>();
        quiz["D"]=row["optionD"].as<std::string>();
        quiz["answer"]=row["answer"].as<std::string>();

        ret[std::to_string(count)]=quiz;

        count++;
    }

    ret["count"]=count;
    
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");
    callback(resp);
}