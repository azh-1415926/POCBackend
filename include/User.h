#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class User : public drogon::HttpController<User>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    METHOD_ADD(User::login,"/login",Post);
    METHOD_ADD(User::info,"/info",Post);
    METHOD_LIST_END
    // your declaration of processing function maybe like this:

    void login(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);

    void info(const HttpRequestPtr &req,
                 std::function<void (const HttpResponsePtr &)> &&callback) const;
};