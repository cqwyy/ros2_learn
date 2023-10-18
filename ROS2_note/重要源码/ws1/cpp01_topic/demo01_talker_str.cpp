
/*  
  需求：以某个固定频率发送文本“hello world!”，文本后缀编号，每发送一条消息，编号递增1。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建发布方；
      3-2.创建定时器；
      3-3.组织消息并发布。
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。
*/
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using namespace std::chrono_literals;//用这个命名空间，设置间隔时间可以直接用1s表示一秒

//3.自定义节点类
class Talker:public rclcpp::Node{
public:
  Talker():Node("talker_node_cpp"),count(0)//初始化count
  {
    RCLCPP_INFO(this->get_logger(),"发布节点创建");
      //  3-1.创建发布方；
      //要设置模板参数--消息类型（就是消息接口类型，自定义的话有自定义类型） 
      // 函数参数：话题名称,10表示发送消息的时候设置队列为10,当发不出去即阻塞时就放到
      // 队列，正常后会从队列里取数据发送
      // 返回发布对象指针
      publisher_ = this->create_publisher<std_msgs::msg::String>("chatter",10);
      // 3-2.创建定时器；（也是模板，不过有默认类型了）
      //时间间隔，回调函数，在回调函数执行操作,返回定时器对象的指针
      // 用bind连接回调函数，连接到Talker（当前类）的on_timer，由this（当前对象调用）
      timer_ = this->create_wall_timer(1s,std::bind(&Talker::on_timer,this));
      }
private:
// 声明发布对象，Pubilsher是模板类，要传入模板参数-即要发布的消息类型
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  // 声明定时器对象
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count;//声明一个计数器（类型是无符号整形）
  void on_timer(){
    // 3-3.组织消息并发布。
    auto message = std_msgs::msg::String();//创建一个String消息对象
    message.data="hello world" + std::to_string(count++);
    RCLCPP_INFO(this->get_logger(),"发布方发布的消息：%s",message.data.c_str());
    publisher_->publish(message);
  }//回调函数无返回值
  
}; 

int main(int argc, char ** argv)
{
  // 2.初始化ROS2客户端
  rclcpp::init(argc,argv);

  // 4.调用spin函数，并传入节点对象指针；
  // 逻辑好像就是这里创建对象，然后构造函数（没有spin只会打印发布节点创建,执行不到回调函数）
  // 定时器对象把对象连接到回调函数
  // spin一直轮询，程序不退出，到时间就让对象执行回调函数（因为回调函数是对象的方法，且注册的时候是让当前对象调用）
  // 每一个对象都是一个节点（继承自Node）
  rclcpp::spin(std::make_shared<Talker>());
  // while(1){}//不能不用spin而直接while保证不推出，spin可能相当于一个管理，
  // 会定时轮询节点，而while直接程序跑不出来了
  // 之前发布者没用回调，就是循环中发布，sleep一定时间来间隔
  // 暂时这样简单理解，创建的对象中有回调函数，就要用spin

  // 把节点理解为qt的按钮啥的，需要一直轮询这个按钮，当按钮按下就执行对应回调函数
  // 这里spin就是要轮询哪个节点，关注有关节点的一举一动，当定时时间到了就执行回调或者当
  // 订阅到消息了就回调
  // （并且回调函数本质是函数指针，是大流程中的一块小流程，如大流程为等1s，等完执行某个动作）

  // spin传入节点对象应该就是明确轮询哪个节点（可以轮询多个节点）（那这里轮询A对象
  // ，却用B对象连接回调函数，回调函数不会被执行，B对象连接回调函数显然等B对象满足条件
  // 且被轮询才会执行-----更可能是这样，轮询A对象，且用A对象创建定时器，到1s，轮询发现
  // 该对象要执行回调了，就让他执行回调，不过回调是由B调用的）
  // 5.资源释放
  rclcpp::shutdown();


  return 0;
}
// 可用ros2 topic echo /chatter看对应话题发布的消息