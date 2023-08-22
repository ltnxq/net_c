
add_executable(MakeTable MakeTable.cpp)

#------------------add_custom_command--------------------------------------------
#output 生成的文件  command 在构建时候执行的命令 后面就是命令需要传递的参数
#DEPENDS 指的是所依赖的文件
#add the command to generate the source code
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable  ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
)

#将当前目录添加到头文件目录中
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add the main libray 后面的Table.h是构建的依赖项
add_library(MathFunctions mysqrt.cpp ${CMAKE_CURRENT_BINARY_DIR}/Table.h)


#安装头文件和静态库
#TARGETS包含六种形式：ARCHIVE, LIBRARY, RUNTIME, OBJECTS, FRAMEWORK, BUNDLE。
#注意Mathfunction安装的是LIBRARY，而根目录下的可执行文件是RUNTIME类型。
install(TARGETS MathFunctions DESTINATION bin)

#DESTINATION 指的是相对路径,取默认值,在unix中指向 /usr/local  windows中是c:/Program Files/${PROJECT_NAME}
#也可以通过设置CMAKE_INSTALL_PREFIX这个变量来设置安装的路径，那么安装位置不指向/usr/local，而指向你所指定的目录

#FILE 将给定的文件复制到指定目录。如果没有给定权限参数，则由该表单安装的文件默认为OWNER_WRITE、OWNER_READ、GROUP_READ和WORLD_READ
install(FILES MathFunction.h DESTINATION include)