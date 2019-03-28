#pragma once

namespace klmth {
  namespace geometry {
    template<
      typename T,
      typename = typename std::enable_if<std::is_unsigned<T>::value, T>::type>
    struct Dimensions {
      T width;
      T height;

      Dimensions() noexcept : width(0), height(0) {
      }

      Dimensions(T _width, T _height) noexcept : width(_width), height(_height) {
      }

      template<typename TOther>
      Dimensions(Dimensions<TOther> d) noexcept :
	width(static_cast<T>(d.width)),
	height(static_cast<T>(d.height)) {
      }

      Dimensions<T>& operator+=(Dimensions other) noexcept {
        this->width += other.width;
        this->height += other.height;
        return *this;
      }
    };

    template<typename T>
    Dimensions<T> union_of(Dimensions<T> a, Dimensions<T> b) noexcept {
      T width = std::max(a.width, b.width);
      T height = std::max(a.height, b.height);
      return { width, height };
    }

    template<typename T>
    T area(Dimensions<T> d) noexcept {
      return d.width * d.height;
    }

    template<typename T>
    auto area(T in) -> decltype(area(in.dimensions())) {
      return area(in.dimensions());
    }

    template<
      typename T,
      typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    struct Point {
      T x;
      T y;

      Point<T> operator+(Point<T> other) const noexcept {
	T x = this->x + other.x;
	T y = this->y + other.y;

	return { x, y };
      }
      
      Point<T>& operator+=(Point<T> other) noexcept {
	this->x += other.x;
	this->y += other.y;
	return *this;
      }
    };

    template<
      typename TPosition,
      typename TDimensions,
      typename = typename std::enable_if<std::is_arithmetic<TPosition>::value, TPosition>::type,
      typename = typename std::enable_if<std::is_unsigned<TDimensions>::value, TDimensions>::type>
    struct Rect {
      Point<TPosition> location;
      Dimensions<TDimensions> dimensions;
    };
  }
}

