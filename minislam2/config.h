#pragma once

#include <string>

// TODO: ingest config as CLI args

namespace Config {

const std::string dataset_path = "../eval/test";

bool show_img = true;
bool show_map2d = true;
bool show_map3d = true;

const auto fx = 718.856;
const auto fy = 718.856;
const auto cx = 607.2;
const auto cy = 185.2;

const auto w = 1241;
const auto h = 376;

const auto tw = 500;
const auto th = 500;

const auto scale = 0.4;

const auto map3d_w = 800;
const auto map3d_h = 550;

}  // namespace Config