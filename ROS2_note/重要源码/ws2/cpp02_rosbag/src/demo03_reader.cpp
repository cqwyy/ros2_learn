//读取bag文件数据，并将数据输出在终端

//保存的就是消息，读取的时候也是读取的消息类型数据，读出来后当作消息类型处理就行---也可以再次发布来控制节点


// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rosbag2_cpp/reader.hpp"
#include "geometry_msgs/msg/twist.hpp"
// 3.定义节点类；
class SimpleBagPlay:public rclcpp::Node{
public:
    SimpleBagPlay():Node("SimpleBagPlay_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"消息回放对象创建");
    //3-1 创建一个回放对象
    reader_ = std::make_unique<rosbag2_cpp::Reader>();
    //3-二设置被读取的文件
    reader_->open("my_bag");
    //3-3读消息
    while(reader_->has_next()){//一条一条读取的，所以有下一条就读
        auto twist= reader_->read_next<geometry_msgs::msg::Twist>();
        RCLCPP_INFO(this->get_logger(),"线速度。%.2f,角速度%.2f",twist.linear.x,twist.angular.z);
    }
    //3.四 关闭资源
    reader_->close();
}
private:
    std::unique_ptr<rosbag2_cpp::Reader> reader_;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<SimpleBagPlay>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}