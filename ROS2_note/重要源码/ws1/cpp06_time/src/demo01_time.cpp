// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;


// 3.定义节点类；

class MyNode:public rclcpp::Node{
public:
  MyNode():Node("MyNode_node_cpp"){
    // demo_rate();
    // demo_time();
    // demo_duration();
    // time与duration区别:二者只是API使用类似,二者有本质区别,time创建的对象是指具体时刻,下面的t2指时刻1970-01-01 00::00::02::500
    // duration指持续2,5s的时间段
    demo_opt();
  }
private:  
  //演示运算符使用
  void demo_opt(){
    rclcpp::Time t1(10,0);
    rclcpp::Time t2(30,0);

    rclcpp::Duration du1(8,0);
    rclcpp::Duration du2(17,0);

    // 比较运算
    RCLCPP_INFO(this->get_logger(),"t1>=t2 %d",t1>=t2);
    RCLCPP_INFO(this->get_logger(),"t1<=t2 %d",t1<=t2);

    // 数学运算
    rclcpp::Duration du3 = t2 - t1;//时刻相减为时间
    rclcpp::Time t3 = t1 + du1;//时刻与时间运算为时刻

    // Duration之间也可以加减乘除以及比较
    // 注意运算结果要有意义，意义就是时间与时刻之间运算的意义

  }
  // 演示duration使用
  void demo_duration(){
    // 1.创建duration对象
    rclcpp::Duration du1(1s);
    rclcpp::Duration du2(2,500000000L);
    // 2.调用函数
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",du1.seconds(),du1.nanoseconds());//获取结果,以秒返回是浮点,以ns是长整形
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",du2.seconds(),du2.nanoseconds());
  }

  // 演示time使用
  void demo_time(){
    // 1.创建time对象(time对象存一个时间或获取当前时间)
    rclcpp::Time t1(500000000L);//传入纳秒,该参数是六十四位,加上后缀L(5亿ns为1s)
    rclcpp::Time t2(2,500000000L);//传入秒和纳秒
    rclcpp::Time right_now=this->get_clock()->now();//通过节点获取当前时刻
    rclcpp::Time right_now2=this->now();//通过节点获取当前时刻(创建获取当前时刻的对象)


    // 2.调用time对象函数
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",t1.seconds(),t1.nanoseconds());//获取结果,以秒返回是浮点,以ns是长整形
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",t2.seconds(),t2.nanoseconds());
    RCLCPP_INFO(this->get_logger(),"s=%.2f,ns=%ld",right_now.seconds(),right_now.nanoseconds());


  }


  // 演示Rate使用
  void demo_rate(){
    // 1.创建Rate对象:传入时间（休眠时间）或浮点类型的频率(Hz)
    rclcpp::Rate rate(500ms);

  }
  // 第2章话题通信案例中，要求话题发布方按照一定的频率发布消息，我们实现时是通过定时器来控制发布频率的，其实，
// 除了定时器之外，ROS2 中还提供了 Rate 类，通过该类对象也可以控制程序的运行频率。
  // 演示Rate使用
  void demo_rate(){
    // 1.创建Rate对象:传入时间（休眠时间）或浮点类型的频率(Hz)
    rclcpp::Rate rate(500ms);
    // rclcpp::Rate rate(1.0);
    // 2.调用Rate的sleep函数
    while(rclcpp::ok()){
      // 节点正常运行的情况下
      RCLCPP_INFO(this->get_logger(),"-----------");
      rate.sleep();
    }
  }
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




