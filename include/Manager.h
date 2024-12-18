#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class Manager : public drogon::HttpController<Manager>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(Manager::get, "/{2}/{1}", Get); // path is /Manager/{arg2}/{arg1}
    // METHOD_ADD(Manager::your_method_name, "/{1}/{2}/list", Get); // path is /Manager/{arg1}/{arg2}/list
    // ADD_METHOD_TO(Manager::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(Manager::isValid,"/isValid?userId={1}",Get);
    METHOD_ADD(Manager::addUser,"/addUser",Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void isValid(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback,const std::string& userId);

    void addUser(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
};
