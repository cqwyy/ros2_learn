//自实现静态坐标变换广播器，执行该程序时需要传入参数
  //ros2 run 包 可执行程序 x y z roll pitch yaw frame child_frame

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
// 3.定义节点类；
class TFStaticBroadcaster:public rclcpp::Node{
public:
    TFStaticBroadcaster(char * argv[]):Node("TF_static_broadcaster_node_cpp"){
    //3.1创建广播对象
    // 坐标变换的广播与监听实现基于话题通信，下面创建广播方没有指定话题。（默认会创建话题/tf_static，发布方坐标关系，rviz2订阅）
    broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);//通信对象都要依赖于节点，不是由节点创建，则要在构造函数里传入
    //3.二组织并发布数据
    pub_static_tf(argv);
    }
private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
  void pub_static_tf(char * argv[]){
    //组织消息
    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = this->now();//头里设置时间戳
    transform.header.frame_id = argv[7];//父坐标系
    transform.child_frame_id = argv[8];//子坐标系
    //偏移量
    transform.transform.translation.x = atof(argv[1]);
    transform.transform.translation.y = atof(argv[2]);
    transform.transform.translation.z = atof(argv[3]);
    //设置四元数
    //欧拉角to四元数
    tf2::Quaternion qtn;
    qtn.setRPY(atof(argv[4]),atof(argv[5]),atof(argv[6]));
    
    transform.transform.rotation.x = qtn.x();
    transform.transform.rotation.y = qtn.y();
    transform.transform.rotation.z = qtn.z();
    transform.transform.rotation.w = qtn.w();
    //发布
    broadcaster_->sendTransform(transform);
  }
};
int main(int argc, char *argv[])
{

    //0.判断传入的参数是否合法
    if(argc!=9){
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"传入的参数不合法");
      return 1;
    }
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<TFStaticBroadcaster>(argv));
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}
