// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
// 3.定义节点类；
class MyNode:public rclcpp::Node{
public:
    MyNode():Node("MyNode_node_cpp","t1_ns"){//有多个构造函数的重载，第一个是节点名，第二个参数传入命名空间
   // pub=this->create_publisher<std_msgs::msg::String>("/shi",10);//全局话题发布方，和命名空间以及节点名无关
    // pub=this->create_publisher<std_msgs::msg::String>("kaihui",10);//相对话题
    pub=this->create_publisher<std_msgs::msg::String>("~/vip",10);//私有
}
private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub;
};
int main(int argc, char const *argv[])
{
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);
// 4.调用spin函数，并传入节点对象指针；
rclcpp::spin(std::make_shared<MyNode>());
// 5.释放资源。
rclcpp::shutdown();
return 0;
}