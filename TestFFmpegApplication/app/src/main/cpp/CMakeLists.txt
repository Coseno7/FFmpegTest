cmake_minimum_required(VERSION 3.22.1)


#自动生成，自己的项目名
project("testffmpegapplication")

message("start............................................................")
# 1. 定义so库和头文件所在目录，方面后面使用
set(ffmpeg_lib_dir_neway ${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI})
set(ffmpeg_head_dir ${CMAKE_SOURCE_DIR}/include)

# 2. 添加头文件目录
include_directories(${ffmpeg_head_dir})

#==========================ffmpeg引入开始====================================
#ffmpegsofile为自定义名称，libffmpeg.so是libs目录下的linux编译出的so文件
add_library( libavcodec
        SHARED
        IMPORTED)
set_target_properties(
        libavcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir_neway}/libavcodec.so)

add_library( libavfilter
        SHARED
        IMPORTED)
set_target_properties(
        libavfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir_neway}/libavfilter.so)

add_library( libavformat
        SHARED
        IMPORTED)
set_target_properties(
        libavformat
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir_neway}/libavformat.so)

add_library( libavutil
        SHARED
        IMPORTED)
set_target_properties(
        libavutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir_neway}/libavutil.so)

add_library( libswresample
        SHARED
        IMPORTED)
set_target_properties(
        libswresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir_neway}/libswresample.so)

add_library( libswscale
        SHARED
        IMPORTED)
set_target_properties(
        libswscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir_neway}/libswscale.so)
#==========================ffmpeg引入结束====================================


add_library(
        testffmpegapplication
        SHARED
        native-lib.cpp)

add_library(
        testffmpeg
        SHARED
        ffmpeg-test.cpp)

find_library(
        log-lib
        log)

#指定编译最终目标库时，cmke要链接的库。
#解释：一般形式是：target_link_libraries(your-lib library1 library2 ...)
target_link_libraries(
        #指定目标库
        testffmpegapplication
        #自定义的测试库
        testffmpeg
        #连接 FFmpeg 相关的库
        libavcodec          #编解码
        libavfilter         #滤镜特效处理
        libavformat         #封装格式处理
        libavutil           #工具库
        libswresample       #音频采样数据格式转换
        libswscale          #视频像素数据格式转换
        ${log-lib})

