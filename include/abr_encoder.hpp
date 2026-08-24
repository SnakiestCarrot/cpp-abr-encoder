#pragma once

#include <cstdint>
#include <string>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}

// One target rendition in the ABR ladder.
struct VideoProfile {
    int width;
    int height;
    int64_t bitrate;      // bits per second
    std::string suffix;   // appended to output filename, e.g. "_1080p"
};

class ABREncoder {
public:
    ABREncoder() = default;
    ~ABREncoder();

    // Not copyable: owns raw FFmpeg resources.
    ABREncoder(const ABREncoder&) = delete;
    ABREncoder& operator=(const ABREncoder&) = delete;

    bool open_input(const std::string& input_path);
    bool initialize_decoders();
    bool initialize_encoders(const std::vector<VideoProfile>& profiles,
                              const std::string& output_dir);
    bool process_frames();
    void cleanup();

private:
    // --- Input / decode side ---
    AVFormatContext* input_format_ctx_ = nullptr;
    AVCodecContext* decoder_ctx_ = nullptr;
    int video_stream_index_ = -1;

    // --- Output / encode side (one entry per ladder rung) ---
    std::vector<VideoProfile> profiles_;
    std::vector<AVFormatContext*> output_format_ctxs_;
    std::vector<AVCodecContext*> encoder_ctxs_;
    std::vector<SwsContext*> sws_ctxs_;
    std::vector<AVStream*> output_streams_;
};
