#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class Quiz : public drogon::HttpController<Quiz>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(Quiz::get, "/{2}/{1}", Get); // path is /Quiz/{arg2}/{arg1}
    // METHOD_ADD(Quiz::your_method_name, "/{1}/{2}/list", Get); // path is /Quiz/{arg1}/{arg2}/list
    // ADD_METHOD_TO(Quiz::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(Quiz::getquiz,"/getquiz?chapter={1}",Get);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void getquiz(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback,const std::string& chapter);
};
