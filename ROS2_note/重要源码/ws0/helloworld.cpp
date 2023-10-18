


//  1.包含头文件
#include "rclcpp/rclcpp.hpp"

// 方式一：不推荐
//  int main(int argc, char ** argv)
//  {
//    // 2.初始化 ROS2客户端--初始化必然有释放
//    rclcpp::init(argc,argv);//参数是main函数里的参数
//    // 3.创建节点指针
//    auto node = rclcpp::Node::make_shared("helloworld_node");//创建Node对象的指针,参数是节点的名字
//    // 4.输出文本(日志)
//    RCLCPP_INFO(node->get_logger(),"hello world!");//参数一是日志对象(节点下面有个函数可以获取日志对象),参数二是输出的文本
//    // 5.释放资源
//    rclcpp::shutdown();
//    return 0;
//  }
//  6. vscode直接配置的话：ctrl+~打开终端 ros2 pkg create pkg03_hellovscode_cpp --build-type ament_cmake --dependencies rclcpp --node-name hellvscode



// 方式二

//自定义类，继承Node
class MyNode:public rclcpp::Node{
  public:
    MyNode():Node("hello_node_cpp"){//直接调用Node的构造函数给节点命名
      RCLCPP_INFO(this->get_logger(),"hello world_继承");//构造函数输出日志

    }

};

int main(int argc, char const *argv[])
{
  
  //初始化
  rclcpp::init(argc,argv);
  //实例化自定义类
  auto node =std::make_shared<MyNode>();
  // 资源释放
  rclcpp::shutdown();
  return 0;
}
//初始化与释放资源在程序中的作用：
// 构建的程序可能由若干步骤或阶段组成，不同步骤和阶段之间涉及到数据传递。ROS2使用
// Context（上下文）对象，把他看成一个流转在不同步骤的容器，每一个步骤产生的数据
// 放到里面，用就从里面拿。初始化就创建了一个context对象，资源释放就销毁这个对象
// 
