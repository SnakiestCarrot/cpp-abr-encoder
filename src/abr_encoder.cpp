#include "abr_encoder.hpp"

#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavcodec/codec_id.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>

ABREncoder::~ABREncoder() {
    cleanup();
}

bool ABREncoder::open_input(const std::string& input_path) {
    if (avformat_open_input(&input_format_ctx_, input_path.c_str(), nullptr, nullptr) < 0) {
        return false;
    }

    if (avformat_find_stream_info(input_format_ctx_, nullptr) < 0) {
        return false;
    }

    video_stream_index_ = av_find_best_stream(input_format_ctx_, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    
    if (video_stream_index_ < 0) {
        return false;
    }

    return true;
}

bool ABREncoder::initialize_decoders() {
    // TODO: avcodec_find_decoder, avcodec_alloc_context3,
    // avcodec_parameters_to_context, avcodec_open2.
    const AVCodec* decoder = avcodec_find_decoder(input_format_ctx_->streams[video_stream_index_]->codecpar->codec_id);
    if (!decoder) {
        return false;
    }
    
    decoder_ctx_ = avcodec_alloc_context3(decoder);
    
    if (!decoder_ctx_) {
        return false;
    }
    
    if (avcodec_parameters_to_context(decoder_ctx_, input_format_ctx_->streams[video_stream_index_]->codecpar) < 0) {
        return false;
    }

    if (avcodec_open2(decoder_ctx_, decoder, nullptr) < 0) {
        return false;
    }
    return true;
}

bool ABREncoder::initialize_encoders(const std::vector<VideoProfile>& profiles,
                                      const std::string& output_dir) {
    profiles_ = profiles;
    (void)output_dir;

    // TODO: Configure AVCodecContext for rate control and strict GOP/keyframe alignment

    return false;
}

bool ABREncoder::process_frames() {
    // TODO: Implement av_read_frame loop, decode to AVFrame, scale using sws_scale, encode, and mux
    return false;
}

void ABREncoder::cleanup() {
    for (auto* sws : sws_ctxs_) {
        if (sws) {
            sws_freeContext(sws);
        }
    }
    sws_ctxs_.clear();
    for (auto* enc_ctx : encoder_ctxs_) {
        if (enc_ctx) {
            avcodec_free_context(&enc_ctx);
        }
    }
    encoder_ctxs_.clear();

    for (auto* out_ctx : output_format_ctxs_) {
        if (out_ctx) {
            if (out_ctx->pb) {
                avio_closep(&out_ctx->pb);
            }
            avformat_free_context(out_ctx);
        }
    }
    output_format_ctxs_.clear();

    output_streams_.clear();

    if (decoder_ctx_) {
        avcodec_free_context(&decoder_ctx_);
    }

    if (input_format_ctx_) {
        avformat_close_input(&input_format_ctx_);
    }

    video_stream_index_ = -1;
}
