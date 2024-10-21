#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>

using namespace drogon;

int main()
{
    app().addListener("127.0.0.1", 8848);
    
    app().loadConfigFile("./res/conf/config.json").run();
}