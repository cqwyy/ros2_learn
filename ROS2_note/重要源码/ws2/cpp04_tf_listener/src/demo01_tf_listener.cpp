// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace std::chrono_literals;
// 发布雷达到小车和相机到小车的坐标关系，然后求解雷达到相机的坐标相对关系
// 3.定义节点类；

class TFListener:public rclcpp::Node{
public:
    TFListener():Node("tf_listener_node_cpp"){
        // 3.1创建缓存对象（保存订阅到的多个坐标系关系，并融合成坐标树）
        buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());//至少要传入一个时钟：用来判断两个坐标帧发布的时间差是否符合要求，或者用来指定某个时刻的坐标系
        // 3.2创建监听器，绑定缓存对象，会将所有广播器广播的数据写入缓存（静态动态广播器都有固定的话题）
        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_,this);//用到的是缓存的引用，所以这里解引用一下
        // 3.3编辑一个定时器，循环实现实现转换
        timer_ = this->create_wall_timer(1s,std::bind(&TFListener::on_timer,this));

    }

private:
  void on_timer(){
    //实现坐标转换（可以不用异常，buffer下面还有个canTransform函数判断是否可以转换，参数和lookupTransform函数一样，返回bool值）
    try{// 异常处理，比如要进行两个坐标系的变换，但是此时只收到一个坐标系的消息--异常为tf2::LookupException
    auto ts = buffer_->lookupTransform("camera","laser",tf2::TimePointZero);//返回值类型是TransformStamped--返回一个新的坐标帧(坐标关系)，参数：转换成新的坐标帧中的父级坐标系，子级，转换时刻(这里用最新时刻--最常用)
    RCLCPP_INFO(this->get_logger(),"转换完成的坐标帧信息");
    RCLCPP_INFO(this->get_logger(),"新坐标帧：父级坐标系：%s，子坐标系：%s，偏移量（%.2f,%.2f,%.2f）",ts.header.frame_id.c_str(),ts.child_frame_id.c_str(),ts.transform.translation.x,ts.transform.translation.y,ts.transform.translation.z);
    }
    catch(const tf2::LookupException& e){//try抛出异常，就进入捕获了对应异常的catch
      RCLCPP_INFO(this->get_logger(),"异常提示：%s",e.what());
    }
  }
  std::unique_ptr<tf2_ros::Buffer> buffer_;
  std::shared_ptr<tf2_ros::TransformListener> listener_;
  rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<TFListener>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}