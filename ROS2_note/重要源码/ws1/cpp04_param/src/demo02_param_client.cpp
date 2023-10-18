/*
    需求：编写参数客户端，获取或修改服务端参数。
    步骤：
        1.包含头文件；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1 创建参数客户端对象
            3-2 连接服务端
            3-3.查询参数；
            3-4.修改参数；
        4.创建节点对象指针，调用参数操作函数；
        5.释放资源。
*/


// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;
// 3.定义节点类；
class ParamClient:public rclcpp::Node{
public:
ParamClient():Node("ParamClient_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"参数客户端创建");
    // 3-1 创建参数客户端对象
    //参数是当前对象依赖的节点和要连接的节点名（参数服务端节点名）
    param_client_ = std::make_shared<rclcpp::SyncParametersClient>(this,"ParamServer_node_cpp");
    // 服务通信是通过话题关联，但是参数客户端这里，通过参数服务中的节点名关联
    // 1.参数服务端启动后，底层封装了多个服务通信的服务端
    // 2.每个服务端的话题，都是采用/服务端节点名/xxx命名
    // 3.参数客户端创建后，也会封装多个服务通信的客户端
    // 4.这些客户端与服务端相呼应，也要用相同的话题，因为 后面的xxx固定，所以就通过服务端节点名来找服务端话题
}
    // 3-2 连接服务端
    bool connect_server(){
        while(!param_client_->wait_for_service(1s)){
            //等待一秒钟，没连上就返回false，然后while一直等连接上才退出
            if(!rclcpp::ok()){//客户端ctrl+c或其他形式退出
                RCLCPP_INFO(this->get_logger(),"服务连接失败");
                return false;
            }
            RCLCPP_INFO(this->get_logger(),"服务连接中");
        }
        return true;
    }
    // 3-3.查询参数；
    void get_param(){
        RCLCPP_INFO(this->get_logger(),"参数查询");
        // 说明，有很多函数可用，这里只是演示常用的，到时候根据情况查函数用
        // 这里比服务端多了模板
        std::string car_name=param_client_->get_parameter<std::string>("car_name");//模板是获取参数的类型,参数是key，返回模板类型
        // 打印要转c_str()
        double width=param_client_->get_parameter<double>("width");
        RCLCPP_INFO(this->get_logger(),"car_name=%s",car_name.c_str());
        RCLCPP_INFO(this->get_logger(),"width=%.2f",width);

        // 获取多个
        auto params = param_client_->get_parameters({"car_name","width","wheels"});//传入多个key,返回参数对象Parameter的vector
        for(auto &&param:params){
            RCLCPP_INFO(this->get_logger(),"(%s=%s)",param.get_name().c_str(),param.value_to_string().c_str());
        }

        // 判断是否包含某个参数
        RCLCPP_INFO(this->get_logger(),"包含car_name?%d",param_client_->has_parameter("car_name"));
    }
    // 3-4.修改参数；
    void updata_param(){
        RCLCPP_INFO(this->get_logger(),"参数修改");
        //传入多个Parameter对象,同样可以传服务端没有的key,相当于新建了，但是也是要保证服务端声明允许删除（构造函数声明的那个）
        // rclcpp::NodeOptions().allow_undeclared_parameters(true)
        param_client_->set_parameters({rclcpp::Parameter("car_name","pig"),rclcpp::Parameter("width",3.0)});
    }
private:
    rclcpp::SyncParametersClient::SharedPtr param_client_;//声明同步的参数客户端
};

int main(int argc, char const *argv[])
{
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);
// // 4.调用spin函数，并传入节点对象指针；
// rclcpp::spin(std::make_shared<ParamClient>());

// 4.创建节点对象指针，调用参数操作函数；（客户端访问完就退出，不需要spin）
auto client = std::make_shared<ParamClient>();
bool flag = client->connect_server();

if (!flag){
    rclcpp::shutdown();
    return 0;
}

client->get_param();
client->updata_param();
client->get_param();

// 5.释放资源。
rclcpp::shutdown();
return 0;
}