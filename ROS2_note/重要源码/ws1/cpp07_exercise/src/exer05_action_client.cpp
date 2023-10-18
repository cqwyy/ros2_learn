// 需求:向动作服务端发送目标点数据,并处理响应结果

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"
using std::placeholders::_1;
using std::placeholders::_2;
using base_interfaces_demo::action::Nav;
using namespace std::chrono_literals;
// 3.定义节点类；
class Exer05ActionClient:public rclcpp::Node{
public:
    Exer05ActionClient():Node("Exer05_action_client_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"动作客户端开启");
    // 3-1:创建动作客户端
    client_ = rclcpp_action::create_client<Nav>(this,"nav");
    }
    void send_goal(float x,float y,float theta){
        // 3-2连接服务器,发送请求
        // 1.连接服务端
        if(!client_->wait_for_action_server(10s)){
            // 等待十秒,连不上就放弃
            RCLCPP_INFO(this->get_logger(),"服务连接超时");
            return;//甚至可以直接释放节点资源
        }
        // 2.组织并发送数据
        Nav::Goal goal;
        goal.goal_x = x;
        goal.goal_y = y;
        goal.goal_theta = theta;

        rclcpp_action::Client<Nav>::SendGoalOptions options;
        options.goal_response_callback = std::bind(&Exer05ActionClient::goal_response_callback,this,_1);//目标值处理回调
        options.feedback_callback = std::bind(&Exer05ActionClient::feedback_callback,this,_1,_2);//处理连续反馈
        options.result_callback = std::bind(&Exer05ActionClient::result_callback,this,_1);//处理最终结果
        client_->async_send_goal(goal,options);//发送的值以及绑定回调等

    }
    void goal_response_callback(std::shared_ptr<rclcpp_action::ClientGoalHandle<Nav>>goal_handle){
        // 3-3处理目标值相关响应结果
        if(!goal_handle){//服务端拒绝处理,goal_handle就是空指针
            RCLCPP_INFO(this->get_logger(),"请求目标非法");
        }
        else{
            RCLCPP_INFO(this->get_logger(),"目标值被接收");
        }

    }
    void feedback_callback(std::shared_ptr<rclcpp_action::ClientGoalHandle<Nav>> goal_handle,std::shared_ptr<const Nav::Feedback> feedback){
        // 3-四 处理连续反馈
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(),"剩余%.3f",feedback->distance);
    }
    void result_callback(const rclcpp_action::ClientGoalHandle<Nav>::WrappedResult &result){
        // 3-5 处理最终结果
        if(result.code==rclcpp_action::ResultCode::SUCCEEDED){
            //响应成功
            RCLCPP_INFO(this->get_logger(),"最终位姿信息:(%.2f,%.2f),航向:%.2f",result.result->turtle_x,result.result->turtle_y,result.result->turtle_theta);
            rclcpp::shutdown();//成功应该直接释放,否则释放不了
        }
        else{
            RCLCPP_INFO(this->get_logger(),"响应失败");
            // 响应失败
        }
    } 
private:

    rclcpp_action::Client<Nav>::SharedPtr client_;
};
int main(int argc, char const *argv[])
{
    // 0.解析launch传入的参数
    if(argc!=5){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"请正确输入数据");
        return 1;
    }
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    // rclcpp::spin(std::make_shared<Exer05ActionClient>());
    auto client = std::make_shared<Exer05ActionClient>();
    client->send_goal(atof(argv[1]),atof(argv[2]),atof(argv[3]));
    rclcpp::spin(client);

    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}