
/*
    需求：编写参数服务端，设置并操作参数。
    步骤：
        1.包含头文件；
        2.初始化 ROS2 客户端；
        3.定义节点类；
            3-1.增；
            3-2.查；
            3-3.改；
            3-4.删。
        4.创建节点对象指针，调用参数操作函数，并传递给spin函数；
        5.释放资源。

*/

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
// 3.定义节点类；
class ParamServer:public rclcpp::Node{
public:
ParamServer():Node("ParamServer_node_cpp",rclcpp::NodeOptions().allow_undeclared_parameters(true)){//如果允许服务端删除参数，需要在构造函数多传入一个参数
    RCLCPP_INFO(this->get_logger(),"参数服务端创建");
    // 节点就可以作为参数服务端，不需要专门对象
}
    // 3-1.增；
    void declare_param(){
        RCLCPP_INFO(this->get_logger(),"-----增-----");
        this->declare_parameter("car_name","tiger");//一次声明一个参数key value
        this->declare_parameter("width",1.55);
        this->declare_parameter("wheels",5);
        // 还有其他重载形式以及一次创建多个参数
        // this->set_parameter(rclcpp::Parameter("height",2.00));//必须在构造函数设置了（在这之前设置就行应该）允许删除，才能用这个来创建参数
    }
    // 3-2.查；
    void get_param(){
        RCLCPP_INFO(this->get_logger(),"-----查-----");
        // this->get_parameter();获取指定key的参数
        // this->get_parameters();//根据key组成的vector获取很多对象
        // this->has_parameter();//根据传入的键判断是否包含该参数
        auto car = this->get_parameter("car_name");//返回的是参数对象--每一个参数都对应一个对象
        RCLCPP_INFO(this->get_logger(),"key=%s,value=%s",car.get_name().c_str(),car.as_string().c_str());
    
        auto params = this->get_parameters({"car_name","width","wheels"});//返回vector,模板类型就是参数对象类型Parameter
        for(auto &&param:params){
            RCLCPP_INFO(this->get_logger(),"(%s=%s)",param.get_name().c_str(),param.value_to_string().c_str());
        }

        RCLCPP_INFO(this->get_logger(),"是否包含car_name%d",this->has_parameter("car_name"));//返回bool

    }
    // 3-3.改；
    void update_param(){
        RCLCPP_INFO(this->get_logger(),"-----改-----");
        this->set_parameter(rclcpp::Parameter("width",1.75));//传入parameter对象，设置同名键即可修改
        RCLCPP_INFO(this->get_logger(),"whidth=%.2f",this->get_parameter("width").as_double());

    }
    // 3-4.删。
    void del_param(){
        RCLCPP_INFO(this->get_logger(),"-----删-----");
        this->undeclare_parameter("height");//传入键---只能删除set创建的(没有声明而设置的参数)，不能删除declare的
    }
};
int main(int argc, char const *argv[])
{
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);
// 4.调用spin函数，并传入节点对象指针；
auto node=std::make_shared<ParamServer>();

node->declare_param();
node->get_param();
node->update_param();
node->del_param();

rclcpp::spin(node);
// 5.释放资源。
rclcpp::shutdown();
return 0;
}