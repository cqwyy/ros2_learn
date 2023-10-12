# 功能包下新建launch文件夹
# launch需要配置配置cmakaelists，加一个install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})
# 建议也在xml里添加一个执行时依赖<exec_depend>ros2launch</exec_depend>
# 命名python文件最后以_launch.py或者.launch.py结尾

# 节点设置
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

