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

std::string compileCodeBy(const std::string& code,const std::string& func)
{
    std::string ret;

    Py_Initialize();
    PyObject* pModule = PyImport_ImportModule("compile");
    if (pModule)
    {
        PyObject* pFunc = PyObject_GetAttrString(pModule, func.data());
        if (pFunc && PyCallable_Check(pFunc))
        {
            PyObject* pResult = PyObject_CallFunction(pFunc, "s", code.c_str());
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

    auto str=req->getBody();
    
    if(!str.empty())
    {
        std::cout<<"Compile data:"<<str;

        Json::Value code=toJson(str.data());

        std::cout<<compileCodeBy("World","hello")<<"\n";
        std::cout<<compileCodeBy(code["data"].asString(),"compileC")<<"\n";
    }
    else
    {
        std::cout<<"Compile data is empty\n";
    }

    Json::Value ret;

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    resp->setStatusCode(k200OK);
    resp->addHeader("Access-Control-Allow-Origin","*");

    callback(resp);
}
