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
      
      T area() const noexcept {
	return this->width * this->height;
      }
    };

    template<typename T>
    Dimensions<T> union_of(Dimensions<T> a, Dimensions<T> b) {
      T width = std::max(a.width, b.width);
      T height = std::max(a.height, b.height);
      return { width, height };
    }

    template<typename TOut, typename TIn>
    Dimensions<TOut> cast(Dimensions<TIn> d) {
      TOut width = static_cast<TOut>(d.width);
      TOut height = static_cast<TOut>(d.height);
      return { width, height };
    }

    template<
      typename T,
      typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    struct Point {
      T x;
      T y;
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

