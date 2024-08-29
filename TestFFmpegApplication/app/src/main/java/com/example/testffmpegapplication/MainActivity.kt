package com.example.testffmpegapplication

import android.os.Bundle
import android.os.Environment
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.example.testffmpegapplication.databinding.ActivityMainBinding
import java.io.File

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.tvVersion.text = getConfiguration()
        binding.btnTransform.setOnClickListener{
            decodeVideo()
        }
    }


    //音频解码
    private fun decodeAudio() {
        val input = "/storage/emulated/0/test1.mp3"
        val outPut = "/hik/app/out.pcm"

        val file = File(input)
        if (file.exists()) {
            println("file exists")
            // 文件存在，进行读取操作
        } else {
            println("ERROR！！file doesn't exist")
            // 文件不存在，进行相应处理
        }
        decodeAudio(input,outPut)
    }

    //视频解码
    //TODO：完善读流后续流程
    private fun decodeVideo() {
        val src = "/hik/app/911Mothers_2010W-480p.mp4"
        val out = "/hik/app/out.yuv"
        println("src:$src")

        val file = File(File("/hik/app"), "911Mothers_2010W-480p.mp4")
        if (file.exists()) {
            println("file exists")
            // 文件存在，进行读取操作
        } else {
            println("ERROR！！file doesn't exist")
            // 文件不存在，进行相应处理
        }

        decodeVideo(src,out)
    }



    external fun decodeAudio(src: String, out: String)
    external fun decodeVideo(src: String, out: String)

    /**
     * 在 ffmpegNdkCustom.so 库中声明的原生c++的native方法，起连接作用
     */
    external fun stringFromFFTest(): String
    external fun getConfiguration(): String?

    companion object {
        init {
            //用于在程序启动时加载cmake编译好的目标库testffmpegapplication.so，注意大小写
            System.loadLibrary("testffmpegapplication")
        }
    }
}