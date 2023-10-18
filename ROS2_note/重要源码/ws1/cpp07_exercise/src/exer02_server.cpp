// 解析客户端提交的目标点坐标，（创建订阅方）获取原生乌龟坐标，计算二者距离 ，响应回客户端


// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
using std::placeholders::_1;
using std::placeholders::_2;
// 3.定义节点类；
class Exer02Server:public rclcpp::Node{
public:
    Exer02Server():Node("exer02_server_node_cpp"),x(0.0),y(0.0){
        RCLCPP_INFO(this->get_logger(),"案例2服务端创建了");
        // 3-1 创建订阅方，获取原生乌龟位姿  话题/turtle/pose  消息turtlesim/msg/Pose
        sub_=this->create_subscription<turtlesim::msg::Pose>("/turtle/pose",10,std::bind(&Exer02Server::pose_cb,this,_1));
        // 3-2创建服务端
        server_ = this->create_service<base_interfaces_demo::srv::Distance>("distance",std::bind(&Exer02Server::distance_cb,this,_1,_2));//服务名以及回调函数

    }
private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_;
    float x,y;//成员变量
    
    rclcpp::Service<base_interfaces_demo::srv::Distance>::SharedPtr server_;


    void pose_cb(const turtlesim::msg::Pose::SharedPtr pose){//这里用的指针，引用也可以（知道怎么用到指针）
        x=pose->x;
        y=pose->y;

    }

    void distance_cb(const base_interfaces_demo::srv::Distance::Request::SharedPtr request,base_interfaces_demo::srv::Distance::Response::SharedPtr response){//两个参数，请求对象以及响应对象
        // 1.解析出目标点坐标
        float goal_x = request->x;
        float goal_y = request->y;

        // 2.计算距离
        float distance_x = goal_x-x;
        float distance_y = goal_y-y;
        float distance = std::sqrt(distance_x * distance_x +distance_y * distance_y);

        // 3.设置进响应
        response->distance=distance;
        RCLCPP_INFO(this->get_logger(),"目标点坐标（%.2f,%.2f）,距离%.2f",goal_x,goal_y,distance);

    }
    
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<Exer02Server>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}