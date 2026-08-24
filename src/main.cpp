#include <iostream>
#include <string>
#include <vector>

#include "abr_encoder.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_dir>\n";
        return 1;
    }

    const std::string input_file = argv[1];
    const std::string output_dir = argv[2];

    const std::vector<VideoProfile> profiles = {
        {1920, 1080, 5'000'000, "_1080p"},
        {1280, 720, 2'500'000, "_720p"},
        {640, 360, 1'000'000, "_360p"},
    };

    ABREncoder encoder;

    if (!encoder.open_input(input_file)) {
        std::cerr << "Failed to open input: " << input_file << "\n";
        return 1;
    }

    if (!encoder.initialize_decoders()) {
        std::cerr << "Failed to initialize decoders\n";
        return 1;
    }

    if (!encoder.initialize_encoders(profiles, output_dir)) {
        std::cerr << "Failed to initialize encoders\n";
        return 1;
    }

    if (!encoder.process_frames()) {
        std::cerr << "Failed while processing frames\n";
        return 1;
    }

    encoder.cleanup();
    return 0;
}
