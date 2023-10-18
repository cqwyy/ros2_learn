// 1.包含头文件；
// 广播不同乌龟相对于world的坐标系相对关系,这里将乌龟名抽取成参数服务,这样可以灵活发布(发布的名要和第一步生成的对应)
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"

// 3.定义节点类；

class TFDynaBroadcaster:public rclcpp::Node{
public:
    TFDynaBroadcaster():Node("tf_dyna_broadcaster_node_cpp"){
    this->declare_parameter("turtle","turtle1");//默认广播turtle1
    turtle = this->get_parameter("turtle").as_string();
    // 3.1创建一个动态广播器（发布方,topic是tf，发布坐标关系）
    broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);//通信对象都要依赖于节点
    // 3.2创建一个乌龟位姿订阅方（发布的消息由这里订阅来的）
    pose_sub_ = this->create_subscription<turtlesim::msg::Pose>("/"+ turtle +"/pose",10,std::bind(&TFDynaBroadcaster::do_pose,this,std::placeholders::_1));
   
    }
private:
    std::string turtle;
    void do_pose(const turtlesim::msg::Pose & pose){
        // 3.3订阅方回调函数中获取乌龟位姿，并生成相对关系然后发布
        //组织消息
        geometry_msgs::msg::TransformStamped ts;

        ts.header.stamp = this->now();
        ts.header.frame_id = "world";
        ts.child_frame_id = turtle;

        ts.transform.translation.x = pose.x;
        ts.transform.translation.y = pose.y;
        ts.transform.translation.z = 0.0;//2D方向无z
        
        //欧拉角转换为四元数
        // 2D只有yaw（偏航）
        tf2::Quaternion qtn;
        qtn.setRPY(0,0,pose.theta);
        ts.transform.rotation.x = qtn.x();
        ts.transform.rotation.y = qtn.y();
        ts.transform.rotation.z = qtn.z();
        ts.transform.rotation.w = qtn.w();

        //发布
        broadcaster_->sendTransform(ts);
    }
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub_;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<TFDynaBroadcaster>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}