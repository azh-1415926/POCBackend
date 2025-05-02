#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class Code : public drogon::HttpController<Code>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(Code::get, "/{2}/{1}", Get); // path is /Code/{arg2}/{arg1}
    // METHOD_ADD(Code::your_method_name, "/{1}/{2}/list", Get); // path is /Code/{arg1}/{arg2}/list
    // ADD_METHOD_TO(Code::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    METHOD_ADD(Code::compile,"/compile",Post);
    METHOD_ADD(Code::releaseExperiment,"/releaseExperiment",Post);
    METHOD_ADD(Code::getUnfinishedExperiment,"/getUnfinishedExperiment",Post);
    METHOD_ADD(Code::getExperiment,"/getExperiment",Post);
    METHOD_ADD(Code::submitExperiment,"/submit",Post);
    METHOD_ADD(Code::getExperimentByStudent,"/getExperimentByStudent",Post);
    METHOD_ADD(Code::submitScore,"/submitScore",Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void compile(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
    void releaseExperiment(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback);
    void getUnfinishedExperiment(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
    void getExperiment(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback);
    void submitExperiment(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
    void getExperimentByStudent(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
    void submitScore(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback);
};
