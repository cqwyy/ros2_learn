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

# 实现乌龟护航
# 发布关于被跟随乌龟的静态坐标变换，因为乌龟是动的，所以其静态坐标变换也是动的。用坐标系而不是座标点，坐标系简单并且蕴含的关系比座标点多
# 思路
# 1.发布目标点相对于turtle1的坐标关系（静态）
# 2.监听back与目标点坐标系的相对位置关系



def generate_launch_description():
    # 1.抽取参数
    escort_back = DeclareLaunchArgument(name="turtle_back",default_value="turtle_back")
    escort_left = DeclareLaunchArgument(name="turtle_left",default_value="turtle_left")
    escort_right = DeclareLaunchArgument(name="turtle_right",default_value="turtle_right")
    # 2.创建turltesim_node节点，并生成新乌龟
    master = Node(package="turtlesim",executable="turtlesim_node")
    spawn_back = Node(package="cpp05_exercise",executable="exer01_spawn",name = "spawn_back",parameters=[{"x":2.0,"y":5.0,"turtle_name":LaunchConfiguration("turtle_back")}])# executable可执行程序，不含扩展名
    spawn_left = Node(package="cpp05_exercise",executable="exer01_spawn",name = "spawn_left",parameters=[{"x":3.0,"y":9.0,"turtle_name":LaunchConfiguration("turtle_left")}])# executable可执行程序，不含扩展名
    spawn_right = Node(package="cpp05_exercise",executable="exer01_spawn",name = "spawn_right",parameters=[{"x":4.0,"y":2.0,"turtle_name":LaunchConfiguration("turtle_right")}])# executable可执行程序，不含扩展名
    # 3.发布坐标变换 各个乌龟到world以及目标点到被跟随乌龟的
    turtle1_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="turtle1_world")#默认就是turtle1到world的
    back_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="back_world",parameters=[{"turtle":LaunchConfiguration("turtle_back")}])
    left_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="left_world",parameters=[{"turtle":LaunchConfiguration("turtle_left")}])
    right_world = Node(package="cpp05_exercise",executable="exer02_tf_broadcaster",name="right_world",parameters=[{"turtle":LaunchConfiguration("turtle_right")}])

    # 执行的命令本质也是传参调包(注意执行命令创建静态坐标变换和代码创建用到的可执行程序不一样)
    # 注意代替命令行的传参用arguments，且只用[],key与value之间用,连接
    escort_goal_back = Node(package="tf2_ros",executable="static_transform_publisher",name="escort_goal_back",arguments=["--frame-id","turtle1","--child-frame-id","escort_goal_back","--x","-1.5"])
    escort_goal_left = Node(package="tf2_ros",executable="static_transform_publisher",name="escort_goal_left",arguments=["--frame-id","turtle1","--child-frame-id","escort_goal_left","--y","1.5"])
    escort_goal_right = Node(package="tf2_ros",executable="static_transform_publisher",name="escort_goal_right",arguments=["--frame-id","turtle1","--child-frame-id","escort_goal_right","--y","-1.5"])
    
    # 4.监听坐标变换--获得跟随着关于跟随点的坐标变换，乌龟跟随点,所以算点相对于乌龟的位姿，乌龟为父坐标系
    back_escort_goal_back = Node(package="cpp05_exercise",executable="exer03_tf_listener",name="back_escort_goal_back",parameters=[{"father_frame":LaunchConfiguration("turtle_back"),"child_frame":"escort_goal_back"}])
    left_escort_goal_left = Node(package="cpp05_exercise",executable="exer03_tf_listener",name="left_escort_goal_left",parameters=[{"father_frame":LaunchConfiguration("turtle_left"),"child_frame":"escort_goal_left"}])
    right_escort_goal_right = Node(package="cpp05_exercise",executable="exer03_tf_listener",name="right_escort_goal_right",parameters=[{"father_frame":LaunchConfiguration("turtle_right"),"child_frame":"escort_goal_right"}])
    
    return LaunchDescription([escort_back,escort_left,escort_right,
                            master,spawn_back,spawn_left,spawn_right,
                            turtle1_world,back_world,left_world,right_world,
                            escort_goal_back,escort_goal_left,escort_goal_right,
                            back_escort_goal_back,left_escort_goal_left,right_escort_goal_right])