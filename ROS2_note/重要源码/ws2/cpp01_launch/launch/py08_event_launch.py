from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类--------------
from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取-----------------
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration
# 文件包含相关-------------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关----------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关----------------------
from launch.event_handlers import OnProcessStart, OnProcessExit
from launch.actions import RegisterEventHandler,LogInfo
# 获取某个功能包下，share目录的路径-------(install下的路径)
# from ament_index_python.packages import get_package_share_directory


# 事件就是节点运行过程中，产生的一些特殊行为，比如节点启动并初始化，这个初始化可以认为是一个事件，节点关闭也可以认为是一个事件。当事件触发的时候可以给他注册一定的处理逻辑（事件绑定）

# 当前案例是为turtlesim_node节点添加事件，事件一：节点启动时候调用spawn服务生成新乌龟，事件二：节点关闭时，输出日志信息

def generate_launch_description():
    turtle = Node(package="turtlesim",executable="turtlesim_node")

    # 终端ros2 service call /spawn turtlesim/srv/Spawn "{'x': 8.0,'y': 3.0}"  没设置的参数还是默认值
    spawn = ExecuteProcess(
        cmd = ["ros2 service call /spawn turtlesim/srv/Spawn \"{'x': 8.0,'y': 3.0}\""],
        output="both",
        shell=True
    )

    # 注册事件一
    event_start = RegisterEventHandler(
        event_handler=OnProcessStart(target_action=turtle,on_start=[spawn,LogInfo(msg="事件一")])#针对哪个事件注册---程序启动时注册，参数是针对哪个节点，以及事件触发时作的操作（作的操作也是节点）
        # 触发多个操作用列表，触发一个可用可不用
    )

    # 注册事件二
    event_exit = RegisterEventHandler(
        event_handler=OnProcessExit(target_action=turtle,on_exit=LogInfo(msg="turtlesim_node 退出"))#日志输出
    )

    return LaunchDescription([turtle,event_start,event_exit])