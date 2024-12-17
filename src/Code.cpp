#include "Code.h"

#include <iostream>  
#include <thread>  
#include <chrono>  
#include <atomic>  
#include <functional>

#include <Python.h>

Json::Value toJson(const std::string& str)
{
    Json::CharReaderBuilder readerBuilder;  
    Json::Value root;  
    std::string errs;  
    std::istringstream s(str);  
  
    bool parsingSuccessful = Json::parseFromStream(readerBuilder, s, &root, &errs);  
  
    if (!parsingSuccessful)
    {  
        std::cerr << "Failed to parse JSON: " << errs << std::endl;  
    }

    return root;  
}

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
    auto session=req->session();

    if(!session->find("token"))
    {
        LOG_DEBUG<<"Not login,error request.";
        // return;
    }
    else
    {
        LOG_DEBUG<<"User compile code.";
    }

    Json::Value ret;

    auto str=req->getBody();
    
    if(!str.empty())
    {
        std::cout<<"Compile data:"<<str;

        Json::Value code=toJson(str.data());

        // std::cout<<compileCodeBy("World","hello")<<"\n";
        ret["output"]=compileCodeBy("compileC",code["code"].asString(),code["input"].asString());
    }
    else
    {
        std::cout<<"Compile data is empty\n";
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");

    callback(resp);
}

void Code::getExperiment(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &studentId)
{
    Json::Value ret;

    if(studentId.empty())
    {
        ret["count"]=0;
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        resp->addHeader("Access-Control-Allow-Origin","*");

        callback(resp);
        return;
    }

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
    
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");
    callback(resp);
}

void Code::submitExperiment(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string &experimentId)
{
    auto json=req->getJsonObject();

    Json::Value ret;

    auto str=req->getBody();

    Json::Value code=toJson(str.data());

    if(experimentId.empty()||str.empty())
    {
        ret["success"] = "false";
        auto resp=HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
        resp->addHeader("Access-Control-Allow-Origin","*");

        callback(resp);
        return;
    }

    auto clientPtr = drogon::app().getDbClient("POC");

    auto result=clientPtr->execSqlSync("select * from experiment_record where id="+experimentId);

    bool isFound=false;

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
        ret["success"]="false";
    else
    {
        clientPtr->execSqlSync("update experiment_record set code='"+code["data"].asString()+"'"+" where id="+experimentId);
        clientPtr->execSqlSync("update experiment_record set isfinish=1 where id="+experimentId);
        ret["success"]="true";
    }
    
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");
    callback(resp);
}