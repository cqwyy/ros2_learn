

// 修改turtlesim_node的背景色


// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
using namespace std::chrono_literals;

// 3.定义节点类；
class Exer06Param:public rclcpp::Node{
public:
    Exer06Param():Node("exer06_param_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"参数客户端");
    // 3-1创建参数客户端
    client_ = std::make_shared<rclcpp::SyncParametersClient>(this,"/turtlesim");//通信对象依赖于节点，不是由节点创建的就要参数里面传入节点,第二个就是连接的节点名
    }
    // 3-二 连接参数服务端
    bool connect_server(){
        while(!client_->wait_for_service(1s)){
            if(!rclcpp::ok()){
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"客户端强制退出");
                return false;
            }   
            RCLCPP_INFO(this->get_logger(),"服务连接中");//之前是给一个长的超时时间，连不上就不连，这里是一直连
        }
        return true;
    }
    // 3-3更新参数
    void update_param(){
        //让背景色递进修改
        // 1.获取参数
        int red = client_->get_parameter<int>("background_r");//模板是参数类型，参数是参数的key

        // 二：编写循环，修改参数（休眠控制修改频率）
        rclcpp::Rate rate(30.0);//30Hz

        // 一个完整的计数周期，计数511个，中间值二55。。所以创建计数器，初始值与red一样，递增值也和red一样，取值[0,511]。。当计数器在0-二55之间，red增，否则减。。。计数器大于等于511就归零
        // 不能用int8_t让他溢出，因为溢出就从0开始加，而不是从最大开始减小
        int count = red;
        while (rclcpp::ok())
        {
            count<=255? red+=5:red-=5;
            count+=5;
            if(count>=511) count=0;


            // 修改参数
            client_->set_parameters({rclcpp::Parameter("background_r",red)});
            rate.sleep();
        }
        
    }
private:
    rclcpp::SyncParametersClient::SharedPtr client_;
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    // rclcpp::spin(std::make_shared<Exer06Param>());

    // 四：创建节点对象指针，并调用相关函数
    auto client = std::make_shared<Exer06Param>();
    if(!client->connect_server()){
        return 1;
    }
    client->update_param();

    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}