/*  
  需求：编写动作服务端实习，可以提取客户端请求提交的整型数据，并累加从1到该数据之间的所有整数以求和，
       每累加一次都计算当前运算进度并连续反馈回客户端，最后，在将求和结果返回给客户端。
  步骤：
    1.包含头文件；
    2.初始化 ROS2 客户端；
    3.定义节点类；
      3-1.创建动作服务端；
      3-2.处理请求数据；（回调函数）
      3-3.处理取消任务请求；（回调函数）
      3-4.生成连续反馈与响应最终结果（回调函数）

    4.调用spin函数，并传入节点对象指针；
    5.释放资源。

*/


// 1.包含头文件；
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "base_interfaces_demo/action/progress.hpp"

using base_interfaces_demo::action::Progress;//为了方便调用，把他引出来
using std::placeholders::_1;
using std::placeholders::_2;
// 3.定义节点类；
class ProgressActionServer:public rclcpp::Node{
public:
ProgressActionServer():Node("ProgressActionServer_node_cpp"){
  RCLCPP_INFO(this->get_logger(),"action 服务端创建");
      // 3-1.创建动作服务端；(跟之前不一样，在那个新导入的包)
      // 下面这个有两种重载实现，这里用第一种
      // 模板参数：消息载体（动作接口类型，节点可以不用设置-用参数设置）
      // 函数参数：依赖的节点，话题名，处理请求数据（目标值）回调函数,取消任务回调函数,生成连续反馈以及响应最终结果回调
      // 返回动作服务指针
      
      // 模板参数根之前同理，在数据包/action/progress.hpp中，先导入头文件
    this->server_=rclcpp_action::create_server<Progress>
      (this,"get_sum",
      // 写回调函数先要知道返回值和参数
      // 按住ctrl点击create_server,再点击新页面的Server（trptname后面）
      // 回调函数有参数，别忘了占位符
                std::bind(&ProgressActionServer::handle_goal,this,_1,_2),
                std::bind(&ProgressActionServer::handle_cancel,this,_1),
                std::bind(&ProgressActionServer::handle_accepted,this,_1));
}
private:
  rclcpp_action::Server<Progress>::SharedPtr server_;
  //可见动作通信立新包是为了不和服务通信冲突，这里初始化的返回类型都差不多，是rclcpp::服务类型<消息载体>
      
      // 3-2.处理请求数据（处理目标值）；（回调函数）----根据目标值判断执行动作

      // std::function<GoalResponse(const GoalUUID &, std::shared_ptr<const typename ActionT::Goal>)>;
      // （整个函数作为一个函数模板？）可以看见返回值是GoalResponse，他的命名空间可以在文件上面看到，鼠标放到rclcpp_action悬停
      // 第一个参数也是这个命名空间下的，可以在定义处鼠标悬停看到。参数二是动作接口类型的Goal，我们这里就是Progress::Goal
      // 参数二可以拿到目标值
      rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID & uuid,std::shared_ptr<const Progress::Goal> goal){
          (void)uuid;//客户端的ID，没有用到就void声明一下
          // 判断提交的数字是否大于一，是就接受，否则就拒绝
          if(goal->num<1){
            RCLCPP_INFO(this->get_logger(),"提交的目标值必须大于一");
            return rclcpp_action::GoalResponse::REJECT;
          }
          
          RCLCPP_INFO(this->get_logger(),"提交的目标值合法");
          return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;//接受立即执行
          // (返回值就是返回类型里面封装的量)还有接收推迟执行（有推迟时间）以及拒绝
          // （就是发一个目标值，然后判断是不是要执行）
      }


      // 3-3.处理取消任务请求；（回调函数）。服务端执行任务过程中，客户端可以发送取消请求，这个函数决定是否取消
      // 比如说:导弹已经发射出去了，客户端让取消发送，就取消不了了
      // std::function<CancelResponse(std::shared_ptr<ServerGoalHandle<ActionT>>)>;
      // 命名空间确定方法跟上面那个函数一样，同样这里动作接口类型也是Progress
      rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
          (void)goal_handle;
          RCLCPP_INFO(this->get_logger(),"接受任务取消请求");//比如客户端ctrl+c--客户端中止，意味着他不想要数据了，就相当于取消
          return rclcpp_action::CancelResponse::ACCEPT;//接受取消任务，还有个拒绝取消任务
          // (返回值就是返回类型里面封装的量)
      }

      // 3-4.生成连续反馈与响应最终结果（回调函数）
      // 连续反馈与最终响应是耗时操作，为了避免主逻辑阻塞，应该单独再开一个线程来处理
      // std::function<void (std::shared_ptr<ServerGoalHandle<ActionT>>)>;
      // 返回空，参数和取消任务的一样
      void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
        // 1.生成连续反馈返回与给客户端
        // void rclcpp_action::ServerGoalHandle<base_interfaces_demo::action::Progress>::publish_feedback
        // (std::shared_ptr<base_interfaces_demo::action::Progress_Feedback> feedback_msg)

        // Progress_Feedback相当于Progress::Feedback.其他同理

        // 获取目标值，然后遍历，遍历中累加，每循环一次计算进度，作为连续反馈发布
          int num=goal_handle->get_goal()->num;
          int sum=0;
          auto feedback = std::make_shared<Progress::Feedback>();//发布连续反馈函数需要的参数类型
          rclcpp::Rate rate(1.0);//设置休眠对象--对象是执行频率，1.0就是一秒钟执行一次

          auto result=std::make_shared<Progress::Result>();//发布最终结果需要的参数(取消时候也可以发，类型一样，所以这里放到前面)

          for(int i=1;i<=num;i++){

              // 判断是否接收到取消请求(客户端ctrl+c的bug修复)
              if(goal_handle->is_canceling()){//返回true就是收到取消请求
              
                // 对应生成最终响应结果succeed，这里取消请求也可以发送一次数据（不过客户端ctrl+c直接退出收不到了）
                result->sum=sum;//半路取消也是有sum值的
                goal_handle->canceled(result);
                RCLCPP_INFO(this->get_logger(),"，任务取消，最终响应结果%d",sum);
                return;//确定取消，就直接空返回
              }
              sum+=i;
              double progress=i/(double)num;//int/int=int
              feedback->progress=progress;//左值是自己当时在action里面设置的字段

              goal_handle->publish_feedback(feedback);
              RCLCPP_INFO(this->get_logger(),"连续反馈中，进度%.2f",progress);\

              rate.sleep();//---1秒一次就是休眠一秒
          }
          
        // 2.生成最终响应结果
        // void rclcpp_action::ServerGoalHandle<base_interfaces_demo::action::Progress>::succeed
        // (std::shared_ptr<base_interfaces_demo::action::Progress_Result> result_msg)
          if(rclcpp::ok()){//判断服务端是否正常运行中
          // auto result=std::make_shared<Progress::Result>();//发布最终结果需要的参数。Progress_Result  _就是::
          result->sum=sum;

          goal_handle->succeed(result);
          RCLCPP_INFO(this->get_logger(),"最终响应结果%d",sum);
          }
        
      }
      void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> goal_handle){
          // 新建子线程，处理耗时操作（当接收任务开始处理后，直接把处理数据丢给子线程，让子线程调用回调函数处理）
          std::thread(std::bind(&ProgressActionServer::execute,this,goal_handle)).detach();
      }
};
int main(int argc, char const *argv[])
{
// 2.初始化 ROS2 客户端；
rclcpp::init(argc,argv);
// 4.调用spin函数，并传入节点对象指针；
rclcpp::spin(std::make_shared<ProgressActionServer>());
// 5.释放资源。
rclcpp::shutdown();
return 0;
}