#include "Login.h"

#include <json/json.h>
#include <drogon/orm/Exception.h>

void Login::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback,
           std::string &&userId,
           const std::string &password)
{
    LOG_DEBUG<<"User "<<userId<<" try to login";

    Json::Value ret;

    if(userId.empty())
    {
        ret["success"] = "false";
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        resp->addHeader("Access-Control-Allow-Origin","*");

        callback(resp);
        return;
    }

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

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+userId+"'");

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
            session->insert("userId",userId);

            ret["token"]=token;
            // const drogon::orm::Result &resultOfToken=clientPtr->execSqlSync("select * from token where id="+userId);
            // if(resultOfToken.size()==0)
            // {
            //     std::cout<<"add a token to database\n";
            //     clientPtr->execSqlSync("insert into token values("+userId+","+token+")");
            // }
            // else
            // {
            //     std::cout<<"update a token from database\n";
            //     clientPtr->execSqlSync("update token set value="+token+" where id="+userId);
            // }
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
             std::function<void (const HttpResponsePtr &)> &&callback,const std::string &userId) const
{
    auto json = req->getJsonObject();

    Json::Value ret;

    if(userId.empty())
    {
        ret["success"] = "false";
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        resp->addHeader("Access-Control-Allow-Origin","*");

        callback(resp);
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");


    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+userId+"'");
    for (auto row : result)
    {
        ret["result"]="true";
        ret["user_name"]=row["name"].as<std::string>();
        ret["role"]=row["role"].as<int>();
    }

    if(!ret["result"])
    {
        ret["result"]="false";
    }
    
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");
    callback(resp);
}