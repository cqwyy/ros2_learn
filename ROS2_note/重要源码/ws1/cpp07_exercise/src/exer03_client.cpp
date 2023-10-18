// 需求:客户端提交目标点坐标，并解析响应结果
// 解析动态传入的数据，作为目标点数据

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
using base_interfaces_demo::srv::Distance;
using namespace std::chrono_literals;

// 3.定义节点类；
class Exer03Client: public rclcpp::Node{
public:
    Exer03Client():Node("exer03_client_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"案例2客户端创建");
    // 3-1:沟在函数创建客户端
    client_ = this->create_client<Distance>("distance");
    }
    // 3-2 客户端连接服务端
    bool connect_server(){
        while(!client_->wait_for_service(1s)){
            if(!rclcpp::ok()){
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"节点强制退出");
                return false;
            }
            RCLCPP_INFO(this->get_logger(),"连接中");
        }
        return true;
    }
    // 3-3 发送请求数据
    rclcpp::Client<Distance>::FutureAndRequestId send_goal(float x , float y,float theta){
        auto request = std::make_shared<Distance::Request>();
        request->x=x;
        request->y=y;
        request->theta=theta;
        return client_->async_send_request(request);
    }
private:
    rclcpp::Client<Distance>::SharedPtr client_;
};
int main(int argc, char const *argv[])
{
    // 解析动态传入的数据，作为目标点数据
    if(argc!=5){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"请提交三个参数");//括号里是给日志管理器起名
        return 1;
    }
    //解析提交的数据
    float goal_x = atof(argv[1]);
    float goal_y = atof(argv[2]);
    float goal_theta = atof(argv[3]);

    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // // 4.调用spin函数，并传入节点对象指针；
    // rclcpp::spin(std::make_shared<Exer03Client>());

    //  4.调用节点对象指针的相关函数
    auto client = std::make_shared<Exer03Client>();
    bool flag = client->connect_server();
    if(!flag){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务连接失败");
        return 1;
    }
    // 发送请求，并处理响应
    auto future = client->send_goal(goal_x,goal_y,goal_theta);
    // 判断响应结果状态
    if(rclcpp::spin_until_future_complete(client,future)==rclcpp::FutureReturnCode::SUCCESS){
        RCLCPP_INFO(client->get_logger(),"两值乌龟距离%.2f",future.get()->distance);
    }
    else{
        RCLCPP_INFO(client->get_logger(),"服务响应失败");
    }


    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}