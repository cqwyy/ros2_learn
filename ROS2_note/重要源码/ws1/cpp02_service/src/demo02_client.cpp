/*  
  需求：编写客户端，发送两个整型变量作为请求数据，并处理响应结果。
  步骤：
     前提：main函数中需要判断提交的参数是否正确
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建客户端；
      3-2.等待服务连接；（对于服务通信，应该满足如果客户端连接不到服务器，就不发送请求）
      3-3.组织请求数据并发送；
    4.创建对象指针调用其功能（调用连接服务的函数）,（根据连接结果进行下一步处理）并处理响应；
    （连接服务后，调用请求发送函数，接收并处理响应结果）
    5.释放资源。

*/



// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/add_ints.hpp"
using base_interfaces_demo::srv::AddInts;
using namespace std::chrono_literals;
// 3.定义节点类；
class AddIntsClient:public rclcpp::Node{
public:
AddIntsClient():Node("AddIntsClient_node_cpp"){
     RCLCPP_INFO(this->get_logger(),"客户端节点创建");
     //  3-1.创建客户端；(服务消息载体，服务话题名，返回服务对象指针)
     client_ = this->create_client<AddInts>("add_ints");

}
     //  3-2.等待服务连接；（对于服务通信，应该满足如果客户端连接不到服务器，就不发送请求）
     bool connect_server(){//自定义连接服务器实现，供外部调用，连接成功返回true
          // client_->wait_for_service(1s);//在等待超时时间内连接服务器，参数是等待时间，返回连接结果true/flase
          while(!client_->wait_for_service(1s)){//循环以1s为超时时间连接服务器，连不上就一直连
               // ctrl+c有异常，会无间断执行下面的打印
              
               //并且按下ctrl+c是结束ros2程序，意味着要释放资源，比如关闭context
               // (context是初始化客户端时候创建的，有shotdown释放，存储着所有资源
               // )这个都关闭了，那里面的资源肯定没有了，所以执行打印（或其他）时候用到在这期间
               // 创建的资源，比如客户端节点，肯定会报错
               // 所以用rclcpp打印（rclcpp的get_logger对象不在context里（这个程序在客户端初始化之前就有日志输出也可以看出来这一点））

               if(!rclcpp::ok()){//判断程序是否正常执行中，不正常执行比如按下ctrl+c，就返回flase
                    RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"强行终止客户端");
                    return false;//返回连接失败
               }
               RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务连接中");
          }
          return true;
     }
     //  3-3.组织请求数据并发送；
     // 参数是两个整形数据，返回服务端的返回结果
     rclcpp::Client<AddInts>::FutureAndRequestId send_request(int num1 , int num2){
          // 组织请求数据
          auto request = std::make_shared<AddInts::Request>();//创建数据，之前话题通信都是一个类型，这里有请求响应两种，所以多了一级
          request->num1=num1;
          request->num2=num2;
          // 发送请求，并返回请求结果（这个请求结果包含很多东西，包括响应状态，响应对象的指针（响应数据就在这里面）等）
          return client_->async_send_request(request);//参数是AddInts_Request类型，就相当于AddInts::Request
     }

private:
     rclcpp::Client<AddInts>::SharedPtr client_;
};
int main(int argc, char const *argv[])
// 在调用可执行程序的时候有参数输入，argc表示参数个数，argv是参数列表
{
     if (argc != 3){//这里是3,因为0位置默认是运行路径名
          RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"请提交两个整形数字");//没有节点对象，就用rclcpp获取日志，不过要有日志管理器名
          // 用错误日志对象打印
          return 1;
     }
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);


// 4.调用spin函数，并传入节点对象指针；---这里不用spin
// rclcpp::spin(std::make_shared<Listener>());
// 4.创建客户端（节点）对象（不用挂起）
// 客户端不用一起挂起，因为客户端请求发送一次，服务完就行了，没必要挂起（挂起也没用，请求就请求一次）。（ros2里也没法挂起，spin轮询处理回调进而产生挂起，客户端里没有回调函数，所以没法挂起）
// 所以不用把对象放到spin里面轮询了
auto client = std::make_shared<AddIntsClient>();//创建客户端对象时候，构造函数里创造ROS2的真客户端对象，这里说为客户端节点对象更贴切

// 4.1调用客户端对象的连接服务器功能
bool flag = client->connect_server();
// 4.2根据连接结果进一步处理
// 连接失败
if(!flag){
     RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"连接失败，程序退出");
     return 0;
}
// 连接成功
// 调用请求发送函数，接收响应结果
auto future = client->send_request(atoi(argv[1]),atoi(argv[2]));//atoi就是转为整形
// 处理响应结果
if(rclcpp::spin_until_future_complete(client,future)==rclcpp::FutureReturnCode::SUCCESS){//成功响应
     // 返回结果一共三种状态，成功，中断，超时。在返回的结果里可以获取状态码INTERUPTED（中断），TIMEOUT（超时）
     // 获取状态码的函数传入当前节点指针以及返回的请求结果
     
     RCLCPP_INFO(client->get_logger(),"响应成功 sum=%d",future.get()->sum);
     //响应结果的值获取：返回结果future里面有响应对象的指针--future.get(),响应值就在这里面
}

else{//失败，超时
     RCLCPP_INFO(client->get_logger(),"响应失败");
}

// 5.释放资源。
rclcpp::shutdown();
return 0;
}