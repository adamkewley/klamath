#include "src/formats/frm.hpp"

frm::Frame::Frame(Dimensions dimensions,
                  PixelShift pixel_shift,
                  std::vector<uint8_t> data) :
  _dimensions{dimensions},
  _pixel_shift{pixel_shift},
  _data(std::move(data)) {

    if (this->_data.size() != area(dimensions)) {
      throw std::runtime_error("invalid frm frame dimensions: do not match the amount of frm frame data");
    }
}

frm::Dimensions frm::Frame::dimensions() const noexcept {
  return this->_dimensions;
}

frm::PixelShift frm::Frame::pixel_shift() const noexcept {
  return this->_pixel_shift;
}

const std::vector<uint8_t>& frm::Frame::data() const noexcept {
  return this->_data;
}


frm::Animation::Animation(uint16_t fps,
                          uint16_t action_frame,
                          std::vector<Frame> frames) :
  _fps{fps},
  _action_frame{action_frame},
  _frames{std::move(frames)} {
  }

frm::Dimensions frm::Animation::dimensions() const noexcept {
  frm::Dimensions largest{0, 0};
  for (const frm::Frame& f : this->_frames) {
    largest = klmth::geometry::union_of(largest, f.dimensions());
  }
  return largest;
}

uint16_t frm::Animation::fps() const noexcept {
  return this->_fps;
}

uint16_t frm::Animation::action_frame() const noexcept {
  return this->_action_frame;
}

const std::vector<frm::Frame>& frm::Animation::frames() const noexcept {
  return this->_frames;
}


frm::File::File(std::vector<Animation> animations,
                std::array<uint8_t, num_orientations> animation_idxs) :
  _animations(std::move(animations)),
  _animation_idxs(std::move(animation_idxs)) {

  // TODO: validate orientations against animations.
}

const frm::Animation& frm::File::animation(Orientation o) const noexcept {
  return this->_animations[o];
}
