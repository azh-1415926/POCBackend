#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class Class : public drogon::HttpController<Class>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(Class::get, "/{2}/{1}", Get); // path is /Class/{arg2}/{arg1}
    // METHOD_ADD(Class::your_method_name, "/{1}/{2}/list", Get); // path is /Class/{arg1}/{arg2}/list
    // ADD_METHOD_TO(Class::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(Class::info,"/info",Post);
    METHOD_ADD(Class::allocate,"/allocate",Post);
    METHOD_ADD(Class::getClassByTeacher,"/getClassByTeacher",Post);
    METHOD_ADD(Class::getStudentByClass,"/getStudentByClass",Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void info(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);

    void allocate(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
    
    void getClassByTeacher(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
    void getStudentByClass(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
};
