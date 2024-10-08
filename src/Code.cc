#include "Code.h"

#include <iostream>  
#include <thread>  
#include <chrono>  
#include <atomic>  
#include <functional> 

void Code::compile(const HttpRequestPtr & req, std::function<void(const HttpResponsePtr&)>&& callback)
{
    auto session=req->session();

    if(!session->find("token"))
    {
        LOG_DEBUG<<"Not login,error request.";
        return;
    }
    else
    {
        LOG_DEBUG<<"User compile code.";
    }

    auto str=req->getBody();
    
    if(!str.empty())
    {
        std::cout<<"Compile data:"<<str;

        std::chrono::seconds timeout(10);  
        std::string command = "echo hello"; // 替换为实际命令  
    
        
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
