#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class Login : public drogon::HttpController<Login>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    METHOD_ADD(Login::login,"/token?userId={1}&passwd={2}",Post);
    METHOD_ADD(Login::getInfo,"/info?userId={1}",Get);
    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    void login(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback,
               std::string &&userId,
               const std::string &password);
    void getInfo(const HttpRequestPtr &req,
                 std::function<void (const HttpResponsePtr &)> &&callback,
                 const std::string &userId) const;
};