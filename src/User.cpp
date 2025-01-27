#include "User.h"

#include <json/json.h>
#include <drogon/orm/Exception.h>

#include "Base.h"

void User::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"登陆失败，未知的请求，请带上id和密码");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"登陆失败，用户id为空");
        return;
    }

    std::string userId=data["id"].as<std::string>();
    std::string password=data["password"].as<std::string>();

    LOG_DEBUG<<"User "<<userId<<" try to login";

    Json::Value ret;

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
        LOG_DEBUG << "User gived password is " << password;
        LOG_DEBUG << "User password is " << row["password"].as<std::string>();
        if(row["password"].as<std::string>()==password)
        {
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

void User::info(const HttpRequestPtr &req,
             std::function<void (const HttpResponsePtr &)> &&callback) const
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"查询失败，未知的请求，请带上id数据");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("id"))
    {
        azh::drogon::returnFalse(callback,"查询失败，用户id为空");
        return;
    }

    Json::Value ret;

    bool isFound=false;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from users where id='"+data["id"].as<std::string>()+"'");
    for (auto row : result)
    {
        isFound=true;
        ret["name"]=row["name"].as<std::string>();
        ret["role"]=row["role"].as<int>();

        if(data.find("token")&&data["token"].as<std::string>()==tokenOfAdmin::getInstance().get())
        {
            ret["password"]=row["password"].as<std::string>();
        }
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"获取用户失败，无该用户");
        return;
    }
    
    azh::drogon::returnTrue(callback,"获取用户成功",ret);
}

void User::getUnallocatedStudent(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"获取失败，未知的请求，请带上管理员token");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(data["token"].as<std::string>()!=tokenOfAdmin::getInstance().get())
    {
        azh::drogon::returnFalse(callback,"获取失败，管理员token无效，请重新登陆");
        return;
    }

    Json::Value ret;

    int count=0;

    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select id from users where role=0 and id not in (select id from student);");
    for (auto row : result)
    {
        ret[std::to_string(count)]=row["id"].as<std::string>();
        count++;
    }

    ret["count"]=count;

    if(count==0)
    {
        azh::drogon::returnTrue(callback,"获取成功，无任何未分配的用户",ret);
        return;
    }
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}
