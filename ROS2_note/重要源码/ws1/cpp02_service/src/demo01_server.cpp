/*  
  需求：编写服务端，接收客户端发送请求，提取其中两个整型数据，相加后将结果响应回客户端。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建服务端；
      3-2.处理请求数据并响应结果。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。
*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"
using base_interfaces_demo::srv::AddInts;
using std::placeholders::_1;
using std::placeholders::_2;
// 3.定义节点类；
class AddIntsServer:public rclcpp::Node{
public:
AddIntsServer():Node("AddIntsServer_node_cpp"){
  RCLCPP_INFO(this->get_logger(),"服务端节点创建");
      // 3-1.创建服务端；
      // 模板参数：消息载体类型(服务接口类型)
      // 参数，话题名,回调函数(因为有两个参数，所以用两个占位符)
      // 返回服务对象指针
      server_ = this->create_service<AddInts>("add_ints",std::bind(&AddIntsServer::add,this,_1,_2));
     
}


private:
// 3-2.处理请求数据并响应结果。
// 回调函数参数固定,是请求和响应，我们用他们的指针，可以获取请求响的参数应进行处理
// 是一个回调函数，不可能自己创建对象去调用他，所以设置为私有
  void add(const AddInts::Request::SharedPtr req,const AddInts::Response::SharedPtr res){
    res->sum=req->num1+req->num2;//只要把和设置进res，ros2自动把response响应回客户端(就是检测变化吧)
    RCLCPP_INFO(this->get_logger(),"%d + %d =%d",req->num1,req->num2,res->sum);
  }
  rclcpp::Service<AddInts>::SharedPtr server_;//创建服务端对象
};
int main(int argc, char const *argv[])
{
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);
// 4.调用spin函数，并传入节点对象指针；
rclcpp::spin(std::make_shared<AddIntsServer>());
// 5.释放资源。
rclcpp::shutdown();
return 0;
}
