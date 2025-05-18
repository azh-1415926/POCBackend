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

void Quiz::collectQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "chapter","studentId","data" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string chapter=data["chapter"].as<std::string>();
    std::string studentId=data["studentId"].as<std::string>();
    Json::Value ids=data["data"];

    int count=ids.size();
    std::string strOfIds="(";

    if(count==0)
    {
        azh::drogon::returnTrue(callback,"无操作");
        return;
    }

    for(int i=0;i<count-1;i++)
    {
        strOfIds+=std::to_string((ids[i].as<int>()+1))+",";
    }

    strOfIds+=std::to_string((ids[count-1].as<int>()+1))+")";

    std::string query="INSERT IGNORE INTO collected_quiz (student_id, quiz_id) "
        "SELECT "
        "'"+studentId+"',"
        "id " 
        "FROM quiz "
        "WHERE id IN "+strOfIds+";";

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync(query);
    
    azh::drogon::returnTrue(callback,"执行成功");
}

void Quiz::uncollectQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "chapter","studentId","data" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string chapter=data["chapter"].as<std::string>();
    std::string studentId=data["studentId"].as<std::string>();
    Json::Value ids=data["data"];

    int count=ids.size();
    std::string strOfIds="(";

    if(count==0)
    {
        azh::drogon::returnTrue(callback,"无操作");
        return;
    }

    for(int i=0;i<count-1;i++)
    {
        strOfIds+=std::to_string((ids[i].as<int>()+1))+",";
    }

    strOfIds+=std::to_string((ids[count-1].as<int>()+1))+")";

    std::string query="DELETE FROM collected_quiz "
        "WHERE student_id = '"+studentId+"' "
        "AND quiz_id IN "+strOfIds+";";
    
    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync(query);
    
    azh::drogon::returnTrue(callback,"执行成功");
}

void Quiz::addWrongQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "studentId","data" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string studentId=data["studentId"].as<std::string>();
    Json::Value ids=data["data"];

    int count=ids.size();
    std::string strOfIds="";

    if(count==0)
    {
        azh::drogon::returnTrue(callback,"无操作");
        return;
    }

    for(int i=0;i<count-1;i++)
    {
        strOfIds+="('"+studentId+"',"+std::to_string((ids[i].as<int>()))+",1),";
    }

    strOfIds+="('"+studentId+"',"+std::to_string((ids[count-1].as<int>()))+",1)";

    std::string query="INSERT INTO wrong_quiz (student_id, quiz_id, wrong_count) "
        "VALUES "
        +strOfIds+" "
        "ON DUPLICATE KEY UPDATE "
        "wrong_count = wrong_count + 1;";
    
    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync(query);
    
    azh::drogon::returnTrue(callback,"执行成功");
}

void Quiz::removeWrongQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "studentId","data" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string studentId=data["studentId"].as<std::string>();
    Json::Value ids=data["data"];

    int count=ids.size();
    std::string strOfIds="(";

    if(count==0)
    {
        azh::drogon::returnTrue(callback,"无操作");
        return;
    }

    for(int i=0;i<count-1;i++)
    {
        strOfIds+=std::to_string((ids[i].as<int>()+1))+",";
    }

    strOfIds+=std::to_string((ids[count-1].as<int>()+1))+")";

    std::string queryOfDelete="DELETE FROM wrong_quiz "
        "WHERE student_id = '"+studentId+"' "
        "AND quiz_id IN "+strOfIds+" AND wrong_count=1;";

    std::string queryOfUpdate="UPDATE IGNORE wrong_quiz "
        "SET wrong_count=wrong_count-1 "
        "WHERE student_id = '"+studentId+"' "
        "AND quiz_id IN "+strOfIds+" ;";
    
    auto clientPtr = drogon::app().getDbClient("POC");
    clientPtr->execSqlSync(queryOfDelete);
    clientPtr->execSqlSync(queryOfUpdate);
    
    azh::drogon::returnTrue(callback,"执行成功");
}

void Quiz::getChapter(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value ret;

    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback,ret))
        return;

    std::string studentId=data["studentId"].as<std::string>();
    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("SELECT COALESCE(MAX(chapter), 0) AS max_chapter FROM quiz;");

    int count=result.size();

    for (int i=0;i<count;i++)
    {
        ret["chapter"]=result.at(i)["max_chapter"].as<int>();
    }
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Quiz::getAnswer(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "chapter" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string chapter=data["chapter"].as<std::string>();

    Json::Value ret;
    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT "
        "q.* "
        "FROM "
        "quiz q "
        "WHERE "
        "q.chapter = "+chapter+";";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    int count=0;

    for (auto row : result)
    {
        Json::Value quiz;
        quiz["id"]=row["id"].as<int>();
        quiz["answer"]=row["answer"].as<std::string>();

        ret[std::to_string(count)]=quiz;

        count++;
    }

    ret["count"]=count;
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Quiz::getWrongQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "studentId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string studentId=data["studentId"].as<std::string>();

    Json::Value ret;
    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT * "
        "FROM quiz q "
        "WHERE id in("
        "SELECT quiz_id from wrong_quiz "
        "WHERE student_id = '"+studentId+"'"
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
