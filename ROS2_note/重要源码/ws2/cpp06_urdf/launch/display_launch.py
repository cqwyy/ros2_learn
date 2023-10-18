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
from ament_index_python.packages import get_package_share_directory#找到指定功能包的安装的share下的功能包处--install/功能包/share/功能包

from launch_ros.parameter_descriptions import ParameterValue #封装为参数
from launch.substitutions import Command #封装指令  xacro 文件路径 --就是一个指令，因为他没有运行ros节点。他是一比一复原，所以下面代码里xacro后面有空格的

def generate_launch_description():
    # 加载urdf文件，并在rviz2中显示机器人模型
    # 1.1启动robot_state_publisher节点，该节点要以参数的方式加载urdf文件内容
    # p_value = ParameterValue(Command(["xacro ",get_package_share_directory("cpp06_urdf")+"/urdf/urdf/demo01_helloworld.urdf"])) #注意安装后目录不在安装包的src下了，在install/功能包/share/功能包/urdf
    # 以参数的形式动态传入urdf文件
    model = DeclareLaunchArgument(name="model",default_value=get_package_share_directory("cpp06_urdf")+"/urdf/urdf/demo01_helloworld.urdf")
    p_value = ParameterValue(Command(["xacro ",LaunchConfiguration("model")])) #这是个指令，xacro后有空格
    robot_state_pub = Node(package="robot_state_publisher",executable="robot_state_publisher",parameters=[{"robot_description":p_value}])#参数是机器人描述，也就是urdf里的内容，考虑到易读性，把他封装为参数
    
    # 1.2启动joint_state_publisher节点，发布非固定关节状态
    joint_state_pub = Node(package="joint_state_publisher",executable="joint_state_publisher")
    
    # 2.启动rviz2节点,为了不重复进行rviz配置，设置一个配置文件启动
    rviz2 = Node(package="rviz2",executable="rviz2",arguments=["-d",get_package_share_directory("cpp06_urdf")+"/rviz/urdf.rviz"])
    
    return LaunchDescription([model,robot_state_pub,joint_state_pub,rviz2])
# TODO 传参方式(xacro一样) ros2 launch cpp06_urdf display_launch.py model:=`ros2 pkg prefix --share cpp06_urdf`/urdf/urdf/haha.urdf   ``不是单引号，是键盘左上角，1前面那个键
# 用model:=是因为上面设置DeclareLaunchArgument的时候，参数name为model
# TODO 注意'ros2 pkg prefix --share cpp06_urdf'用命令的方式等效获取了share下的功能包的位置

"""
问题：通过joint_state_publisher_gui让关节运动后，有抖动现象
解决：不再启动joint_state_publisher节点
原因： 
1.robot_state_publisher订阅关节运动信息，并生成坐标变换数据广播
2.他两个作用一样，都是发布非固定关节的运动信息，只要有一个存在就能发布关节运动信息，进而生成坐标变换。当两个都不存在，坐标树生成不了，模型会显示异常。当两个都存在
joint_state_publisher发布初始关节位姿信息，_gui的发布一个当前信息，这些信息都被robot_state_publisher订阅，产生了抖动
# 一般情况用joint_state_publisher（用程序控制运动） ，调试的时候可以注释掉他，用_gui调试
ros2 run joint_state_publisher_gui joint_state_publisher_gui 
"""