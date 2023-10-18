
 //  6. vscode直接配置的话：ctrl+~打开终端 ros2 pkg create pkg03_hellovscode_cpp --build-type ament_cmake --dependencies rclcpp --node-name hellvscode
// 。。。然后其他步骤也都一样，因为编译是编译整个工作空间，所以可以选择功能包编译 colcon build --packages-select pkg03_hellvscode_cpp 多个的话空格隔开
//  1.包含头文件
#include "rclcpp/rclcpp.hpp"

 int main(int argc, char ** argv)
 {
   // 2.初始化 ROS2客户端--初始化必然有释放
   rclcpp::init(argc,argv);//参数是main函数里的参数
   // 3.创建节点指针
   auto node = rclcpp::Node::make_shared("helloworld_node");//创建Node对象的指针,参数是节点的名字
   // 4.输出文本(日志)
   RCLCPP_INFO(node->get_logger(),"hello world22222!");//参数一是日志对象(节点下面有个函数可以获取日志对象),参数二是输出的文本
   // 5.释放资源
   rclcpp::shutdown();
   return 0;
 }
