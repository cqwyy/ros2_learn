from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类--------------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取-----------------
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
# 文件包含相关-------------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关----------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关----------------------
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler,LogInfo
# 获取功能包下share目录路径-------
# from ament_index_python.packages import get_package_share_directory




# 需求在launch文件启动时，传入参数来设置turtlesim_node的背景色


def generate_launch_description():

    # 一：声明参数,设置key以及value，值是默认值，启动launch没有传入，则用默认值
    # 逻辑是声明一个变量，变量名是key，初始化为value，然后把声明的对象给一个变量（这个变量相当于节点应该--要放入最后那个return列表）
    bg_r = DeclareLaunchArgument(name="backg_r",default_value="255")  #注意key和value都是字符串
    bg_g = DeclareLaunchArgument(name="backg_g",default_value="255")
    
    # 二：调用参数
    turtle = Node(
        package="turtlesim",
        executable="turtlesim_node",
        parameters=[{"background_r": LaunchConfiguration("backg_r"),"background_g": LaunchConfiguration("backg_g")}]#传入一个被解析的参数的名，就是上面声明的参数的key
    )
    # 启动时ros2 launch cpp01_launch py05_args_launch.py backg_r:=0 backg_g:=0 可以设置key的值(空格分割不同参数)
    return LaunchDescription([bg_r,bg_g,turtle])