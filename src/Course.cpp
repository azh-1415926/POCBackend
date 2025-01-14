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

void Course::getCourse(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"获取课程失败，请带上请求数据，比如指定章、节");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("chapter"))
    {
        azh::drogon::returnFalse(callback,"获取失败，章为空");
        return;
    }

    if(!data.find("section"))
    {
        azh::drogon::returnFalse(callback,"获取失败，节为空");
        return;
    }

    std::string chapter=data["chapter"].as<std::string>();
    std::string section=data["section"].as<std::string>();

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

    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Course::updateCourse(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"更新失败，未知的请求，请带上管理员token和数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"更新失败，管理员token无效，请重新登陆");
        return;
    }

    if(!data.find("chapter"))
    {
        azh::drogon::returnFalse(callback,"更新失败，章为空");
        return;
    }

    if(!data.find("section"))
    {
        azh::drogon::returnFalse(callback,"更新失败，节为空");
        return;
    }

    if(!data.find("content"))
    {
        azh::drogon::returnFalse(callback,"更新失败，更新内容为空");
        return;
    }

    std::string chapter=data["chapter"].as<std::string>();
    std::string section=data["section"].as<std::string>();
    std::string content=data["content"].as<std::string>();

    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from course_content where chapter="+chapter+" and section="+section);

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"更新失败，无该章节");
        return;
    }

    clientPtr->execSqlSync("update course_content set content='"+content+"' where chapter="+chapter+" and section="+section);

    azh::drogon::returnTrue(callback,"更新成功",ret);
}