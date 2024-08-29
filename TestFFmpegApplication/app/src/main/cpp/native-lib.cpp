#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"FFmpegAudioPlayer",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"FFmpegAudioPlayer",FORMAT,##__VA_ARGS__);


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_testffmpegapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_testffmpegapplication_MainActivity_getConfiguration(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(avcodec_configuration());
}

extern "C" JNIEXPORT void
JNICALL
Java_com_example_testffmpegapplication_MainActivity_decodeVideo(
        JNIEnv *env,
        jobject /* this */, jstring _src, jstring _out) {
    const char *src = env->GetStringUTFChars(_src, 0);
    const char *out = env->GetStringUTFChars(_out, 0);

    // 初始化解封装
    av_register_all();
    // 初始化网络
    avformat_network_init();
    // 打开文件
    AVFormatContext *fmt_ctx = NULL;

    int ret = avformat_open_input(&fmt_ctx, src, 0, 0);
    if (ret == 0) {
        LOGI("avformat_open_input() called success.");
    } else {
        LOGE("avformat_open_input() called failed: %s", av_err2str(ret));
    }

    //读取视频格式文件信息
    int ret2 = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret2 == 0) {
        LOGI("avformat_find_stream_info() called success.");
    } else {
        LOGE("avformat_find_stream_info() called failed: %s", av_err2str(ret));
        return;
    }

    //获取视频索引
    int video_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            LOGI("find video stream index");
            break;
        }
    }

    //获取解码器
    AVCodecContext *codec_ctx = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[video_stream_index]->codecpar);
    AVCodec *codec = avcodec_find_decoder(codec_ctx->codec_id);
    //打开解码器
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        LOGE("could not open codec");
        return;
    }

    //分配AVPacket和AVFrame内存，用于接收视频数据，解码数据
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    int got_frame;//接收解码结果
    int index = 0;
    //yuv输出文件
    FILE *out_file = fopen(out, "wb");
    while (av_read_frame(fmt_ctx, packet) == 0) {//将视频数据读入packet
        if (packet->stream_index == video_stream_index) {//取视频索引packet
            //TODO:函数修改
            if (avcodec_decode_audio4(codec_ctx, frame, &got_frame, packet) <
                0) {//将packet解码成AVFrame
                LOGE("decode error:%d", index);
                break;
            }
            if (got_frame > 0) {
                LOGI("decode frame:%d", index++);
                fwrite(frame->data[0], 1, static_cast<size_t>(frame->linesize[0]),
                       out_file); //想将单个声道pcm数据写入文件
            }
        }
    }
    LOGI("decode finish...");
    //释放资源
    av_packet_unref(packet);
    av_frame_free(&frame);
    avcodec_close(codec_ctx);
    avformat_close_input(&fmt_ctx);
    fclose(out_file);


    //TODO：完善读流后续流程
}

extern "C" JNIEXPORT void
JNICALL
Java_com_example_testffmpegapplication_MainActivity_decodeAudio(
        JNIEnv *env,
        jobject /* this */, jstring _src, jstring _out) {
    const char *src = env->GetStringUTFChars(_src, 0);
    const char *out = env->GetStringUTFChars(_out, 0);

    // 初始化解封装
    avcodec_register_all();
    // 初始化网络
    avformat_network_init();
    // 准备打开文件
    AVFormatContext *fmt_ctx = NULL;//设置成Null的时候，ffmpeg内部会自动生成一个AVFormatContext指针

    //打开文件
    int ret = avformat_open_input(&fmt_ctx, src, NULL, NULL);
    if (ret == 0) {
        LOGI("avformat_open_input() called success.");
    } else {
        LOGE("avformat_open_input() called failed: %s", av_err2str(ret));
        return;
    }

    //读取音频格式文件信息
    int ret2 = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret2 == 0) {
        LOGI("avformat_find_stream_info() called success.");
    } else {
        LOGE("avformat_find_stream_info() called failed: %s", av_err2str(ret));
        return;
    }

    //获取音频索引
    int audio_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            LOGI("find audio stream index");
            break;
        }
    }
    //获取解码器
    AVCodecContext *codec_ctx = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[audio_stream_index]->codecpar);
    AVCodec *codec = avcodec_find_decoder(codec_ctx->codec_id);
    //打开解码器
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        LOGE("could not open codec");
        return;
    }
    //分配AVPacket和AVFrame内存，用于接收音频数据，解码数据
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    int got_frame;//接收解码结果
    int index = 0;
    //pcm输出文件
    FILE *out_file = fopen(out, "wb");
    while (av_read_frame(fmt_ctx, packet) == 0) {//将音频数据读入packet
        if (packet->stream_index == audio_stream_index) {//取音频索引packet
            if (avcodec_decode_audio4(codec_ctx, frame, &got_frame, packet) <
                0) {//将packet解码成AVFrame
                LOGE("decode error:%d", index);
                break;
            }
            if (got_frame > 0) {
                LOGI("decode frame:%d", index++);
                fwrite(frame->data[0], 1, static_cast<size_t>(frame->linesize[0]),
                       out_file); //想将单个声道pcm数据写入文件

            }
        }
    }
    LOGI("decode finish...");
    //释放资源
    av_packet_unref(packet);
    av_frame_free(&frame);
    avcodec_close(codec_ctx);
    avformat_close_input(&fmt_ctx);
    fclose(out_file);
}