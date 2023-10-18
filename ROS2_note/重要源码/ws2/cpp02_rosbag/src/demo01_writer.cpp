
// 录制控制乌龟运动的速度指令

//保存的就是消息，读取的时候也是读取的消息类型数据，读出来后当作消息类型处理就行---也可以再次发布来控制节点





// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rosbag2_cpp/writer.hpp"

// 3.定义节点类；
class SimpleBagRecorder:public rclcpp::Node{
public:
    SimpleBagRecorder():Node("SimpleBagRecorder_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"消息录制对象创建");
    //3-1创建录制对象
    writer_ = std::make_unique<rosbag2_cpp::Writer>();
    //3-二设置磁盘文件
    writer_->open("my_bag");//传入文件路径--这里用的相对路径，在工作空间下，不是代码文件)(不用自己创建，这里自动创建)--一个名字只能存在一次，不能覆盖，要不把之前的删除，要不这里用个动态的名字
    //3-3写数据，(创建一个速度订阅方，回调函数中执行写出操作)
    sub_=this->create_subscription<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10,std::bind(&SimpleBagRecorder::do_write_msg,this,std::placeholders::_1));
    //writer_ = write();
    }
private:
  void do_write_msg(std::shared_ptr<rclcpp::SerializedMessage> msg){//多态，消息都是可序列化的类型的子类，所以这里用父类声明
    writer_->write(msg,"turtle1/cmd_vel","geometry_msgs/msg/Twist",this->now());//字符串参数里用到消息啥的，就别用：：了，用/
    //        被写出的消息   话题            消息类型                  时间戳
    RCLCPP_INFO(this->get_logger(),"数据写出");
  }
  std::unique_ptr<rosbag2_cpp::Writer>writer_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<SimpleBagRecorder>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}