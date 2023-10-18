// 处理客户端发送的请求数据(处理目标点),控制乌龟向目标点运动,且连续反馈剩余距离

// 连续反馈是发布到动作通信话题上的,所以不能控制乌龟运动(就是发布出来看看状态),所以需要发布方

// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/nav.hpp"
using std::placeholders::_1;
using std::placeholders::_2;
using base_interfaces_demo::action::Nav;

// 3.定义节点类；
class Exer04ActionServer:public rclcpp::Node{
public:
    Exer04ActionServer():Node("exer04_action_server_node_cpp"),x(0.0),y(0.0){
        RCLCPP_INFO(this->get_logger(),"动作服务端开启");
        // 3.1 创建原生乌龟位姿订阅方,获取当前乌龟坐标
        sub_=this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&Exer04ActionServer::pose_cb,this,_1));

        // 3.2 创建一个速度指令发布方,控制乌龟运动
        cmd_pub_=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
        // 3-3 创建一个动作服务端
        action_server_ = rclcpp_action::create_server<Nav>(this,//不管什么通信对象都要依赖节点,之前是this直接创建,这里是参数传入
        "nav",//话题名(这里第一次出现,不存在什么创建话题,都是通信对象直接关联)
        std::bind(&Exer04ActionServer::handle_goal,this,_1,_2),
        std::bind(&Exer04ActionServer::handle_cancel,this,_1),
        std::bind(&Exer04ActionServer::handle_accepted,this,_1)
        );
    }
private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_;
    float x,y;//成员变量

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp_action::Server<Nav>::SharedPtr action_server_;

    void pose_cb(const turtlesim::msg::Pose::SharedPtr pose){//这里用的指针，引用也可以（知道怎么用到指针）
    // 订约回调函数不发布数据,等在动作连续反馈时候发布数据,控制乌龟运动(其实感觉服务加话题也能实现,这里用动作,所以这么做,并且主逻辑是耗时操作,所以话题不太合适)
        x=pose->x;
        y=pose->y;
    }
    
    rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const Nav::Goal> goal){
        // 3.4解析动作客户端提交的数据
        (void)uuid;
        // 取出目标中的x,y坐标,分别判断是否超出取值范围(0,11.08),超出就非法
        if(goal->goal_x<0 || goal->goal_x>11.08 ||goal->goal_y<0 || goal->goal_y>11.08 ){
            // 非法就拒绝请求
            RCLCPP_INFO(this->get_logger(),"目标点超出正常取值范围");
            return rclcpp_action::GoalResponse::REJECT;
        }
        RCLCPP_INFO(this->get_logger(),"目标点合法");
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }
    rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle){
        //3.5处理客户端取消请求操作
        RCLCPP_INFO(this->get_logger(),"取消请求");
        return rclcpp_action::CancelResponse::ACCEPT;
    }
    
    void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle){
        //3.7子线程中,产生连续反馈,发布速度指令,并响应最终结果
        RCLCPP_INFO(this->get_logger(),"主逻辑开始执行");

        rclcpp::Rate rate(1.0);//一秒一次
        auto result = std::make_shared<Nav::Result>();
        auto feedback = std::make_shared<Nav::Feedback>();
        geometry_msgs::msg::Twist twist;
        while(true){
            // 客户端要取消任务,则特殊处理
            if(goal_handle->is_canceling()){//取消则设置一个最终结果对象
                goal_handle->canceled(result);
                return;//这里不能释放节点资源,因为这里是服务端
            }
            // 解析目标点坐标与原生乌龟实时坐标
            float goal_x=goal_handle->get_goal()->goal_x;
            float goal_y=goal_handle->get_goal()->goal_y;
            // 计算剩余距离并发布
            float distance_x=goal_x - x;
            float distance_y=goal_y - y;
            float distance = std::sqrt(distance_x*distance_x+distance_y*distance_y);
            feedback->distance = distance;
            goal_handle->publish_feedback(feedback);

            // 根据剩余距离计算速度指令并发布
            float scale = 0.5;
            float liner_x = scale*distance_x;
            float liner_y = scale*distance_y;
            twist.linear.x=liner_x;
            twist.linear.y=liner_y;
            cmd_pub_->publish(twist);
            // 循环结束条件
            if(distance<=0.05){//距离很小时认为达到,结束导航
                RCLCPP_INFO(this->get_logger(),"乌龟已经导航至目标点");

                //这里还应该发布一次0速度吧 

                break;
            }
            rate.sleep();
        }
        // 生成最终响应
        if(rclcpp::ok()){
            result->turtle_x = x;
            result->turtle_y = y;

            goal_handle->succeed(result);
        }


    }
    void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Nav>> goal_handle){
        // 3.6实现主逻辑(耗时操作),启动子线程
        std::thread(std::bind(&Exer04ActionServer::execute,this,goal_handle)).detach();
        
    }
};
int main(int argc, char const *argv[])
{
    // 2.初始化 ROS2 客户端；
    rclcpp::init(argc,argv);
    // 4.调用spin函数，并传入节点对象指针；
    rclcpp::spin(std::make_shared<Exer04ActionServer>());
    // 5.释放资源。
    rclcpp::shutdown();
    return 0;
}