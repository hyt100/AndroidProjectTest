package com.example.media;
import android.util.Log;

public class MediaSt {
    private String Tag = "medialib";
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public String get_version()
    {
        //VideoDecTest();
        LiveStreamTest();
        return stringFromJNI();
    }

    private void VideoDecTest()
    {
        byte[] data = new byte[2];
        data[0] = 11;
        data[1] = 22;

        long handle = VideoDecCreate();
        VideoDecFlush(handle);
        VideoDecWrite(handle, data, 2, 1000, 2000);
        VideoDecRead(handle, data, 2, 5000);
        VideoDecDestroy(handle);

        Log.v("medialib",  " " + data[0] + data[1]);
    }

    private void LiveStreamTest()
    {
        byte[] data = new byte[2];
        data[0] = 11;
        data[1] = 22;

        long handle = LiveStreamCreate();
        LiveStreamWrite(handle, data, 2, 1000, 1);
        LiveStreamStartRecord(handle, "hello");
        LiveStreamStopRecord(handle);
        LiveStreamDestroy(handle);

        VideoExtractFrame("123", "456", 3, 1);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private native String stringFromJNI();

    // native method: decoder
    private native long VideoDecCreate();
    private native int VideoDecDestroy(long handle);
    private native int VideoDecFlush(long handle);
    private native int VideoDecWrite(long handle, byte[] data, int dataLen, long pts, int timeoutMs);
    private native int VideoDecRead(long handle, byte[] out, int outLen, int timeoutMs);

    // native method: live stream
    private native long LiveStreamCreate();
    private native int LiveStreamDestroy(long handle);
    private native int LiveStreamWrite(long handle, byte[] data, int dataLen, long pts, int timeoutMs);
    private native int LiveStreamStartRecord(long handle, String filename);
    private native int LiveStreamStopRecord(long handle);
    //private native void LiveStreamOnCallbackFrame(byte[] data, int width, int height, long pts, int format);
    private void LiveStreamOnCallbackFrame(byte[] data, int width, int height, long pts, int format)
    {
        Log.v(Tag, "callback => " + " widht:" + width + " height:" + height + " pts:" + pts);
    }

    // native method: video extrace frame
    private native int VideoExtractFrame(String videoFile, String ouputPath, int num, int hwEnable);
}
