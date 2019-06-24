#pragma once

#include <cmath>

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

      constexpr Dimensions(T _width, T _height) noexcept : width(_width), height(_height) {
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

      Dimensions<T> operator+(Dimensions<T> other) const noexcept {
        return {
          static_cast<T>(this->width + other.width),
            static_cast<T>(this->height + other.height),
        };
      }

      Dimensions<T> operator*(unsigned scalar) const noexcept {
        return {
          static_cast<T>(this->width * scalar),
            static_cast<T>(this->height * scalar),
        };
      }
    };

    template<typename T>
    Dimensions<T> union_of(Dimensions<T> a, Dimensions<T> b) noexcept {
      T width = std::max(a.width, b.width);
      T height = std::max(a.height, b.height);
      return { width, height };
    }

    template<typename T>
    auto width(T in) -> decltype(in.dimensions().width) {
      return in.dimensions().width;
    }

    template<typename T>
    auto width(T in) -> decltype(in.dimensions.width) {
      return in.dimensions.width;
    }

    template<typename T>
    auto height(T in) -> decltype(in.dimensions().height) {
      return in.dimensions().height;
    }

    template<typename T>
    auto height(T in) -> decltype(in.dimensions.height) {
      return in.dimensions.height;
    }

    template<typename T>
    T area(Dimensions<T> d) noexcept {
      return d.width * d.height;
    }

    template<typename T>
    auto area(T in) -> decltype(area(in.dimensions())) {
      return area(in.dimensions());
    }

    template<typename T>
    auto area(T in) -> decltype(area(in.dimensions)) {
      return area(in.dimensions);
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

    
    template<typename T>
    Point<typename std::make_unsigned<T>::type> abs(Point<T> in) {
      return { 0U, 0U };
    }

    template<typename T>
    Dimensions<typename std::make_unsigned<T>::type> dimensions(Point<T> in) {
      // The dimensions of a point are the dimensions of the rectangle
      // formed between the origin and the point.
      
      using ValType = typename std::make_unsigned<T>::type;

      ValType x = static_cast<ValType>(std::abs(in.x));
      ValType y = static_cast<ValType>(std::abs(in.y));
      
      return { x, y };
    }

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

