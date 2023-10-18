// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/msg/student.hpp"

using base_interfaces_demo::msg::Student;//这里可以简化调用，后面用到Student类型就直接Student即可，不用写这么长了
using namespace std::chrono_literals;

// 3.定义节点类；
class TalkerStu:public rclcpp::Node{
public:
TalkerStu():Node("talkerstu_node_cpp"),age(0)
{
    // 3.1创建发布方
    publisher_ = this->create_publisher<Student>("chatter_stu",10);
    // 3.2创建定时器
    timer=this->create_wall_timer(500ms,std::bind(&TalkerStu::on_timer,this));
}
private:       
    void on_timer(){
        // 3.3组织发布学生信息
        auto stu=Student();
        stu.name="wyy";
        stu.age=age;
        stu.height=2.20;
        age++;
        publisher_->publish(stu);
        RCLCPP_INFO(this->get_logger(),"发布的消息:(%s,%d,%.2f)",stu.name.c_str(),stu.age,stu.height);
    }
    rclcpp::Publisher<Student>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer;
    int age;
};
int main(int argc, char const *argv[])
{
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);
// 4.调用spin函数，并传入节点对象指针；
rclcpp::spin(std::make_shared<TalkerStu>());
// 5.释放资源。
rclcpp::shutdown();
return 0;
}