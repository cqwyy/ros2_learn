// 1.包含头文件；
// 需求:发送请求,生成一只新的乌龟

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

// 3.定义节点类；
class Exer01Spawn:public rclcpp::Node{
public:
    Exer01Spawn():Node("exer01_spawn_node_cpp"){
      // 3.1 使用参数服务声明新的乌龟信息(不写死,更灵活--调用节点的时候可以动态传参)
      this->declare_parameter("x",3.0);//声明参数服务,第二个参数是默认值
      this->declare_parameter("y",3.0);
      this->declare_parameter("theta",0.0);
      this->declare_parameter("turtle_name","turtle2");

      x = this->get_parameter("x").as_double();
      y = this->get_parameter("y").as_double();
      theta = this->get_parameter("theta").as_double();
      turtle_name = this->get_parameter("turtle_name").as_string();
      // 3.2创建服务客户端
      spawn_client_ = this->create_client<turtlesim::srv::Spawn>("/spawn");//参数是话题名
          }
    // 3.3连接服务端的接口
    bool connect_server(){
      while(!spawn_client_->wait_for_service(1s)){//超时时间1s,连不上就一直连
        if(!rclcpp::ok()){//没有等待连接成功就放弃连接(终端结束了),走这个语句
          RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"强制退出");//终端没了.就没有this了

          return false;

        }
        RCLCPP_INFO(this->get_logger(),"服务连接中");
      }
      return true;
    }
    // 3.4组织要发送数据,并提供发送接口
    rclcpp::Client<turtlesim::srv::Spawn>::FutureAndRequestId request(){
      auto req = std::make_shared<turtlesim::srv::Spawn::Request>();
      req->x = x;//不用声明,因为req用的srv::Spawn创建,所以里面有相应的srv
      req->y = y;
      req->theta = theta;
      req->name = turtle_name;
      
      return spawn_client_->async_send_request(req);
    }
private:
  double_t x,y,theta;
  std::string turtle_name;
  rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn_client_;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 调用spin函数，并传入节点对象指针；(服务客户端发送完请求之后,任务就完成了,节点就可以销毁了,没必要回调)
    // rclcpp::spin(std::make_shared<Exer01Spawn>());

    // 4.创建自定义节点类的对象,连接服务端,发送请求,处理响应结果
    auto client_ = std::make_shared<Exer01Spawn>();
    bool flag = client_->connect_server();
    if(!flag){
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"服务连接失败");
      return 1;//连接失败直接退出
    }
    //发送请求
    auto response = client_->request();
    //处理响应
    if(rclcpp::spin_until_future_complete(client_,response) == rclcpp::FutureReturnCode::SUCCESS)//判断响应结果 状态码获得有节点(传入实例化的节点类对象即可)和响应两个参数
    {
      RCLCPP_INFO(client_->get_logger(),"响应成功");
      //响应成功不一定意味着乌龟正常创建,比如说创建时候重名了,响应会成功,但是不会生成乌龟.正常是创建什么名就返回什么名字,重名返回空串----响应成功也就是收到了响应
      std::string name = response.get()->name;//get()获得响应对象的指针
      if(name.empty()){
        RCLCPP_INFO(client_->get_logger(),"生成的乌龟重名");
      }
      else{
        RCLCPP_INFO(client_->get_logger(),"生成乌龟成功");
      }
    }
    else{
      RCLCPP_INFO(client_->get_logger(),"响应失败");
    }
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}