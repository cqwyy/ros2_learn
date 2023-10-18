// 1.包含头文件；
// 广播雷达到基坐标系的相对关系，然后发布点到雷达坐标系的坐标，求解点到基坐标系的坐标
// 逻辑：现订阅发布的数据（同如上坐标变换监听，可以监听坐标系关系。并需要另外创建一个订阅对象，订阅坐标点数据）。用过滤器整合两部分数据，再求解结果
// 求解逻辑：按照之前逻辑创建缓存以及坐标系监听器，这里不同的是，缓存还要有一个定时器，用来统一缓存的坐标系和外部的座标点。
// 订阅座标点，创建过滤器。过滤器创建时候传入了坐标系变换关系以及座标点订阅对象，内部自动整合数据。然后在过滤器回调函数中直接处理数据，指定把点转换到哪个坐标系。

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h" 
#include "tf2_ros/create_timer_ros.h"
#include "message_filters/subscriber.h"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "tf2_ros/message_filter.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"//有误报错


using namespace std::chrono_literals;
// 3.定义节点类；

class TFPointListener:public rclcpp::Node{
public:
    TFPointListener():Node("tf_point_listener_node_cpp"){
        // 3.1创建缓存
        buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        // 因为是坐标系和座标点两部分，buffer里的时钟只能统一坐标系的，和座标点之间的还需要额外设置
        timer_ = std::make_shared<tf2_ros::CreateTimerROS>(this->get_node_base_interface(),this->get_node_timers_interface());//两个参数，NodeBaseInterface指针以及NodeTimersInterface指针
        buffer_->setCreateTimerInterface(timer_);//设置一个创建定时器的接口
        // 3.2创建监听器,并绑定buffer
        listener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_,this);
        // 3.3创建座标点消息订阅方（有封装专门的类,之前发布坐标点直接用publisher）。并且这个函数有默认的无参构造，并且提供了订阅函数，所以这里不用赋值了，可以直接用函数订阅
        point_sub_.subscribe(this,"point");//订阅到数据不用回调函数处理，因为数据要统一用过滤器处理
        //3.4创建过滤器解析数据
        filter_ = std::make_shared<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>>(point_sub_,*buffer_,"base_link",10,this->get_node_logging_interface(),this->get_node_clock_interface(),1s);
                                                                                    //参数:订阅对象，坐标系监听的缓存，要转到哪个坐标系，消息队列长度，日志接口，时钟接口，超时时间,默认无限大（超时会抛出异常）
        // 3.5解析数据--过滤器的回调函数
        filter_->registerCallback(&TFPointListener::transform_point,this);//(有多个重载，这里用无返回值的回调函数，回调函数有一个参数，即订阅到的点)回调函数都是用的函数指针,然后是当前节点调用它


    }
private:
    void transform_point(const geometry_msgs::msg::PointStamped & ps){
        //实现座标点的变换
        //必须包含头文件"tf2_geometry_msgs/tf2_geometry_msgs.hpp"
        auto out = buffer_->transform(ps,"base_link");//参数：被转换的数据,转换的目标坐标系（要转到哪个坐标系）。返回转换后的数据也是一个点
        //若订阅到点，但坐标系的消息还没有，就会抛异常--仅在终端上输出一下
        RCLCPP_INFO(this->get_logger(),"父级坐标系：%s,坐标：（%.2f,%.2f,%.2f）",out.header.frame_id.c_str(),out.point.x,out.point.y,out.point.z);
    }
    std::shared_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    std::shared_ptr<tf2_ros::CreateTimerROS> timer_;
    message_filters::Subscriber<geometry_msgs::msg::PointStamped> point_sub_;//没用到指针(用指针的话，后面有异常（后面的函数用的引用）)
    std::shared_ptr<tf2_ros::MessageFilter<geometry_msgs::msg::PointStamped>> filter_;//MessageFilter也是模板类，要设置模板--传入要处理的消息类型,因为坐标系有专门的监听处理，所以这里传座标点的类型
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<TFPointListener>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}