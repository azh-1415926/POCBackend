#include "User.h"

#include <json/json.h>
#include <drogon/orm/Exception.h>

#include "Base.h"

void User::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback,
           std::string &&userId,
           const std::string &password)
{
    LOG_DEBUG<<"User "<<userId<<" try to login";

    Json::Value ret;

    if(userId.empty())
    {
        azh::drogon::returnFalse(callback,"登陆失败，用户id为空");
        return;
    }

    auto json = req->getJsonObject();

    bool isVaild=false;
    bool isFound=false;

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
        isFound=true;
        std::cout << "user password is " << row["password"].as<std::string>() << std::endl;
        if(row["password"].as<std::string>()==password)
        {
            LOG_DEBUG<<"User "<<userId<<" passwd:"<<password;
            isVaild=true;
        }    
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"登陆失败，用户不存在");
        return;
    }

    if(!isVaild)
    {
        azh::drogon::returnFalse(callback,"登陆失败，密码错误");
        return;
    }

    if(userId=="admin")
    {
        auto session=req->getSession();
        if(session->find("token"))
        {
            std::string token=session->get<std::string>("token");
            LOG_DEBUG<< "The Admin has token : "<<token;
        }
        else
        {
            std::string token=drogon::utils::getUuid();
            tokenOfAdmin::getInstance().set(token);
            ret["token"]=token;

            LOG_DEBUG<< "The Admin has no token,send token:" <<token;
        }
    }
    
    azh::drogon::returnTrue(callback,"登陆成功",ret);
}

void User::getInfo(const HttpRequestPtr &req,
             std::function<void (const HttpResponsePtr &)> &&callback,const std::string &userId) const
{
    auto json = req->getJsonObject();

    Json::Value ret;

    if(userId.empty())
    {
        azh::drogon::returnFalse(callback,"用户id为空，无法获取信息");
        return;
    }

    bool isFound=false;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+userId+"'");
    for (auto row : result)
    {
        isFound=true;
        ret["user_name"]=row["name"].as<std::string>();
        ret["role"]=row["role"].as<int>();
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"获取用户失败，无该用户");
        return;
    }
    
    azh::drogon::returnTrue(callback,"获取用户成功",ret);
}