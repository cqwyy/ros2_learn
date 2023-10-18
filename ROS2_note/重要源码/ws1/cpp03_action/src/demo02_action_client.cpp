/*  
  需求：编写动作客户端实现，可以提交一个整型数据到服务端，并处理服务端的连续反馈以及最终返回结果。
  步骤：
     前提：解析终端动态传入的参数
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建动作客户端；
      3-2.发送请求；
      3-3.处理关于目标值的服务端响应；（回调）
      3-4.处理连续反馈；（回调）
      3-5.处理最终响应。（回调）
    //   根据框图可知，最终响应的时候还会发请求到服务端，但是这个已经被封装好了，不需要实现，所以这里以及服务端都没有体现
    4.调用spin函数，并传入节点对象指针；
    5.释放资源。
*/

// 要是代码写的客户端就是不行,那么用launch的终端命令创建应该可以


// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"
using base_interfaces_demo::action::Progress;
using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;
// 3.定义节点类；
class ProgressActionClient:public rclcpp::Node{
public:
ProgressActionClient():Node("ProgressActionClient_node_cpp"){
    RCLCPP_INFO(this->get_logger(),"action客户端创建");

    //   3-1.创建动作客户端；
        client_ = rclcpp_action::create_client<Progress>(this,"get_sum");

}//这里特殊，以前都是创建完直接开始处理（创建和处理是一起的），这里把创建和发送分开了（跟服务通信的客户端一样）
    //   3-2.发送请求；
    void send_goal(int num){
        // 需要等客户端连接服务端再发
        if(!client_->wait_for_action_server(10s)){//参数是超时时间，在时间内连接上就返回true
            RCLCPP_ERROR(this->get_logger(),"连接失败");
            // return;
            rclcpp::shutdown();
        }
        
        // 发送具体请求
        //   std::shared_future<typename GoalHandle::SharedPtr>
        //   async_send_goal(
        //   const Goal & goal, 
        //   const SendGoalOptions & options = SendGoalOptions())
        auto goal = Progress::Goal();//设置目标值
        goal.num=num;
        rclcpp_action::Client<Progress>::SendGoalOptions options;//ctrl点击SendGoalOptions,再点各自声明，看返回与值和参数
        //这里和客户端不一样了，动作通信需要回调，这个就设置回调函数(分别对应要处理的三个步骤)
        // （单其实整个架构可以和客户端一样，这里是把连接和发送写到一起了，客户端是分开的，各自单独调用）
        options.goal_response_callback = std::bind(&ProgressActionClient::goal_response_callback,this,_1);
        options.feedback_callback = std::bind(&ProgressActionClient::feedback_callback,this,_1,_2);
        options.result_callback = std::bind(&ProgressActionClient::result_callback,this,_1);
        auto future= client_->async_send_goal(goal,options);//异步发送目标
    }
   
private:
    rclcpp_action::Client<Progress>::SharedPtr client_;

     //   3-3.处理关于目标值的服务端响应；（回调）
    void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle){
//   343  using GoalHandle = ClientGoalHandle<ActionT>;
//   345  using GoalResponseCallback = std::function<void (typename GoalHandle::SharedPtr)>;
// 找到定义如上，注意发现GoalHandle不知道是什么，看见343行有定义，然后再点进343行ClientGoalHandle看工作空间
// 注意回调函数参数写了，回头把定义回调函数时候的bind加上占位符号

    // 发送之后，服务端进行一个响应，所以当目标值可处理的话，goal_handle里面有内容，不能被处理就是一个空指针
    if(!goal_handle){
        RCLCPP_ERROR(this->get_logger(),"目标请求被服务器拒绝");
        }
    else{
        RCLCPP_INFO(this->get_logger(),"目标请求处理中");
        }
    }
    //   3-4.处理连续反馈；（回调）
    void feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal_handle,const std::shared_ptr<const Progress::Feedback> feedback){
        // 这里ctrl同理，不过，要找到最后，上面找到时候就有函数返回值类型了，这里到这层还没有返回值类型，
        // 所以因为这里是GoalHandle右值是类型，点进类型再看，找到FeedbackCallback的声明（有函数返回值）

        // goal_handle用不上，feedback存了反馈的数据，用这个就行
        // (void)goal_handle;
        

        // if(goal_handle->get_status()==(int8_t)3 || goal_handle->get_status()==(int8_t)5 ||goal_handle->get_status()==(int8_t)6 ||goal_handle->get_status()==(int8_t)0){//返回true就是收到取消请求
        // // 对应生成最终响应结果succeed，这里取消请求也可以发送一次数据（不过客户端ctrl+c直接退出收不到了）
        // RCLCPP_INFO(this->get_logger(),"，任务取消%d",goal_handle->get_status());
        // // return;//确定取消，就直接空返回
        // }
        // goal_handle->get_status()--调用就卡死
        
        double progress=feedback->progress;//反馈的进度数据
        int pro = (int)(progress * 100);
        RCLCPP_INFO(this->get_logger(),"当前进度:%d%%",pro);
        // if(pro==50){
            // 下面两句话都可以停止客户端，都不可以停止服务端
            // rclcpp::shutdown();
            // client_->async_cancel_goal(goal_handle);
        // }
        
        // 可能会有数据丢失，因为feedback是基于发布订阅的
    }
    //   3-5.处理最终响应。（回调）
    void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult & result){
        //通过状态码判断最终响应结果的状态ABORTED强行终止 CANCELED取消 SUCCEEDED成功 UNKNOWN未知异常
        if(result.code==rclcpp_action::ResultCode::SUCCEEDED){
            RCLCPP_INFO(this->get_logger(),"最终结果%d",result.result->sum);
        }
        else if(result.code==rclcpp_action::ResultCode::ABORTED){
            RCLCPP_INFO(this->get_logger(),"被中断！");
        }
        else if(result.code==rclcpp_action::ResultCode::CANCELED){//客户端人为的被取消了
            RCLCPP_INFO(this->get_logger(),"被取消！");
        }
        else{
            RCLCPP_INFO(this->get_logger(),"未知异常");
        }   
        rclcpp::shutdown();//接受完消息后退出轮询
    }


};
int main(int argc, char const *argv[])
{
    if(argc !=2 ){
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"请提交一个整形数据");
        return 1;
    }
// 2.初始化 ROS2 客户端；(客户端有ID，用来区分不同客户端)
rclcpp::init(argc,argv);
// 4.调用spin函数，并传入节点对象指针；
auto node= std::make_shared<ProgressActionClient>();
node->send_goal(atoi(argv[1]));


rclcpp::spin(node);//回调函数：一切初始化完成，调用回调等待回调触发
// spin维护了节点的回调函数列表，当有回调触发，列表里就有个函数等待，spin轮询这个列表，有函数就执行，
// 然后把函数从列表去除（传入节点是因为要用节点调用他们？）

// 5.释放资源。
rclcpp::shutdown();
return 0;
}