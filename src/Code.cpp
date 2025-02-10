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


void Code::releaseExperiment(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnFalse(callback,"未带上任何数据，无法添加实验");
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("classId"))
    {
        azh::drogon::returnFalse(callback,"请带上班级id，以提交实验数据");
        return;
    }

    std::string classId=data["classId"].as<std::string>();

    Json::Value ret;
    bool isFound=false;

    auto clientPtr = drogon::app().getDbClient("POC");

    auto result=clientPtr->execSqlSync("select * from class where id='"+classId+"'");

    for(auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"提交失败，班级不存在");
        return;
    }
    
    clientPtr->execSqlSync("insert into experiment values(NULL,'"+classId+"','"+data["name"].as<std::string>()+"','"+data["content"].as<std::string>()+"',NOW())");

    azh::drogon::returnTrue(callback,"添加成功");
}

void Code::getUnfinishedExperiment(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    Json::Value ret;
    ret["count"]=0;

    auto str=req->getBody();
    
    if(str.empty())
    {
        azh::drogon::returnTrue(callback,"获取失败，未知的请求",ret);
        return;
    }

    Json::Value data=azh::json::toJson(str.data());

    if(!data.find("studentId"))
    {
        azh::drogon::returnTrue(callback,"请带上学生id，以请求实验数据",ret);
        return;
    }

    std::string studentId=data["studentId"].as<std::string>();
    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT e.* "
        "FROM experiment e "
        "LEFT JOIN experiment_record er ON e.id = er.experiment_id "
        "LEFT JOIN student s ON e.class_id = s.class_id AND er.student_id = s.id AND er.student_id = '"+studentId+"' "
        "WHERE er.id IS NULL;";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    int count=result.size();

    for (int i=0;i<count;i++)
    {
        Json::Value experiment;

        const std::string& experimentId=result.at(i)["id"].as<std::string>();

        const drogon::orm::Result &resultOfInfo=clientPtr->execSqlSync("select * from experiment where id='"+experimentId+"'");
        
        experiment["experimentId"]=experimentId;
        experiment["name"]=resultOfInfo.at(0)["name"].as<std::string>();
        experiment["content"]=resultOfInfo.at(0)["content"].as<std::string>();
        experiment["createTime"]=resultOfInfo.at(0)["create_time"].as<std::string>();

        ret[std::to_string(i)]=experiment;
    }

    ret["count"]=count;

    azh::drogon::returnTrue(callback,"获取成功",ret);
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

        const std::string& experimentId=result.at(i)["experiment_id"].as<std::string>();

        const drogon::orm::Result &resultOfInfo=clientPtr->execSqlSync("select * from experiment where id='"+experimentId+"'");
        
        experiment["id"]=result.at(i)["id"].as<int>();
        experiment["studentId"]=result.at(i)["student_id"].as<std::string>();
        experiment["experimentId"]=experimentId;
        experiment["name"]=resultOfInfo.at(0)["name"].as<std::string>();
        experiment["content"]=resultOfInfo.at(0)["content"].as<std::string>();
        experiment["code"]=result.at(i)["code"].as<std::string>();
        experiment["createTime"]=resultOfInfo.at(0)["create_time"].as<std::string>();
        experiment["finishTime"]=result.at(i)["finish_time"].as<std::string>();

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

    if(!data.find("studentId"))
    {
        azh::drogon::returnFalse(callback,"请带上学生id，以提交实验数据");
        return;
    }

    if(!data.find("experimentId"))
    {
        azh::drogon::returnFalse(callback,"请带上实验id，以提交实验数据");
        return;
    }

    if(!data.find("code"))
    {
        azh::drogon::returnFalse(callback,"请带上代码，以提交实验数据");
        return;
    }

    std::string studentId=data["studentId"].as<std::string>();
    std::string experimentId=data["experimentId"].as<std::string>();

    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    auto result=clientPtr->execSqlSync("select * from experiment where id="+experimentId);

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

    clientPtr->execSqlSync("insert into experiment_record values(NULL,'"+studentId+"','"+experimentId+"','"+data["code"].as<std::string>()+"',NOW())");
    // submit

    azh::drogon::returnTrue(callback,"提交成功");
}