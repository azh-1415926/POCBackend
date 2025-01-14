#include "Quiz.h"

#include "Base.h"

void Quiz::getQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"获取失败，未知的请求");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("chapter"))
    {
        azh::drogon::returnFalse(callback,"请填写章节，获取指定章节的题目");
        return;
    }

    std::string chapter=data["chapter"].as<std::string>();

    Json::Value ret;
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
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}