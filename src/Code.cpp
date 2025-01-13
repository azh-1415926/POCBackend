#include "Code.h"

#include <iostream>  
#include <thread>  
#include <chrono>  
#include <atomic>  
#include <functional>

#include <Python.h>

#include <Base.h>

std::string compileCodeBy(const std::string& func,const std::string& code,const std::string& input)
{
    std::string ret;

    Py_Initialize();
    // 创建一个Python列表对象来存储sys.path  
    PyObject *sys_path = PySys_GetObject("path");  
    if (!sys_path) {  
        PyErr_Print();  
        Py_Finalize();  
        return "";  
    }  
  
    // 插入新的路径到sys.path中  
    const char *new_path = "./res/script";  
    PyObject *path_item = PyUnicode_FromString(new_path);  
    if (!path_item) {  
        PyErr_Print();  
        Py_Finalize();  
        return "";  
    }  
    PyList_Insert(sys_path, 0, path_item);  // 在列表的开头插入新路径  
    Py_DECREF(path_item);  // 不再需要path_item的额外引用
    PyObject* pModule = PyImport_ImportModule("compile");
    if (pModule)
    {
        PyObject* pFunc = PyObject_GetAttrString(pModule, func.data());
        if (pFunc && PyCallable_Check(pFunc))
        {
            PyObject* pResult = PyObject_CallFunction(pFunc, "ss", code.c_str(),input.c_str());
            if (pResult)
            {
                if (PyUnicode_Check(pResult))
                {
                    const char* result = PyUnicode_AsUTF8(pResult);

                    ret=result;
                }
                Py_DECREF(pResult);
            }
            Py_DECREF(pFunc);
        }
        Py_DECREF(pModule);
    }
    Py_Finalize();

    return ret;
}

void Code::compile(const HttpRequestPtr & req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"编译失败，输入为空，请传入代码文本和程序输入文本");
        return;
    }

    LOG_DEBUG<<"User compile data:"<<str;

    Json::Value code=azh::json::toJson(str.data());

    if(code["code"].empty())
    {
        azh::drogon::returnFalse(callback,"编译失败，输入代码文本为空");
        return;
    }

    Json::Value ret;

    ret["output"]=compileCodeBy("compileC",code["code"].asString(),code["input"].asString());

    azh::drogon::returnTrue(callback,"编译成功",ret);
}

void Code::getExperiment(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value ret;
    ret["count"]=0;

    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"获取失败，未知的请求",ret);
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("studentId"))
    {
        azh::drogon::returnFalse(callback,"请带上学生id，以请求实验数据",ret);
        return;
    }

    std::string studentId=data["studentId"].as<std::string>();
    auto clientPtr = drogon::app().getDbClient("POC");

    const drogon::orm::Result &result=clientPtr->execSqlSync("select * from experiment_record where student_id='"+studentId+"'");

    int count=result.size();

    for (int i=0;i<count;i++)
    {
        Json::Value experiment;
        
        experiment["id"]=result.at(i)["id"].as<int>();
        experiment["student_id"]=result.at(i)["student_id"].as<std::string>();
        experiment["teacher_id"]=result.at(i)["teacher_id"].as<std::string>();
        experiment["name"]=result.at(i)["name"].as<std::string>();
        experiment["content"]=result.at(i)["content"].as<std::string>();
        experiment["isfinish"]=result.at(i)["isfinish"].as<int>();
        experiment["code"]=result.at(i)["code"].as<std::string>();

        ret[std::to_string(i)]=experiment;
    }

    ret["count"]=count;
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Code::submitExperiment(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"未带上任何数据，无法提交实验");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("experimentId"))
    {
        azh::drogon::returnFalse(callback,"请带上实验id，以提交实验数据");
        return;
    }

    std::string experimentId=data["experimentId"].as<std::string>();

    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    auto result=clientPtr->execSqlSync("select * from experiment_record where id="+experimentId);

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"提交失败，所提交实验不存在或已被删除");
        return;
    }
    
    clientPtr->execSqlSync("update experiment_record set code='"+data["code"].asString()+"'"+" where id="+experimentId);
    clientPtr->execSqlSync("update experiment_record set isfinish=1 where id="+experimentId);
    
    // submit

    azh::drogon::returnTrue(callback,"提交成功");
}