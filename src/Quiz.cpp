#include "Quiz.h"

#include "Base.h"

void Quiz::getQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "chapter","studentId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string chapter=data["chapter"].as<std::string>();
    std::string studentId=data["studentId"].as<std::string>();

    Json::Value ret;
    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT "
        "q.*,"
        "EXISTS ("
        "    SELECT 1 "
        "    FROM collected_quiz cq "
        "    WHERE cq.quiz_id = q.id "
        "      AND cq.student_id = '"+studentId+"'"
        ") AS collected "
        "FROM "
        "quiz q "
        "WHERE "
        "q.chapter = "+chapter+";";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

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
        // quiz["answer"]=row["answer"].as<std::string>();
        quiz["collected"]=row["collected"].as<int>();

        ret[std::to_string(count)]=quiz;

        count++;
    }

    ret["count"]=count;
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Quiz::getCollectedQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "chapter","studentId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string chapter=data["chapter"].as<std::string>();
    std::string studentId=data["studentId"].as<std::string>();

    Json::Value ret;
    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT q.* "
        "FROM quiz q "
        "WHERE q.chapter = "+chapter+" "
            "AND EXISTS ("
                "SELECT 1 "
                "FROM collected_quiz cq "
                "WHERE cq.quiz_id = q.id "
                "AND cq.student_id = '"+studentId+"'"
                ");";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

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
        // quiz["answer"]=row["answer"].as<std::string>();
        quiz["collected"]=1;

        ret[std::to_string(count)]=quiz;

        count++;
    }

    ret["count"]=count;
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}
