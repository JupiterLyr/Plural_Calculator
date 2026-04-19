cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME PluralCalculator)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 Qt 的安装目录
set(CMAKE_PREFIX_PATH "D:/ProgramData/Qt/6.9.1/mingw_64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

# 设置头文件目录
file(GLOB HEADER_FILES
    "${PROJECT_SOURCE_DIR}/includes/*.h"
)
# 添加源文件
file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/srcs/*.cpp"
)
# 处理 UI 文件
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(UI_FILES "${PROJECT_SOURCE_DIR}/resources/mainwindow.ui")
# 资源配置文件
set(RC_FILES "${PROJECT_SOURCE_DIR}/res_config.qrc" "${PROJECT_SOURCE_DIR}/icon_config.rc")
# 添加可执行文件
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
    ${RC_FILES}
)
# 头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/includes
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/include"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE   # 不显示 CMD 窗口
)

# 链接 Qt 库
target_link_libraries(
    ${PROJECT_NAME} PRIVATE 
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
)

# 复制 resource 文件夹
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # 一次性拷贝整个 resource
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
    COMMENT "Copying resources/ to $<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

# ==================== 安装规则 ====================
set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/Release")   # build/Release

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DEPLOY_DIR}"
)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/qml"
        DESTINATION "${DEPLOY_DIR}"
        CONFIGURATIONS Release MinSizeRel RelWithDebInfo
)

# 找到 windeployqt 可执行文件
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

# 每次构建完成后自动运行 windeployqt
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E echo ">>> Running windeployqt..."
    COMMAND "${WINDEPLOYQT_EXECUTABLE}"
            # --no-compiler-runtime   # 不自动拷 VC 运行库，想拷就去掉
            --no-translations       # 不需要翻译包
            --qmldir "${CMAKE_SOURCE_DIR}/qml"
            "$<TARGET_FILE:${PROJECT_NAME}>"
    WORKING_DIRECTORY "${DEPLOY_DIR}"
    COMMENT "Deploying Qt dependencies..."
    VERBATIM
)

qt_finalize_executable(${PROJECT_NAME})
