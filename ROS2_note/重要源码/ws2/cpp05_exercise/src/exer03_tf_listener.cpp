// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;
// 需求:监听坐标变换广播数据,并生成turtle1相对于t2的坐标数据,进而生成控制turtle2运动的速度指令


// 3.定义节点类；

class Exer03TFListener:public rclcpp::Node{
public:
    Exer03TFListener():Node("exer03_tf_listener_node_cpp"){
        // 3.1声明参数服务(动态输入坐标系)
        this->declare_parameter("father_frame","turtle2");//程序控制乌龟2,所以程序的视角中2是父坐标系,算得1相对于2的相对位姿(2跟随1,所以要算1对2的相对关系,2一定是父)
        this->declare_parameter("child_frame","turtle1");
        father_frame = this->get_parameter("father_frame").as_string();
        child_frame = this->get_parameter("child_frame").as_string();
        // 3.2创建缓存
        buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        // 3.3创建监听器
        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_);
        // 3.4创建速度发布方
        cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/"+father_frame+"/cmd_vel",10);//控制第二只乌龟
        // 3.5创建定时器,实现坐标变换,并组织和发布速度指令
        timer_ = this->create_wall_timer(1s,std::bind(&Exer03TFListener::on_timer,this));

    }
private:
void on_timer(){
    try{
    // 1.实现坐标变换
    auto ts = buffer_->lookupTransform(father_frame,child_frame,tf2::TimePointZero);
    // 2.组织并发布速度指令
    geometry_msgs::msg::Twist twist;
    twist.linear.x = 0.5 * sqrt(pow(ts.transform.translation.x,2) + pow(ts.transform.translation.y,2));
    twist.angular.z = 1.0*atan2(ts.transform.translation.y,ts.transform.translation.x);//以弧度为单位

    cmd_pub_->publish(twist);
    }
    catch(const tf2::LookupException& e){
        RCLCPP_INFO(this->get_logger(),"异常提示:%s",e.what());//一般是有个坐标系没有启动导致监听不到,从而异常
    }
}
    std::string father_frame;
    std::string child_frame;
    std::shared_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::TimerBase::SharedPtr timer_;


};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<Exer03TFListener>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}