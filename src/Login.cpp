#include "Login.h"

#include <json/json.h>
#include <drogon/orm/Exception.h>

void Login::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback,
           std::string &&userId,
           const std::string &password)
{
    LOG_DEBUG<<"User "<<userId<<" login";

    auto json = req->getJsonObject();

    auto session=req->session();

    bool isVaild=false;

    auto clientPtr = drogon::app().getDbClient("POC");
    // clientPtr->execSqlAsync("select * from users where id="+userId,
    //                             [&isVaild,userId,password](const drogon::orm::Result &result) {
    //                                 std::cout << result.size() << " rows selected!" << std::endl;
    //                                 int i = 0;
    //                                 for (auto row : result)
    //                                 {
    //                                     std::cout << i++ << ": user password is " << row["password"].as<std::string>() << std::endl;
    //                                     if(row["password"].as<std::string>()==password)
    //                                     {
    //                                         LOG_DEBUG<<"User "<<userId<<" passwd:"<<password;
    //                                         isVaild=true;
    //                                     }    
    //                                 }
    //                             },
    //                             [](const drogon::orm::DrogonDbException &e) {
    //                                 std::cerr << "error:" << e.base().what() << std::endl;
    //                             });

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id="+userId);

    // std::cout << result.size() << " rows selected!" << std::endl;
    for (auto row : result)
    {
        std::cout << "user password is " << row["password"].as<std::string>() << std::endl;
        if(row["password"].as<std::string>()==password)
        {
            LOG_DEBUG<<"User "<<userId<<" passwd:"<<password;
            isVaild=true;
        }    
    }

    Json::Value ret;

    if(isVaild)
    {
        ret["success"] = "true";

        if(session->find("token"))
        {
            std::string token=session->get<std::string>("token");
            std::cout << "has token:"<<token << std::endl;
        }
        else
        {
            std::cout << "no token,send token" << std::endl;
            std::string token=drogon::utils::getUuid();
            session->insert("token",token);

            ret["token"]=token;
        }
    }
    else
    {
        ret["success"] = "false";
    }
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");

    callback(resp);
}
void Login::getInfo(const HttpRequestPtr &req,
             std::function<void (const HttpResponsePtr &)> &&callback,
             const std::string &token) const
{
    auto json = req->getJsonObject();

    auto session=req->session();
    Json::Value ret;

    if(session->find("token"))
    {
        std::string token=session->get<std::string>("token");
        std::cout << "has token:"<<token << std::endl;
    }
    else
    {
        std::cout << "no token,send token" << std::endl;
        std::string token=drogon::utils::getUuid();
        session->insert("token",token);

        ret["token"]=token;
    }
    
    ret["result"]="ok";
    ret["user_name"]="azh";
    ret["gender"]=1;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}