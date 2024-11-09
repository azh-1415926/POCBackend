#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class Course : public drogon::HttpController<Course>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(Course::get, "/{2}/{1}", Get); // path is /Course/{arg2}/{arg1}
    // METHOD_ADD(Course::your_method_name, "/{1}/{2}/list", Get); // path is /Course/{arg1}/{arg2}/list
    // ADD_METHOD_TO(Course::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(Course::getOutline,"/getOutline",Get);
    METHOD_ADD(Course::getCourse,"/getCourse?chapter={1}&section={2}",Get);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void getOutline(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
    void getCourse(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback,const std::string& chapter,const std::string& section);
};
