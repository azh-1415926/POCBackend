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
    
    Json::Value data;
    std::vector<std::string> params={ "code" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    LOG_DEBUG<<"User compile data:"<<str;

    Json::Value code=azh::json::toJson(str.data());

    Json::Value ret;

    ret["output"]=compileCodeBy("compileC",code["code"].asString(),code["input"].asString());

    azh::drogon::returnTrue(callback,"编译成功",ret);
}


void Code::releaseExperiment(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "classId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

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
    
    Json::Value data;
    std::vector<std::string> params={ "studentId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback,ret))
        return;

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
    
    Json::Value data;
    std::vector<std::string> params={ "studentId" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback,ret))
        return;

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
    
    Json::Value data;
    std::vector<std::string> params={ "studentId","experimentId","code" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string studentId=data["studentId"].as<std::string>();
    std::string experimentId=data["experimentId"].as<std::string>();

    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    auto resultOfRecord=clientPtr->execSqlSync("select * from experiment_record where experiment_id="+experimentId+" and student_id='"+studentId+"'");

    bool isFound=false;

    for(auto row : resultOfRecord)
    {
        isFound=true;
    }

    if(isFound)
    {
        azh::drogon::returnFalse(callback,"提交失败，所提交实验已存在实验记录");
        return;
    }

    isFound=false;

    auto result=clientPtr->execSqlSync("select * from experiment where id="+experimentId);

    for (auto row : result)
    {
        isFound=true;
    }

    if(!isFound)
    {
        azh::drogon::returnFalse(callback,"提交失败，所提交实验不存在或已被删除");
        return;
    }

    clientPtr->execSqlSync("insert into experiment_record values(NULL,'"+studentId+"',"+experimentId+",'"+data["code"].as<std::string>()+"',NOW(),0,0)");
    // submit

    azh::drogon::returnTrue(callback,"提交成功");
}

void Code::getExperimentByStudent(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "id" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string studentId=data["id"].as<std::string>();

    Json::Value ret;
    auto clientPtr = drogon::app().getDbClient("POC");

    std::string query="SELECT e.id,e.name,e.content,er.code "
        "FROM experiment_record er "
        "LEFT JOIN experiment e ON er.experiment_id = e.id "
        "LEFT JOIN student s ON s.id = er.student_id "
        "WHERE s.id='"+studentId+"' and er.isCorrect=0;";

    const drogon::orm::Result &result=clientPtr->execSqlSync(query);

    int count=0;

    for (auto row : result)
    {
        Json::Value info;
        info["id"]=row["id"].as<std::string>();
        info["name"]=row["name"].as<std::string>();
        info["content"]=row["content"].as<std::string>();
        info["code"]=row["code"].as<std::string>();

        ret[std::to_string(count)]=info;

        count++;
    }

    ret["count"]=count;
    
    azh::drogon::returnTrue(callback,"获取成功",ret);
}

void Code::submitScore(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto str=req->getBody();
    
    Json::Value data;
    std::vector<std::string> params={ "studentId","experimentId","score" };
    
    if(!azh::drogon::checkParams(str.data(),params,data,callback))
        return;

    std::string studentId=data["studentId"].as<std::string>();
    std::string experimentId=data["experimentId"].as<std::string>();
    int score=data["score"].as<int>();

    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient("POC");

    clientPtr->execSqlSync("update experiment_record set score="+std::to_string(score)+",isCorrect=1 where student_id='"+studentId+"' and experiment_id="+experimentId+";");

    azh::drogon::returnTrue(callback,"提交成功",ret);
}
