//  演示参数API使用
/*
    需求：编写参数服务端，设置并操作参数。
    步骤：
        1.包含头文件；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.参数对象创建
            3-2.参数对象解析（获取键，值，将值转化为字符串。。）
        4.创建节点对象指针，调用参数操作函数，并传递给spin函数；
        5.释放资源。

*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"

// 3.定义节点类；
class Myparam:public rclcpp::Node{
public:
Myparam():Node("Myparam_node_cpp"){
  RCLCPP_INFO(this->get_logger(),"参数API使用");
  // 3-1.参数对象创建
  rclcpp::Parameter p1("car_name","togger");//键是字符串类型，值可以是规定类型
  rclcpp::Parameter p2("height",1.68);
  rclcpp::Parameter p3("wheels",4);

  // 3-2.参数对象解析（获取键，值，将值转化为字符串。。）
  // 获取值
  RCLCPP_INFO(this->get_logger(),"car_name=%s",p1.as_string().c_str());//获取值，以字符串形式(要打印还要转化成c的形式)
  RCLCPP_INFO(this->get_logger(),"car_name=%.2f",p2.as_double());//值是什么类型就用什么类型输出
  RCLCPP_INFO(this->get_logger(),"car_name=%ld",p3.as_int());//int64

  // 获取键
  RCLCPP_INFO(this->get_logger(),"name=%s",p1.get_name().c_str());
  // 获取参数值的数据类型
  RCLCPP_INFO(this->get_logger(),"typename=%s",p1.get_type_name().c_str());
  // 把值转为字符串类型
  RCLCPP_INFO(this->get_logger(),"value2string=%s",p2.value_to_string().c_str());
  

  }
};
int main(int argc, char const *argv[])
{
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);
// 4.调用spin函数，并传入节点对象指针；
rclcpp::spin(std::make_shared<Myparam>());
// 5.释放资源。
rclcpp::shutdown();
return 0;
}