package com.example.zw.audiocourse.util;

import android.media.MediaCodecList;

import java.util.HashMap;

public class VideoFormatSupportUtil {
    private static HashMap<String,String>codecMap=new HashMap<>();
    static {
        codecMap.put("h264", "video/avc");
    }
    public static String findVideoCodecName(String ffcodecName){
        if(codecMap.containsKey(ffcodecName)){
            return codecMap.get(ffcodecName);
        }
        return  "";
    }
    public static boolean isSupportCodec(String ffcodecName){
        boolean supportVideo=false;
        int count=MediaCodecList.getCodecCount();
        for(int i=0;i<count;i++){
            String[]types=MediaCodecList.getCodecInfoAt(i).getSupportedTypes();
            for(int j=0;j<types.length;j++){
                if(types[j].equals(findVideoCodecName(ffcodecName)))//字符串的equals相当于==,进行了重写
                {
                    supportVideo=true;
                    break;
                }
            }
            if(supportVideo){
                break;
            }
        }
        return supportVideo;
    }

}
