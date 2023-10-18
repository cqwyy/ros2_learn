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
# 获取某个功能包下，share目录的路径-------(install下的路径)
# from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    # 优化坐标系的设置
    # 频繁使用的数据抽取为变量,调用时直接调用变量
    t2 = DeclareLaunchArgument(name="t2_name",default_value="t2")#变量名,和默认值

    # 1.启动turtlesim_node节点
    turtle = Node(package="turtlesim",executable="turtlesim_node")#默认生成turtle1
    # 2.启动自定义的spawn节点
    spawn = Node(package="cpp05_exercise",executable="exer01_spawn",parameters=[{"turtle_name":LaunchConfiguration("t2_name")}])# executable可执行程序，不含扩展名
    # 3.分别广播两只乌龟相对于world的坐标变换
    broadcaster1 = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="broa1")# 默认广播turtle1,所以不用参数服务.注意启动了两遍这个exer02_tf_broadcaster节点,要避免重名节点
    broadcaster2 = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="broa2",parameters=[{"turtle":LaunchConfiguration("t2_name")}])# 名字要和发布时候的对应,故用t2
    #4.创建监听节点
    listener = Node(package="cpp05_exercise",executable="exer03_tf_listener",parameters=[{"father_frame":LaunchConfiguration("t2_name"),"child_frame":"turtle1"}])

    return LaunchDescription([t2,turtle,spawn,broadcaster1,broadcaster2,listener])