#include "Course.h"

#include "Base.h"

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
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Course::getCourse(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &chapter, const std::string &section)
{
    if(chapter.empty())
    {
        azh::drogon::returnFalse(callback,"获取失败，请求数据为空");
        return;
    }

    Json::Value ret;

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
    {
        azh::drogon::returnFalse(callback,"获取失败，无该章节");
        return;
    }

    azh::drogon::returnTrue(callback,"获取成功");
}