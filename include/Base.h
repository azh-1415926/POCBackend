#pragma once

#define SINGLETONE(Name,Type) \
class Name \
{ \
    public: \
        static Name& getInstance() \
        { \
            static Name instance; \
            return instance; \
        } \
        Type get() {  return m_Instance; } \
        void set(const Type& data) { m_Instance=data; } \
    private: \
        explicit Name() { } \
        Type m_Instance; \
};

#include <string>

SINGLETONE(tokenOfAdmin,std::string)

namespace azh
{
    namespace json
    {
        inline Json::Value toJson(const std::string& str)
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
    }

    namespace drogon
    {
        inline void returnFalse(std::function<void(const HttpResponsePtr &)> &callback,const std::string& info,const Json::Value& ret=Json::Value())
        {
            Json::Value _ret=ret;

            _ret["result"]="false";
            _ret["info"]=info;
            auto resp=HttpResponse::newHttpJsonResponse(_ret);
            resp->setStatusCode(k200OK);
            resp->addHeader("Access-Control-Allow-Origin","*");
            callback(resp);
        }

        inline void returnTrue(std::function<void(const HttpResponsePtr &)> &callback,const std::string& info,const Json::Value& ret=Json::Value())
        {
            Json::Value _ret=ret;

            _ret["result"]="true";
            _ret["info"]=info;
            auto resp=HttpResponse::newHttpJsonResponse(_ret);
            resp->setStatusCode(k200OK);
            resp->addHeader("Access-Control-Allow-Origin","*");
            callback(resp);
        }

        inline bool checkParams(const std::string& str,const std::vector<std::string>& params,Json::Value& data,std::function<void(const HttpResponsePtr &)> &callback,const Json::Value& ret=Json::Value())
        {
            if(str.empty())
            {
                azh::drogon::returnFalse(callback,"未知的请求，请求格式有误");
                return false;
            }

            data=azh::json::toJson(str.data());

            for(const auto& str : params)
            {
                if(!data.find(str))
                {
                    azh::drogon::returnFalse(callback,"请求失败，请求参数'"+str+"'缺失");
                    return false;
                }
            }

            return true;
        }
    }
}