// 1.包含头文件；
// 发布相对于laser坐标系的座标点数据
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

using namespace std::chrono_literals;

// 3.定义节点类；

class PointBroadcaster:public rclcpp::Node{
public:
    PointBroadcaster():Node("point_broadcaster_node_cpp"),x(0.0){
        // 3.1创建发布方
        point_pub_ = this->create_publisher<geometry_msgs::msg::PointStamped>("point",10);
        // 3.2创建定时器（指定时间间隔，设置回调函数）
        timer_ = this->create_wall_timer(1s,std::bind(&PointBroadcaster::on_timer,this));//当前节点this调用回调函数on_timer
        
    }
private:
    // 3.3回调函数中组织并发布消息
    void on_timer(){
        // 组织消息
        geometry_msgs::msg::PointStamped ps;
        ps.header.stamp = this->now();
        ps.header.frame_id = "laser";//当前坐标系
        x += 0.05;
        ps.point.x = x;
        ps.point.y = 0.0;
        ps.point.z = -0.1;
        // 发布消息
        point_pub_->publish(ps);
    }
    rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr point_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    double_t x;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<PointBroadcaster>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}