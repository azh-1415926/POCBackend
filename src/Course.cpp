#include "Course.h"

#include "Base.h"

void Course::getOutline(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from course_content where id in (select course_content_id from course where section=0)");

    int countOfChapter=result.size();

    for (int i=0;i<countOfChapter;i++)
    {
        Json::Value course;
        
        const drogon::orm::Result &resultOfSection=clientPtr->execSqlSync("select * from course_content where id in (select course_content_id from course where chapter="+std::to_string(i)+")");

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
    
    Json::Value data;
    std::vector<std::string> params={ "chapter","section" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string chapter=data["chapter"].as<std::string>();
    std::string section=data["section"].as<std::string>();

    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from course_content where id in (select course_content_id from course where chapter="+chapter+" and section="+section+")");

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
    
    Json::Value data;
    std::vector<std::string> params={ "token","chapter","section","content" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"更新失败，管理员token无效，请重新登陆");
        return;
    }

    std::string chapter=data["chapter"].as<std::string>();
    std::string section=data["section"].as<std::string>();
    std::string content=data["content"].as<std::string>();

    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from course_content where id in (select course_content_id from course where chapter="+chapter+" and section="+section+")");

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

    clientPtr->execSqlSync("update course_content set content='"+content+"' where id in (select course_content_id from course where chapter="+chapter+" and section="+section+")");

    azh::drogon::returnTrue(callback,"更新成功",ret);
}