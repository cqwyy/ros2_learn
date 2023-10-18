/*
需求：订阅乌龟1的位姿信息，解析出线速度和角速度，生成并发布控制乌龟二运动的速度指令
    订阅话题：/turtle1/pose
    订阅消息：turtlesim/msg/Pose
    发布话题：/t2/turtle1/cmd_vel
    发布消息：geometry_msgs/msg/Twist

    BUG:乌龟一后退时，线速度仍然为正，导致错误---只能改源码修改
*/



// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"//这两个订阅发布的消息类型根据之前学的目录结构以及这里得知的消息类型就知道导入什么了
// 3.定义节点类；
class Exer01PubSub:public rclcpp::Node{
public:
    Exer01PubSub():Node("exer01_pub_sub_node_cpp"){
      RCLCPP_INFO(this->get_logger(),"案例一对象创建");
      // 3-1：创建发布方以及订约方
      pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/t2/turtle1/cmd_vel",10);//模板参数：消息类型，参数是话题名以及队列长度
      sub_ = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&Exer01PubSub::pose_cb,this,std::placeholders::_1));//模板参数是消息类型，参数是话题，队列长度，回调函数

  }
    
private:
  void pose_cb(const turtlesim::msg::Pose & pose){//这个参数，订阅的什么类型的消息就是什么类型,这里引用接收
      // 3-2订阅方回调函数来处理订阅到的速度，并生成控制新乌龟运动的速度指令，然后发布
      //   创建新的速度指令
      geometry_msgs::msg::Twist twist;

      // 解析数据
      twist.linear.x=pose.linear_velocity;
      twist.angular.z=-pose.angular_velocity;

      //    发布
      pub_->publish(twist);
  }
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<Exer01PubSub>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}