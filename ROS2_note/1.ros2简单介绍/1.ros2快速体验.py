
# TODO 说明，笔记以老师整理的文档为主，这里对有补充的地方进行补充
# TODO 习惯把接口文件定义到一个功能包，然后后面的包依赖这个自己创建的接口功能包
# TODO 功能包在src下创建，编译在工作空间下编译


# TODO 工作空间创建---创建工作空间后导入文件可能会报错，需要用快速修复，在包含路径里添加/opt/ros/humble/include/**
# 无论是使用C++还是Python编写ROS2程序，都需要依赖于工作空间（一个根目录--工程项目），在此，我们先实现工作空间的创建与编译，
# 打开终端，输入如下指令：
#
# mkdir -p ws00_helloworld/src #创建工作空间以及子级目录 src，工作空间名称可以自定义  mkdir创建文件夹  -p文件夹是父子级的，/前面是父级，自定义的工作空间名称，src是放代码的地方
# cd ws00_helloworld #进入工作空间
# colcon build #编译
# 上述指令执行完毕，将创建ws00_helloworld目录，且该目录下包含build、install、log、src共四个子级目录。（src是自己创建的，前面三个是colcon编译时创建的）


# TODO 工作空间创建完毕后，我们可以在工作空间下的src目录中编写C++或Python程序，且两种语言的实现流程大致一致，主要包含如下步骤：
# 创建功能包；
# 编辑源文件；
# 编辑配置文件；
# 编译；
# 执行。


# cpp
# 终端下，进入ws00_helloworld/src目录，使用如下指令创建一个C++功能包：
# ros2 pkg create（创建功能包） pkg01_helloworld_cpp（包名） --build-type（指定包的具体类型（构建类型）） ament_cmake（cpp要用的的类型） --dependencies(添加依赖) rclcpp(ros2的cpp的客户端,包含了一些API) --node-name（设置一个可执行程序，在功能包下面会自动生成一个同名的原代码文件） helloworld
# 执行完毕，在src目录下将生成一个名为pkg01_helloworld_cpp的目录（包），且目录中已经默认生成了一些子级文件与文件夹。
# TODO 添加依赖是添加的依赖的功能包，可以是系统给的，也可以是自定义的，只要是个功能包且能被检索到，

# 主要是会自动生成cmakelists以及.xml文件，xml注意笔记中第十二左右
# < !-- 所需要依赖 -->  即功能包依赖哪些外部库  create那指定的,需要其他依赖就再按照这个格式添加多行
# < depend > rclcpp < / depend >
# cmakelists中
# 引入外部依赖包--需要其他的也可以手动添加
# find_package(rclcpp REQUIRED)
# 映射源文件与可执行文件
# add_executable(helloworld src/helloworld.cpp)   直译为添加可执行的，参数二对应源文件，参数一为这个文件生成一个可执行程序，程序名为参数一（默认和文件名一样，就是create时候最后那个参数）
# 设置目标依赖库（为可执行程序添加依赖）
# ament_target_dependencies(
#   helloworld  ---生成的可执行程序
#   "rclcpp"  ---可执行程序依赖于rclcpp
# )

# 定义安装规则---为可执行程序设置一个安装目录
# install(TARGETS helloworld
#   DESTINATION lib/${PROJECT_NAME})   ---安装到了PROJECT_NAME(即当前项目或说当前功能包下的lib目录)

# TODO 注意编译应该在工作空间下(注意各步骤应该在哪个文件夹操作)
# 编译后,进入工作空间-install-功能包名-lib-功能包名(里面就有可执行程序了)

# . install/setup.bash    或source install/setup.bash--刷新文件.刷新这个文件就是刷新环境变量
# ros2 run pkg01_helloworld_cpp helloworld  ---run 功能包名 可执行程序名(第一步creat里创建的,应该也是cmakelists设置的,所以这几个包括文件名一样（和文件名一样是为了知道是谁生成的可执行文件）
# --creat设置的--文件默认创建,名称和creat一样--cmakelists进行联接,creat和cmakelists的可执行程序应该就是一个,然后定义安装规则那决定可执行程序的安装目录)

# TODO 功能包是个什么存在,跟工作空间什么关系---功能包在src目录,并且运行的也是功能包,应该就是工作空间包含所有东西,功能包就是包含执行的程序
# TODO create--build-type可以省略，默认就是cpp，添加依赖和--node-name都是配置的xml和cmakelists里的参数，node这个就是可执行程序，然后安装规则决定他放到哪，最后编译就生成了，运行也是包下的运行可执行文件
# TODO 一个功能包下可以有多个cpp文件，cmakelists里add_excetable新建可执行文件再多连接一个cpp文件  ament_target_dependencies里让新的可执行文件连接依赖，最后生成目录的生成名也改成新的（这里还是不用新的目录了，可以在install里添加多个可执行程序，都放到一个目录里）
# TODO 注意一定不是在一行add_excetable里面直接加文件，因为之前是一个main，现在是多个main，一个main的话就add_excetable后面加几个文件就行其他不用改，多个main相当于多个可执行程序，所以要多生成一个新的(ament_target_dependencies同理)
# TODO 所以一个工作空间，对应多个功能包，一个功能包对应多个可执行程序，执行的时候是以可执行程序为单位执行，一个可执行程序下可有多个节点
# TODO 文件名与可执行程序名设置为一样的，这样知道可执行程序名是那个文件生成的（可执行程序名是cmakelist配置的，但是最好是和文件名一样吧）

# CPP代码编写-在终端输出文本hello world
# 1.包含头文件
#include "rclcpp/rclcpp.hpp"

# int main(int argc, char ** argv)
# {
#   // 2.初始化 ROS2客户端--初始化必然有释放
#   rclcpp::init(argc,argv);//参数是main函数里的参数
#   // 3.创建节点指针
#   auto node = rclcpp::Node::make_shared("helloworld_node");//创建Node对象的指针,参数是节点的名字
#   // 4.输出文本(日志)
#   RCLCPP_INFO(node->get_logger(),"hello world!");//参数一是日志对象(节点下面有个函数可以获取日志对象),参数二是输出的文本
#   // 5.释放资源
#   rclcpp::shutdown();
#   return 0;
# }
# vscode直接配置的话：ctrl+~打开终端

# TODO 下面的方法只是对一个工作空间(工程)有用,新工程就要换
# 每次终端中执行工作空间下的节点时，都需要调用. install/setup.bash指令，使用不便，优化策略是，可以将该指令的调用添加进~/setup.bash，操作格式如下：
#
# echo "source /{工作空间路径}/install/setup.bash" >> ~/.bashrc
# 示例：
#
# echo "source /home/ros2/ws00_helloworld/install/setup.bash" >> ~/.bashrc
# 以后再启动终端时，无需手动再手动刷新环境变量，使用更方便。

# 功能包创建的有问题，一般除非很清楚知道该哪些配置外，还是直接重新创建比较好
