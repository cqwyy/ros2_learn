from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类--------------
# from launch.actions import ExecuteProcess
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
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler,LogInfo
# 获取某个功能包下，share目录的路径-------
from ament_index_python.packages import get_package_share_directory
import os
# 演示node的使用


def generate_launch_description():
    turtle1 = Node(#package被执行程序所属的功能包  executable可执行程序 name设置节点名   namespace设置命名空间  exec_name设置程序标签  parameters设置参数  remappings重映射话题 
    # arguments为节点启动时传参（参数在前，参数后面自动跟上--ros-args，所以要注意参数个数）  ros_arguments节点启动时传参（参数会自动跟上--ros-args）   respawn是否重启（默认false，设置为True，则窗口关闭后自动重启）

        package="turtlesim",
        executable="turtlesim_node",
        exec_name="my_label",# 就是输出一些INFO时候会显示什么程序输出的，这里给他命名
        ros_arguments=["--remap","__ns:=/t2"],#终端设置命名空间： ros2 run turtlesim turtlesim_node --ros-args --remap __ns:=/t1
        # parameters=[{"background_r": 255,"background_g": 0,"background_b": 0}]  #yaml格式--直接设置参数 #改节点内的一些参数（参数服务器），比如乌龟运动背景色，上面那个是改一些命名空间啥的
        # 更常用的是读取yaml文件--通过yaml的绝对路径读取---需要安装，就在cmake的launch后面加上config即可--导出参数ros2 param dump /t2/turtlesim --output-dir config（路径，注意终端在哪个目录下）
        parameters = [os.path.join(get_package_share_directory("cpp01_launch"),"config","config","t2__turtlesim.yaml")]# 动态读install安装的，而不是读源码
        # remapping=[(“/turtle1/cmd_vel”，“/cmd_vel”)]#列表里面有元组，元组元素一是原话题名，元素二是新话题名
        )
    
    return LaunchDescription([turtle1])