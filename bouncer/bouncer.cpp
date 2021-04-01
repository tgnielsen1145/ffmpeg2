/*
  Copyright 2021 Todd Nielsen, Corey Buchanan
  This work may not be copied for academic use.
  
  Citations:
  https://github.com/loupus/ffmpeg_tutorial/blob/master/02_avformat_context.cpp - Help with decoding
  https://riptutorial.com/ffmpeg/example/30957/reading-from-a-format-context - Help with decoding
  
 */

/*
  Copyright 2021 Todd Nielsen, Corey Buchanan
  This work may not be copied for academic use.
  
  Citations:
  https://github.com/loupus/ffmpeg_tutorial/blob/master/02_avformat_context.cpp - Help with decoding
  https://riptutorial.com/ffmpeg/example/30957/reading-from-a-format-context - Help with decoding
  
 */

extern "C"
{
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
}

#include <iostream>

void decode_image(char *filename, AVFrame *frame);
void draw_ball();
void encode_images();

void decode_image(char *filename, AVFrame *frame) {
  AVFormatContext *format_context = NULL;
  if (avformat_open_input(&format_context, filename, NULL, NULL) < 0) {
    std::cout << "Failed to open file" << std::endl;
    exit(1);
  }

  if (avformat_find_stream_info(format_context, NULL) < 0) {
    std::cout << "Failed to get stream info" << std::endl;
    avformat_close_input(&format_context);
    exit(1);
  }
  
  AVCodec *codec = avcodec_find_decoder(format_context->streams[0]->codecpar->codec_id);
  AVCodecContext *codec_context = avcodec_alloc_context3(codec);
  avcodec_parameters_to_context(codec_context,format_context->streams[0]->codecpar);
  avcodec_open2(codec_context, codec, NULL);
  frame= av_frame_alloc();
  
  
  
  if (!codec_context) {
    std::cout << "Failed to set up context.." << std::endl;
    avformat_close_input(&format_context);
    exit(1);
  }

  AVPacket packet;
  av_read_frame(format_context, &packet);
  //av_init_packet(&packet);
  //packet.data = NULL;
  //packet.size = 0;


//  if (av_read_frame(format_context, &packet) == 0) {
   // std::cout << "We couldn't read a frame" << std::endl;
   // avformat_close_input(&format_context);
   // av_packet_unref(&packet);
   // exit(1);
  //}

  avcodec_send_packet(codec_context, &packet);

  
 
  if (avcodec_receive_frame(codec_context, frame) < 0){
    std::cout << "Fail 2" << std::endl;
    av_packet_unref(&packet);
    avformat_close_input(&format_context);
    av_frame_unref(frame);
    av_freep(frame);
    exit(1);
  }
  
  std::cout << "Got to step 6" << std::endl;
  
  std::cout << frame->width << " " << frame->height << std::endl; 

  std::cout << "Step 6.5" << std::endl;
  
  av_packet_unref(&packet);
  avformat_close_input(&format_context);
  
}

int main(int argc, char** argv) {

  if (argc != 2) {
    std::cout << "Usage: ./bouncer <image_path>" << std::endl;
    exit(1);
  }

  AVFrame *frame;

  decode_image(argv[1], frame);

  std::cout << "Got to step 7" << std::endl;
  
  std::cout << frame->width << " " << frame->height << std::endl; 

  std::cout << "Got to step 8" << std::endl;
  av_frame_unref(frame);
  av_freep(frame);

  /*
    decode_image(argv[1], frame)
    for(i in 1...300)
      draw_ball(frame, i)
        // Draw ball calculates where the ball is based on height, width of image, and frame number
      encode(frame, image_name) // <outputimagename>i.png - may depend on input argv[2]
   */

  return 0;
  
}
