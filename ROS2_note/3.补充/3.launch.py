# 功能包下新建launch文件夹
# launch需要配置配置cmakalists，加一个install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
# TODO 新建文件夹都要有这步操作
# 建议也在xml里添加一个执行时依赖<exec_depend>ros2launch</exec_depend>
# 命名python文件最后以_launch.py或者.launch.py结尾

# 节点设置（生成的变量都要放到return列表，意思是生成的变量都算是节点，还是所有变量都要放入？）--调用多次的节点一定要设置不同的名称，这里解决重名问题的成本最低“
# launch中需要执行的节点被封装为launch_ros.actions.Node对象，可以用来配置节点相关的属性

# 执行指令
# 封装为launch.actions.ExecuteProcess对象，可以在launch文件中执行ros指令，简化部分功能调用

# 参数设置(增强launch文件启动的灵活性)
# 声明被封装为launch.actions.DeclareLaunchArgument   调用被封装为launch.substitutions import LaunchConfiguration

# 文件包含
# launch中可以包含其他launch文件，API为launch.actions.IncludeLaunchDescription和launch.launch_description_sources.PythonLaunchDescriptionSource

# 分组设置
# 可以对多个Node进行分组

# 添加事件
# 节点在运行过程中会触发不同的事件，当事件触发时可以注册一定的处理逻辑，事件使用相关API为launch.actions.RegisiterEventHandler、
# launch.event_handlers.OnProcessStart、launch.event_handlers.OnProcessExit

# 运行：ros2 launch 所在包名 launch文件名（含扩展名）
# 有DeclareLaunchArgument时可以动态传参  ros2 launch 包名 model:=    (model是launch中由DeclareLaunchArgument定义的参数名)
# 有DeclareLaunchArgument时可以动态传参,并集成指令例子  ros2 launch 包名 model:=`ros2 pkg prefix --share cpp06_urdf`  注意不是单引号，是键盘左上角，1前面那个

# TODO launch涉及的名字有点多，尽量让含义一样的东西同名，比如节点变量名和节点名一样，防止混乱
