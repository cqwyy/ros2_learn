# 在package.xml中需要添加一些依赖包，具体内容如下：
# <build_depend>rosidl_default_generators</build_depend> 构建依赖
# <exec_depend>rosidl_default_runtime</exec_depend>  执行依赖
# <member_of_group>rosidl_interface_packages</member_of_group>所属功能包组
# 加完之后后面test_depend报错，就删掉报错的即可

# 为了将.msg文件转换成对应的C++和Python代码，还需要在CMakeLists.txt中添加如下配置：
#
# find_package(rosidl_default_generators REQUIRED)  查找的功能包，把构建依赖添加到这
#
# rosidl_generate_interfaces(${PROJECT_NAME}  指定一下当前被构建的接口文件的路径，通过这个函数就能转换成对应c++代码
#   "msg/Student.msg"
# )



# TODO .msg文件名首字母必须大写
# TODO 编译完后，在install/功能包名/include下就有cpp源码,后面导入的也是这里的源码
# 或者
# . install/setup.bash
# ros2 interface show base_interfaces_demo/msg/Student
# 正常情况下，终端将会输出与Student.msg文件一致的内容。

# 头文件路径和对象路径基本一致，文件名首字母大写，导入的hpp首字母小写，对象名大写
#include "base_interfaces_demo/action/nav.hpp"
# using base_interfaces_demo::action::Nav;


# C++文件中包含自定义消息相关头文件时，可能会抛出异常，可以配置VSCode中c_cpp_properties.json文件，在文件中的 includePath属性下添加一行
# ："${workspaceFolder}/install/base_interfaces_demo/include/**"
#
# 添加完毕后，包含相关头文件时，就不会抛出异常了，其他接口文件或接口包的使用也与此同理。

# TODO 有新节点cmakelists就修改：add_executable、ament_target_dependencies，并在install添加新的可执行文件（xml不用改）

# 一个话题下可以有多个订阅方和发布方，这样一个订阅方可以订阅多个发布方的消息，发布方可以向多个订阅方发布消息