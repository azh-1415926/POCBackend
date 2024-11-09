#include "Course.h"

void Course::getOutline(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from course_content where section=0");

    int countOfChapter=result.size();

    for (int i=0;i<countOfChapter;i++)
    {
        Json::Value course;
        
        const drogon::orm::Result &resultOfSection=clientPtr->execSqlSync("select * from course_content where chapter="+std::to_string(i));

        int countOfSection=resultOfSection.size();

        for(int j=0;j<countOfSection;j++)
        {
            if(j==0)
                course["value"]=result.at(i)["name"].as<std::string>();
            else
                course[std::to_string(j-1)]=resultOfSection.at(j)["name"].as<std::string>();
        }

        course["count"]=countOfSection-1;
        ret[std::to_string(i)]=course;
    }

    ret["count"]=countOfChapter;
    
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");
    callback(resp);
}

void Course::getCourse(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &chapter, const std::string &section)
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

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from course_content where chapter="+chapter+" and section="+section);

    bool isFound=false;

    for (auto row : result)
    {
        ret["name"]=row["name"].as<std::string>();
        ret["content"]=row["content"].as<std::string>();

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