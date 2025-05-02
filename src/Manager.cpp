#include "Manager.h"

#include "Base.h"

void Manager::isValid(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &userId)
{
    Json::Value ret;

    if(userId.empty())
    {
        azh::drogon::returnFalse(callback,"输入为空，请输入用户id");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+userId+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"判定失败，无该用户");
        return;
    }
    
    azh::drogon::returnTrue(callback,"判定成功，该用户存在");
}

void Manager::addUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token","id","name","password" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"添加失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
        azh::drogon::returnFalse(callback,"添加失败，该id已存在");
        return;
    }

    clientPtr->execSqlSync("insert into users values('"+data["id"].as<std::string>()+"','"+data["name"].as<std::string>()+"','"+data["password"].as<std::string>()+"',"+(data["role"].as<std::string>())+",NOW(),NOW(),NOW());");

    azh::drogon::returnTrue(callback,"添加成功");
}

void Manager::removeUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token","id" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;

        if(row["name"].as<std::string>()=="admin")
        {
            azh::drogon::returnFalse(callback,"删除失败，无法删除管理员");
            return;
        }
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"删除失败，该用户不存在");
        return;
    }

    clientPtr->execSqlSync("delete from users where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"删除成功");
}

void Manager::alterUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token","id" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;

        if(row["name"].as<std::string>()=="admin")
        {
            azh::drogon::returnFalse(callback,"修改失败，无法修改管理员");
            return;
        }
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"修改失败，该用户不存在");
        return;
    }

    clientPtr->execSqlSync("update users set name='"+data["name"].as<std::string>()+"',password='"+data["password"].as<std::string>()+"',role="+data["role"].as<std::string>()+" where id='"+data["id"].as<std::string>()+"'");
    clientPtr->execSqlSync("update users set update_time=NOW() where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"修改成功");
}

void Manager::addClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token","id","teacherId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(isFound)
    {
        azh::drogon::returnFalse(callback,"添加失败，该班级已存在");
        return;
    }

    clientPtr->execSqlSync("insert into class values('"+data["id"].as<std::string>()+"','"+data["name"].as<std::string>()+"','"+data["teacherId"].as<std::string>()+"');");

    azh::drogon::returnTrue(callback,"添加成功");
}

void Manager::removeClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token","id" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"删除失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"删除失败，该班级不存在");
        return;
    }

    clientPtr->execSqlSync("delete from class where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"删除成功");
}

void Manager::alterClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token","id","teacherId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"修改失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");
    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from class where id='"+data["id"].as<std::string>()+"'");

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"修改失败，该班级不存在");
        return;
    }

    clientPtr->execSqlSync("update class set name='"+data["name"].as<std::string>()+"',teacher_id='"+data["teacherId"].as<std::string>()+"' where id='"+data["id"].as<std::string>()+"'");

    azh::drogon::returnTrue(callback,"修改成功");
}

void Manager::getUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT u.id,u.name,'学生' AS user_role,c_student.name AS class_name,u.last_login_time "
        "FROM users u "
        "LEFT JOIN student s ON u.id = s.id "
        "LEFT JOIN class c_student ON s.class_id = c_student.id "
        "WHERE u.role = 0 "
        // "WHERE s.id IS NOT NULL "
        "UNION "
        "SELECT u.id,u.name,'教师' AS user_role,'' AS class_name,u.last_login_time "
        "FROM users u "
        //"LEFT JOIN class c_teacher ON u.id = c_teacher.teacher_id "
        "WHERE u.role = 1;";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    int rowCount=1;

    Json::Value ret;
    
    Json::Value tittle;
    tittle["0"]="姓名";
    tittle["1"]="编号";
    tittle["2"]="职位";
    tittle["3"]="班级名";
    tittle["4"]="最后登录时间";

    ret["col"]=5;
    ret["0"]=tittle;

    for (auto row : result)
    {
        Json::Value userInfo;

        userInfo["0"]=row["name"].as<std::string>();
        userInfo["1"]=row["id"].as<std::string>();
        userInfo["2"]=row["user_role"].as<std::string>();
        userInfo["3"]=row["class_name"].as<std::string>();
        userInfo["4"]=row["last_login_time"].as<std::string>();
        
        ret[std::to_string(rowCount)]=userInfo;
        rowCount++;
    }

    ret["row"]=rowCount;

    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Manager::getClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"获取失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT c.id AS class_id,c.name AS class_name,u_teacher.name AS teacher_name,COUNT(s.id) AS population "
            "FROM class c JOIN users u_teacher ON c.teacher_id = u_teacher.id AND u_teacher.role = 1 "
            "LEFT JOIN " 
            "student s ON c.id = s.class_id "
            "GROUP BY "
            "c.id, c.name, u_teacher.name;";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    int rowCount=1;

    Json::Value ret;
    
    Json::Value title;
    title["0"]="名称";
    title["1"]="编号";
    title["2"]="任课教师";
    title["3"]="学生总数";

    ret["col"]=4;
    ret["0"]=title;

    for (auto row : result)
    {
        Json::Value userInfo;

        userInfo["0"]=row["class_name"].as<std::string>();
        userInfo["1"]=row["class_id"].as<std::string>();
        userInfo["2"]=row["teacher_name"].as<std::string>();
        userInfo["3"]=row["population"].as<std::string>();
        
        ret[std::to_string(rowCount)]=userInfo;
        rowCount++;
    }

    ret["row"]=rowCount;

    azh::drogon::returnTrue(callback,"获取成功",ret);
}


void Manager::statUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"获取失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    int rowCount=1;

    Json::Value ret;
    
    Json::Value title;
    title["0"]="名称";
    title["1"]="统计";

    ret["col"]=2;
    ret["0"]=title;

    std::string query="SELECT "
        // "-- 用户总数 "
        "(SELECT COUNT(*) FROM users) AS total_users,"
        // "-- 学生总数"
        "(SELECT COUNT(*) FROM users WHERE role = 0) AS total_students,"
        // "-- 教师总数"
        "(SELECT COUNT(*) FROM users WHERE role = 1) AS total_teachers,"
        // "-- 最近登录的用户"
        "(SELECT name FROM users WHERE role = 0 || role=1 ORDER BY last_login_time DESC LIMIT 1) AS recent_login_user,"
        // "-- 最久未登录的用户"
        "(SELECT name FROM users WHERE role = 0 || role=1 ORDER BY last_login_time ASC LIMIT 1) AS inactive_user;";
    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    Json::Value totalUsers;
    Json::Value totalStudents;
    Json::Value totalTeachers;
    Json::Value recentLoginUser;
    Json::Value inactiveUser;

    for (auto row : result)
    {
        Json::Value userInfo;

        totalUsers["0"]="用户总数";
        totalUsers["1"]=row["total_users"].as<std::string>();
        totalStudents["0"]="学生总数";
        totalStudents["1"]=row["total_students"].as<std::string>();
        totalTeachers["0"]="教师总数";
        totalTeachers["1"]=row["total_teachers"].as<std::string>();
        recentLoginUser["0"]="最近登录用户";
        recentLoginUser["1"]=row["recent_login_user"].as<std::string>();
        inactiveUser["0"]="不活跃用户";
        inactiveUser["1"]=row["inactive_user"].as<std::string>();

        ret["1"]=totalUsers;
        ret["2"]=totalStudents;
        ret["3"]=totalTeachers;
        ret["4"]=recentLoginUser;
        ret["5"]=inactiveUser;

        rowCount=6;
    }

    ret["row"]=rowCount;

    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Manager::statClass(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"获取失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    int rowCount=1;

    Json::Value ret;
    
    Json::Value title;
    title["0"]="名称";
    title["1"]="统计";

    ret["col"]=2;
    ret["0"]=title;

    std::string query="SELECT "
        // 班级总数，查询为 NULL 时为 0
        "COALESCE((SELECT COUNT(*) FROM class), 0) AS total_classes,"
        // 管理班级最多的教师id，查询为 NULL 时为 'N/A'
        "COALESCE("
        "    (SELECT teacher_id "
        "     FROM class "
        "     GROUP BY teacher_id "
        "     ORDER BY COUNT(*) DESC "
        "     LIMIT 1), "
        "    'N/A'"
        ") AS most_active_teacher_id,"
        // 班级人数最多的班级，查询为 NULL 时为 'N/A'
        "COALESCE("
        "    (SELECT class_id "
        "     FROM student "
        "     GROUP BY class_id "
        "     ORDER BY COUNT(*) DESC "
        "     LIMIT 1), "
        "    'N/A'"
        ") AS most_popular_class_id,"
        // 班级人数最少的班级，查询为 NULL 时为 'N/A'
        "COALESCE("
        "    (SELECT class_id "
        "     FROM student "
        "     GROUP BY class_id "
        "     ORDER BY COUNT(*) ASC "
        "     LIMIT 1), "
        "    'N/A'"
        ") AS least_popular_class_id;";
    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    Json::Value totalClass;
    Json::Value activeTeacher;
    Json::Value activeClass;
    Json::Value inactiveClass;

    for (auto row : result)
    {
        Json::Value userInfo;

        totalClass["0"]="班级总数";
        totalClass["1"]=row["total_classes"].as<std::string>();
        activeTeacher["0"]="管理班级最多的教师";
        activeTeacher["1"]=row["most_active_teacher_id"].as<std::string>();
        activeClass["0"]="人数最多的班级";
        activeClass["1"]=row["most_popular_class_id"].as<std::string>();
        inactiveClass["0"]="人数最少的班级";
        inactiveClass["1"]=row["least_popular_class_id"].as<std::string>();

        ret["1"]=totalClass;
        ret["2"]=activeTeacher;
        ret["3"]=activeClass;
        ret["4"]=inactiveClass;

        rowCount=5;
    }

    ret["row"]=rowCount;

    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Manager::statQuiz(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "token" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"获取失败，管理员token无效，请重新登陆");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    int rowCount=1;

    Json::Value ret;
    
    Json::Value title;
    title["0"]="名称";
    title["1"]="统计";

    ret["col"]=2;
    ret["0"]=title;

    std::string query="SELECT "
        // 题目总数
        "COALESCE((SELECT COUNT(*) FROM quiz), 0) AS total_quizzes,"
        // 选择题总数
        "COALESCE((SELECT COUNT(*) FROM quiz WHERE type = 0), 0) AS total_select_choice,"
        // 判断题总数
        "COALESCE((SELECT COUNT(*) FROM quiz WHERE type = 1), 0) AS total_judge_choice,"
        // 填空题总数
        "COALESCE((SELECT COUNT(*) FROM quiz WHERE type = 2), 0) AS total_fill_blank,"
        // 编程题总数
        "COALESCE((SELECT COUNT(*) FROM quiz WHERE type = 3), 0) AS total_coding,"
        // 收藏数最多的题目id
        "COALESCE("
        "    (SELECT quiz_id "
        "     FROM collected_quiz "
        "     GROUP BY quiz_id "
        "     ORDER BY COUNT(*) DESC "
        "     LIMIT 1), "
        "    'N/A'"
        ") AS most_collected_quiz_id,"
        // 答错数最多的题目id
        "COALESCE("
        "    (SELECT quiz_id "
        "     FROM wrong_quiz "
        "     GROUP BY quiz_id "
        "     ORDER BY SUM(wrong_count) DESC "
        "     LIMIT 1), "
        "    'N/A'"
        ") AS most_wrong_quiz_id;";
    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    Json::Value totalQuiz;
    Json::Value totalSelectChoice;
    Json::Value totalJudgeChoice;
    Json::Value totalFillBlank;
    Json::Value totalCoding;
    Json::Value mostCollectedQuiz;
    Json::Value mostWrongQuiz;

    for (auto row : result)
    {
        Json::Value userInfo;

        totalQuiz["0"]="题目总数";
        totalQuiz["1"]=row["total_quizzes"].as<std::string>();
        totalSelectChoice["0"]="选择题总数";
        totalSelectChoice["1"]=row["total_select_choice"].as<std::string>();
        totalJudgeChoice["0"]="判断题总数";
        totalJudgeChoice["1"]=row["total_judge_choice"].as<std::string>();
        totalFillBlank["0"]="填空题总数";
        totalFillBlank["1"]=row["total_fill_blank"].as<std::string>();
        totalCoding["0"]="编程题总数";
        totalCoding["1"]=row["total_coding"].as<std::string>();
        mostCollectedQuiz["0"]="最多收藏的题目id";
        mostCollectedQuiz["1"]=row["most_collected_quiz_id"].as<std::string>();
        mostWrongQuiz["0"]="最易错的题目id";
        mostWrongQuiz["1"]=row["most_wrong_quiz_id"].as<std::string>();

        ret["1"]=totalQuiz;
        ret["2"]=totalSelectChoice;
        ret["3"]=totalJudgeChoice;
        ret["4"]=totalFillBlank;
        ret["5"]=totalCoding;
        ret["6"]=mostCollectedQuiz;
        ret["7"]=mostWrongQuiz;

        rowCount=8;
    }

    ret["row"]=rowCount;

    azh::drogon::returnTrue(callback,"获取成功",ret);
}