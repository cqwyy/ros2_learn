


/*  
    需求：订阅发布方发布的消息，并输出到终端。
    步骤：
        1.包含头文件；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.创建订阅方；
            3-2.处理订阅到的消息。
        4.调用spin函数，并传入节点对象指针；
        5.释放资源。
*/
// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// 3.定义节点类；
class Listener:public rclcpp::Node{
public:
    Listener():Node("Listener_node_cpp"){
        RCLCPP_INFO(this->get_logger(),"订阅方创建");
        //  3-1.创建订阅方；
        // 参数1是话题名称，参数二理解为队列长度，参数3三是回调函数，在回调函数里处理参数
        // 模板参数是订阅到的消息类型
        // 返回订阅对象的指针
        subscription=this->create_subscription<std_msgs::msg::String>("chatter",10,std::bind(&Listener::do_cb,this,std::placeholders::_1));
        
        // 对象就是节点
        // 进一步理解spin，轮询传入的对象，保证对象的回调函数能被调用
        // 回调函数通过bind，将对象与回调函数绑定，并可以传入数据。比如这里的参数是订阅到的
        // 消息，这里用的占位符（就一个数据，就_1）
        // 因为订约方订阅了话题，所以当话题里面有消息，并且订约方被spin轮询，就会执行回调函数

    }
private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription;
    void do_cb(const std_msgs::msg::String &msg)
    //  3-2.处理订阅到的消息。
    {//有参数，参数是收到的消息，这里设为引用类型
        RCLCPP_INFO(this->get_logger(),"订阅到的消息是%s",msg.data.c_str());
    }
};


int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);

    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<Listener>());

    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}
